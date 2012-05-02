#pragma once

#include "Object.h"
#include "cPolygon.h"

class Player : public Object
{
public:
	Player(float x, float y);
	~Player();
	void draw();
	void update(float dt);
private:
	float mSpeed;
};