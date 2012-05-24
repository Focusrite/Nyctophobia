#pragma once

#include "Vector.h"
#include "cPolygon.h"

class Level;

enum ObjectType {WALL = 0, PLAYER, LIGHT};

class Object 
{
public:
	Object(float x, float y, float w, float h);
	virtual void update(float dt) = 0;
	virtual void draw() = 0;

	Vector getPos()						{return mPos;}
	int getId()							{return mId;}
	Level* getLevel()					{return mLevel;}
	ObjectType getType()				{return mObjectType;}
	bool isDead()						{return mDelete;}
	bool isCollidable()					{return mCollidable;}
	bool getDrawToAlpha()				{return mDrawToAlpha;}
	bool getCastsShadow()				{return mCastsShadow;}
	cPolygon getShadowBase()			{return mShadowBase;}
	
	void setPos(Vector v)				{mPos = v; mShadowBase.setPos(mPos);}
	void setId(int i)					{mId = i;}
	void setLevel(Level* l)				{mLevel = l;}
	void setType(ObjectType o)			{mObjectType = o;}
	void setDrawToAlpha(bool b)			{mDrawToAlpha = b;}
	void setCastShadow(bool b)			{mCastsShadow = b;}
	void setCollidable(bool b)			{mCollidable = b;}
	void setShadowBase(cPolygon base)	{mShadowBase = base;}

	void kill()							{mDelete=true;}
	void move(Vector dV);
private:
	int mId;
	Vector mPos;
	Level* mLevel;
	bool mDelete;
	ObjectType mObjectType;

	//Shadow/Collision data
	bool mDrawToAlpha;
	bool mCastsShadow;
	bool mCollidable;
	cPolygon mShadowBase;
};

