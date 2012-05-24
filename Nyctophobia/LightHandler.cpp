#include "LightHandler.h"
#include "Graphics.h"
#include "Defines.h"
#include "BasicLight.h"
#include "Vertex.h"

LightHandler::LightHandler(void)
{
	HR(gd3dDevice->CreateOffscreenPlainSurface(SCREEN_WIDTH,SCREEN_HEIGHT,D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &mReadSurface, NULL));
}

//Note, no remove from vectors!
LightHandler::~LightHandler(void)
{
	
}

void LightHandler::addEmitter(BasicLight* LightSource)
{
	mEmitters.push_back(LightSource);
}
void LightHandler::addObstacle(Object* obj)
{
	mObstacles.push_back(obj);
}

void LightHandler::clean()
{
	mEmitters.clear();
	mObstacles.clear();
}

void LightHandler::doShadow()
{
	for(int j = 0; j<mEmitters.size();j++)
	{
		BasicLight *light = mEmitters.at(j);

		for(int i = 0; i<mObstacles.size();i++)
		{
			Object* o = mObstacles.at(i);
			if(!o->getCastsShadow() && gMath->distance(o->getPos(), light->getPos()) < light->getRange())
				continue;
			//Shadow to alpha
			HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND,		  D3DBLEND_ONE));
			HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND,		  D3DBLEND_ZERO));
			HR(gd3dDevice->SetRenderState(D3DRS_ZENABLE,		  D3DZB_FALSE));
			HR(gd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA));
			drawShadow(o, light);
			//Remove object overlap from alpha - illuminate object
			o->draw();
		}
		//Draw light to texture, modulated by alpha
		HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND,		  D3DBLEND_DESTALPHA));
		HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND,		  D3DBLEND_ONE));//D3DBLEND_SRCALPHA - ONE
		HR(gd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED));
		HR(gd3dDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE));
		light->draw();

		//Clear alpha
		HR(gd3dDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE));
		HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND,		  D3DBLEND_ONE));
		HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND,		  D3DBLEND_ZERO));
		HR(gd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA));
		gGraphics->drawRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255,0,0,0), true, false);
	
	}
	//Reset
	HR(gd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND,		  D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND,		  D3DBLEND_INVSRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_ZENABLE,		  D3DZB_TRUE));

	//Save player shadow
	//get color data from surface
	IDirect3DSurface9* surface;
	HR(gd3dDevice->GetRenderTarget(0,&surface));
	HR(gd3dDevice->GetRenderTargetData(surface, mReadSurface));
	D3DLOCKED_RECT locked;
	HR(mReadSurface->LockRect(&locked,NULL,D3DLOCK_READONLY)); //(0, ... for texture
	BYTE *bytePointer=(BYTE*)locked.pBits;

	int x = SCREEN_WIDTH/2;
	int y = SCREEN_HEIGHT/2;
	DWORD index=(x*4+(y*(locked.Pitch)));
	// Red
	int r=bytePointer[index+2];
	mReadSurface->UnlockRect();
	float f = float(r/256.0f);
	mCurrentShade = 1-f;
}

void LightHandler::drawShadow(Object* obj, BasicLight* lightSource)
{
	Vector prevPoint;
	Vector thisPoint;
	bool backFacing[32];
	int vertexCount = obj->getShadowBase().mVertices.size();
	//calculate facing to find edge
	for(int i = 0;i < obj->getShadowBase().mVertices.size(); i++)
	{
		prevPoint = Vector(obj->getShadowBase().mVertices[i].x, obj->getShadowBase().mVertices[i].y) + obj->getShadowBase().getPos();
        int secondIndex = (i + 1) % vertexCount;
        thisPoint = Vector(obj->getShadowBase().mVertices[secondIndex].x, obj->getShadowBase().mVertices[secondIndex].y) + obj->getShadowBase().getPos();
        Vector middle = (prevPoint + thisPoint) / 2;
		
		Vector L = lightSource->getPos() - middle;

        Vector N;
        N.x = - (thisPoint.y - prevPoint.y);
        N.y = thisPoint.x - prevPoint.x;
                
		if (N.dot(L) > 0)
			backFacing[i] = false;
        else
			backFacing[i] = true;
	}

	//find beginning and ending vertices belonging to shadow
    int startingIndex=0;
    int endingIndex=0;
    for (int i = 0; i < obj->getShadowBase().mVertices.size(); i++)
    {
		int currentEdge = i;
		int nextEdge = (i + 1) % vertexCount;
	
		if (backFacing[currentEdge] && !backFacing[nextEdge])
			endingIndex = nextEdge;//nextEdge

		if (!backFacing[currentEdge] && backFacing[nextEdge])
			startingIndex = nextEdge;
    }

	//Draw
	int shadowVertexCount;

	if (endingIndex > startingIndex)
		shadowVertexCount = endingIndex - startingIndex+1;
	else
		shadowVertexCount = vertexCount + 2 - startingIndex + endingIndex ;

	shadowPoints.clear();

		//create a triangle strip that has the shape of the shadow
	int currentIndex = startingIndex;
	int svCount = 0;
	while (svCount != shadowVertexCount*2)
	{
		Vector vertexPos = obj->getShadowBase().mVertices[currentIndex] + obj->getPos();//vertices[currentIndex].Position + new Vector3(position,0);

		//one vertex on the hull
		shadowPoints.push_back(vertexPos);

		//one extruded by the light direction
		Vector L2P = vertexPos - Vector(lightSource->getPos());
		L2P.normalize();
		shadowPoints.push_back(lightSource->getPos() + (L2P *9000));

		svCount+=2;
		currentIndex = (currentIndex + 1) % vertexCount;
	}

	gGraphics->drawShadowShape(&shadowPoints, D3DCOLOR_ARGB(0,255,255,255));
}
