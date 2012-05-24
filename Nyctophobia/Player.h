#pragma once

#include "Object.h"
#include "cPolygon.h"
#include "d3dUtil.h"

class Player : public Object
{
public:
	Player(float x, float y);
	~Player();
	void draw();
	void update(float dt);

	float* getFear()		{return &mFear;}
private:
	float mSpeed;
	float mFear;
	float mRotation;
	IDirect3DTexture9* mTexture;
};