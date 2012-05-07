#include "LightHandler.h"
#include "Graphics.h"
#include "Defines.h"
#include "BasicLight.h"
#include "Vertex.h"

LightHandler::LightHandler(void)
{

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
			if(!o->getCastsShadow())
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
		HR(gd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));
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
}

void LightHandler::drawShadow(Object* obj, BasicLight* lightSource)
{
	Vector prevPoint;
	Vector thisPoint;
	bool backFacing[32];
	//calculate facing to find edge
	for(int i = 0;i < obj->getShadowBase().mVertices.size(); i++)
	{
		prevPoint = Vector(obj->getShadowBase().mVertices[i].x, obj->getShadowBase().mVertices[i].y) + obj->getShadowBase().getPos();
        int secondIndex = (i + 1) % obj->getShadowBase().mVertices.size();
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
		int nextEdge = (i + 1) % obj->getShadowBase().mVertices.size();
	
		if (backFacing[currentEdge] && !backFacing[nextEdge])
			endingIndex = nextEdge;//nextEdge

		if (!backFacing[currentEdge] && backFacing[nextEdge])
			startingIndex = nextEdge;
    }

	//check if wrong order
	bool mod = false;
	if(startingIndex>=endingIndex) 
	{
		mod = true;
		int t = endingIndex;
		endingIndex = startingIndex;
		startingIndex = t;
	}

	//Draw
	temp.clear();
	if(mod)
		thisPoint = obj->getShadowBase().mVertices[startingIndex];
	else
		thisPoint = obj->getShadowBase().mVertices[endingIndex];
	float a=gMath->calculateAngle(lightSource->getPos(),thisPoint);
	Vector v = Vector(cosf(a), sinf(a));
	v.multiply(100000);
	v.add(thisPoint);
	temp.addPoint(v);

	for (int i = startingIndex;i <= endingIndex;i++)
		temp.addPoint(obj->getShadowBase().mVertices[i]);

	if(!mod)
		thisPoint = obj->getShadowBase().mVertices[startingIndex];
	else
		thisPoint = obj->getShadowBase().mVertices[endingIndex];
	a=gMath->calculateAngle(lightSource->getPos(),thisPoint);
	v = Vector(cosf(a), sinf(a));
	v.multiply(100000);
	v.add(thisPoint);
	temp.addPoint(v);

	gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	gGraphics->drawPolygon(&temp, D3DCOLOR_ARGB(0,255,255,255),false,0.0f);
	gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
