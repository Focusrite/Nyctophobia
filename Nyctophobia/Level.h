#pragma once

#include <vector>
#include <string>

class Object;
class Terrain;

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

	void loadLevel(const char* path);

	std::vector<Object*> getObjectList()	{return mObjectList;}
	std::vector<Terrain*> getTerrainList()	{return mTerrainList;}
	bool isInEditor()						{return mInEditor;}

	void setInEditor(bool b)				{mInEditor = b;}
private:
	static int mIdCounter;
	std::string mName;
	int mWidth;
	int mHeight;
	bool mInEditor;

	std::vector<Object*> mObjectList;
	std::vector<Terrain*> mTerrainList;
};