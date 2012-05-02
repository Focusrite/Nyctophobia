#pragma once

#include "Object.h"

class Wall : public Object
{
public:
	Wall(Vector start, Vector end);
	void draw();
	void update(float dt);

	void setEndPos(Vector v)	{mEnd = v; validate();}
	void setStartPos(Vector v)	{setPos(v); validate();}

	Vector getEndPos()			{return mEnd;}
	Vector getStartPos()		{return getPos();}
	void validate();
private:
	Vector mEnd;
	float mThickness;
};