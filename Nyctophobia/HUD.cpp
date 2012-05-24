#include "HUD.h"
#include "Player.h"
#include "Graphics.h"
#include "Defines.h"
#include "d3dUtil.h"
#include "LightHandler.h"
#include "Effects.h"
#include <string>


HUD::HUD(Player *player)
{
	mFear = player->getFear();
	mTime = 0;
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\menuOutline.png", &mOutline));
}

HUD::~HUD()
{
	ReleaseCOM(mReadSurface);
	ReleaseCOM(mOutline);
}

void HUD::update(float dt)
{
	float f = gLightHandler->getPlayerIllumination()-0.6;
	if(f>0)
		f*=3;
	else
		f*=1;
	*mFear+=f*dt*40;
	mTime+=dt*(*mFear)/20;
	if(mTime > 2*PI)
		mTime = 0;
	if(*mFear>100)
		*mFear = 100;
	else if(*mFear<0)
		*mFear = 0;
}

void HUD::draw()
{
	drawFear();
	drawScreenFX();
}

void HUD::drawFear()
{
	float f = *mFear;
	gGraphics->drawRect(SCREEN_WIDTH/2,SCREEN_HEIGHT-20, SCREEN_WIDTH*(f/100), 40, D3DCOLOR_ARGB(200,150+int(30*cos(mTime)),20,20),true,false);
}

void HUD::drawScreenFX()
{
	gGraphics->drawTexture(mOutline,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT);
}