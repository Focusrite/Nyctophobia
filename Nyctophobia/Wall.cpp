#pragma once

#include "Wall.h"
#include "Graphics.h"
#include "LightHandler.h"
#include "GameState.h"

Wall::Wall(Vector start, Vector end) : Object(start.x,start.y,1,1)
{
	setEndPos(end);
	validate();
	setType(WALL);
	mThickness = 2;
	setCollidable(true);
	setCastShadow(true);
	Vector diff = start - end;
	float angle = atan2f(diff.y, diff.x);
	angle -= PI/2;
	cPolygon p(start);
	Vector dV = end-start;
	p.addPoint(Vector(0, 0));
	p.addPoint(Vector(-cosf(angle) * mThickness, -sinf(angle) * mThickness));
	p.addPoint(Vector(dV.x - cosf(angle) * mThickness, dV.y - sinf(angle) * mThickness));
	p.addPoint(Vector(dV.x, dV.y));
	setShadowBase(p);

	gLightHandler->addObstacle(this);
}

void Wall::draw() 
{
	//if(gGameState->drawingToAlpha())
	//	gGraphics->drawLine(getStartPos(), getEndPos(), 2, D3DCOLOR_XRGB(255,255,255),false);
	//else
	//{
	//	//gGraphics->drawPolygon(&getShadowBase(), D3DCOLOR_ARGB(100,255,0,0));
	//}
}

void Wall::update(float dt)
{

}

void Wall::validate()
{
	if(getEndPos().y < getStartPos().y) 
	{
		Vector t = getEndPos();
		setEndPos(getStartPos());
		setStartPos(getEndPos());
	}
}