#pragma once

#include <vector>
#include <string>
#include "Vector.h"
#include "Collision.h"

class Object;
class Terrain;
class Player;

class Level
{
public:
	Level();
	~Level();

	void update(float dt);
	void draw();

	void clear();

	void addObject(Object*);
	void removeObject(Object*);
	void moveObjects(Vector dV);

	void loadLevel(const char* path);

	std::vector<Object*> getObjectList()	{return mObjectList;}
	std::vector<Terrain*> getTerrainList()	{return mTerrainList;}
	Player* getPlayer()						{return mPlayer;}
	bool isInEditor()						{return mInEditor;}

	void setInEditor(bool b)				{mInEditor = b;}
private:
	static int mIdCounter;
	std::string mName;
	int mWidth;
	int mHeight;
	bool mInEditor;
	Player *mPlayer;
	Vector mLevelDisplacement;
	MTV mtv;

	std::vector<Object*> mObjectList;
	std::vector<Terrain*> mTerrainList;
};