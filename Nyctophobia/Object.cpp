#include "Object.h"

Object::Object(float x, float y, float w, float h)
{
	setPos(Vector(x,y));
	mDelete = false;
	mDrawToAlpha = false;
	setCastShadow(false);
	mCollidable = false;
}

void Object::move(Vector dV)
{
	mPos += dV;
	mShadowBase.setPos(mPos);
}