#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "LightHandler.h"
#include "Gamestate.h"
#include "Level.h"

Player::Player(float x, float y) : Object(x,y,10,10)
{
	mRotation = 0;
	setType(PLAYER);
	mSpeed=200;
	mFear = 0;
	setCastShadow(true);
	setCollidable(true);
	cPolygon c(Vector(x,y));
	c.addPoint(+5,-5);
	c.addPoint(-5,-5);
	c.addPoint(-5,+5);
	c.addPoint(+5,+5);
	setShadowBase(c);
	gLightHandler->addObstacle(this);
	mTexture = gGraphics->loadTexture("Data\\Images\\player.png");
}

Player::~Player()
{
	ReleaseCOM(mTexture);
}

void Player::draw()
{
	if(gGameState->drawingToAlpha())
	{
		//gGraphics->drawRect(getPos().x,getPos().y,15,15, D3DCOLOR_XRGB(255,255,255), true, false);
		gGraphics->drawTexture(mTexture, getPos().x,getPos().y,40,40,mRotation);
	}
	else
	{
		//gGraphics->drawRect(getPos().x,getPos().y,10,10, D3DCOLOR_XRGB(200,200,200));
		gGraphics->drawBlendedTexture(mTexture, getPos().x,getPos().y,40,40,mRotation);
	}
}

void Player::update(float dt)
{
	if(gInput->keyDown('A'))
	{
		getLevel()->moveObjects(Vector(mSpeed*dt,0));
		mRotation = PI;
	}
	if(gInput->keyDown('D'))
	{
		getLevel()->moveObjects(Vector(-mSpeed*dt,0));
		mRotation = 0;
	}
	if(gInput->keyDown('W'))
	{
		getLevel()->moveObjects(Vector(0,mSpeed*dt));
		mRotation = -PI/2;
	}
	if(gInput->keyDown('S'))
	{
		getLevel()->moveObjects(Vector(0,-mSpeed*dt));
		mRotation = PI/2;
	}
}