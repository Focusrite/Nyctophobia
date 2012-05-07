#pragma once

#include "Object.h"
#include "cPolygon.h"

class BasicLight : public Object
{
public:
	BasicLight(float x, float y);
	~BasicLight(){};

	void update(float dt);
	void draw();

	void setIntensity(float f)		{mIntensity = f;}
	void setRange(int range)		{mRange = range;}
private:
	float mIntensity;
	int mRange;

	float mTimer;

	//cPolygon temp;
};