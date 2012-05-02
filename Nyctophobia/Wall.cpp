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
	
	setCastShadow(true);
	Vector diff = start - end;
	float angle = atan2f(diff.y, diff.x);
	angle -= PI/2;
	cPolygon p;
	p.addPoint(Vector(start.x, start.y));
	p.addPoint(Vector(start.x - cosf(angle) * mThickness, start.y - sinf(angle) * mThickness));
	p.addPoint(Vector(end.x - cosf(angle) * mThickness, end.y - sinf(angle) * mThickness));
	p.addPoint(Vector(end.x, end.y));
	setShadowBase(p);

	gLightHandler->addObstacle(this);
}

void Wall::draw() 
{
	if(gGameState->drawingToAlpha())
		gGraphics->drawLine(getStartPos(), getEndPos(), 2, D3DCOLOR_XRGB(255,255,255),false);
	
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