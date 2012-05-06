#include "Editor.h"
#include "Terrain.h"
#include "Graphics.h"
#include "Input.h"
#include "Level.h"
#include "LightHandler.h"
#include "Gamestate.h"
#include "Defines.h"

Editor::Editor()
{
	mShadowEnabled = true;
	mArt_ShadowOn = gGraphics->loadTexture("Data\\Images\\Editor\\ICO-shadow-on.png");
	mArt_ShadowOff = gGraphics->loadTexture("Data\\Images\\Editor\\ICO-shadow-off.png");
	mToolSelected = TOOL_TERRAIN;
	mTerrainSelected = 0;
	mDrawRadius = 40;
	mDrawIntensity = 0.10f;
	mCounter = 0;
	mInitialized = false;
}

Editor::~Editor()
{
	for(int i = 0; i<mTerrainBlender.size(); i++)
		delete mTerrainBlender[i];
	mTerrainBlender.clear();
	ReleaseCOM(mArt_ShadowOn);
	ReleaseCOM(mArt_ShadowOff);
}

/// Note: do not use during draw()!
void Editor::addTerrainLayer(Terrain *terrain)
{
	DrawableTex2D* texture = new DrawableTex2D(512,512, 0, D3DFMT_A8R8G8B8, true, D3DFMT_D24S8, true);
	texture->beginScene();
	gGraphics->drawTexture(terrain->getBlendTexture(),256,256,512,512);
	texture->endScene();
	terrain->releaseBlend();
	terrain->setBlendTexture(texture->d3dTex());
	mTerrainBlender.push_back(texture);
}

void Editor::init()
{
	for(int i = 0; i < mActiveLevel->getTerrainList().size(); i++)
		addTerrainLayer(mActiveLevel->getTerrainList().at(i));
	mInitialized = true;
}

void Editor::update(float dt)
{
	if(!mInitialized)
		init();
	mActiveLevel->update(dt);
	if(gInput->keyPressed('O'))
	{
		if(mShadowEnabled)
			mShadowEnabled = false;
		else
			mShadowEnabled = true;
	}
	if(mToolSelected == TOOL_TERRAIN)
	{
		if(gInput->keyDown(VK_LBUTTON))
		{
			if(mCounter == 5) {
				for(int i = 0; i < mTerrainBlender.size(); i++)
				{
					mTerrainBlender[i]->beginScene();
					if(i == mTerrainSelected)
						gGraphics->drawCircle(gInput->mousePosition(), mDrawRadius, D3DCOLOR_ARGB(int(mDrawIntensity*255),255,255,255), true);
					else
						gGraphics->drawCircle(gInput->mousePosition(), mDrawRadius, D3DCOLOR_ARGB(int(mDrawIntensity*255),0,0,0), true);
					mTerrainBlender[i]->endScene();
				}
				mCounter = 0;
			}
			else 
				mCounter++;
		}
		else if(gInput->keyDown(VK_RBUTTON))
		{
			if(mCounter == 5) {
				mTerrainBlender[mTerrainSelected]->beginScene();
				gGraphics->drawCircle(gInput->mousePosition(), mDrawRadius, D3DCOLOR_ARGB(int(mDrawIntensity*255),0,0,0), true);
				mTerrainBlender[mTerrainSelected]->endScene();
				mCounter = 0;
			}
			else 
				mCounter++;
		}
	}
}

void Editor::draw()
{
	mActiveLevel->draw();
	if(mShadowEnabled)
		gGraphics->drawTexture(mArt_ShadowOn, SCREEN_WIDTH-25,SCREEN_HEIGHT-45, 30, 30);
	else
		gGraphics->drawTexture(mArt_ShadowOff, SCREEN_WIDTH-25,SCREEN_HEIGHT-45, 30, 30);
}

void Editor::drawAlpha()
{
	gGraphics->getAlphaTexture()->beginScene();
	if(mShadowEnabled)
	{
		HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0x00));
		gGameState->drawToAlpha(true);
		gLightHandler->doShadow();
		gGameState->drawToAlpha(false);
	}
	else 
	{
		HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0,255,0,0), 1.0f, 0x00));
	}
	gGraphics->getAlphaTexture()->endScene();
}