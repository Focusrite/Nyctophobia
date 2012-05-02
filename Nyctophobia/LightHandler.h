#pragma once
#include <vector>
#include "Object.h"
#include "cPolygon.h"

class BasicLight;

class LightHandler
{
public:
	LightHandler(void);
	~LightHandler(void);

	void addObstacle(Object* obj);
	void addEmitter(BasicLight* lightSource);

	void doShadow();
	void drawShadow(Object* obj, BasicLight* lightSource);

private:
	std::vector<Object*> mObstacles;
	std::vector<BasicLight*> mEmitters;

	cPolygon temp;
};

extern LightHandler* gLightHandler;