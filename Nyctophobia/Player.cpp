#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "LightHandler.h"
#include "Gamestate.h"

Player::Player(float x, float y) : Object(x,y,10,10)
{
	setType(PLAYER);
	mSpeed=200;
	setCastShadow(true);
	cPolygon c;
	c.addPoint(+5,-5);
	c.addPoint(-5,-5);
	c.addPoint(-5,+5);
	c.addPoint(+5,+5);
	c.setPos(Vector(x,y));
	setShadowBase(c);
	gLightHandler->addObstacle(this);
}

Player::~Player()
{
	
}

void Player::draw()
{
	if(gGameState->drawingToAlpha())
		gGraphics->drawRect(getPos().x,getPos().y,10,10, D3DCOLOR_XRGB(200,200,200), true, false);
	else
		gGraphics->drawRect(getPos().x,getPos().y,10,10, D3DCOLOR_XRGB(200,200,200));
}

void Player::update(float dt)
{
	if(gInput->keyDown('A'))
		move(Vector(-mSpeed*dt,0));
	if(gInput->keyDown('D'))
		move(Vector(mSpeed*dt,0));
	if(gInput->keyDown('W'))
		move(Vector(0,-mSpeed*dt));
	if(gInput->keyDown('S'))
		move(Vector(0,mSpeed*dt));
}