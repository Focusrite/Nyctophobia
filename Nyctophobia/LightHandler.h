#pragma once
#include <vector>
#include "Object.h"
#include "cPolygon.h"
#include <list>
#include "d3dUtil.h"

class Player;
class BasicLight;

class LightHandler
{
public:
	LightHandler(void);
	~LightHandler(void);

	void addObstacle(Object* obj);
	void addEmitter(BasicLight* lightSource);

	void clean();

	void doShadow();
	void drawShadow(Object* obj, BasicLight* lightSource);

	float getPlayerIllumination()	{return mCurrentShade;}

private:
	std::vector<Object*> mObstacles;
	std::vector<BasicLight*> mEmitters;

	std::list<Vector> shadowPoints;
	float mCurrentShade;
	IDirect3DSurface9 *mReadSurface;
};

extern LightHandler* gLightHandler;