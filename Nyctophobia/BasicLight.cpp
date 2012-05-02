#include "BasicLight.h"
#include "Level.h"
#include "Wall.h"
#include "Graphics.h"
#include "Input.h"
#include "Defines.h"
#include "Effects.h"
#include "LightHandler.h"


BasicLight::BasicLight(float x, float y) : Object(x,y,1,1)
{
	//temp.setPos(Vector(0,0));
	setType(LIGHT);
	setDrawToAlpha(true);
	mIntensity = 0.5f;
	mRange = 400;
	mTimer=0;
	gLightHandler->addEmitter(this);
}

void BasicLight::draw()
{
	for(int i = 1; i<=4; i++)
	{
		gGraphics->drawCircle(getPos(),i*i*(mRange/9),D3DCOLOR_ARGB((int)(100*mIntensity + 50*sinf(mTimer))/4,(int)(200*mIntensity),200,200),true,true);
	}
}

void BasicLight::update(float dt)
{
	mTimer+=dt;
	if(mTimer>=2*PI)
		mTimer=0;
	//setPos(Vector(gInput->mousePosition().x+cos(mTimer), gInput->mousePosition().y+sin(mTimer)));
}