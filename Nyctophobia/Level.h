#pragma once

#include <vector>

class Object;

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

	std::vector<Object*> getObjectList(){return mObjectList;}
private:
	static int mIdCounter;
	std::vector<Object*> mObjectList;
};