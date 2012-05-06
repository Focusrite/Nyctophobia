#pragma once
#include <vector>
#include "DrawableTex2D.h"
#include "d3dUtil.h"

class Terrain;
class Level;

enum TOOL {
	TOOL_TERRAIN = 0, TOOL_WALL, TOOL_LIGHT, TOOL_REGION
};

class Editor
{
public:
	Editor();
	~Editor();
	void update(float dt);
	void draw();
	void drawAlpha();
	void handleInput();
	void init();

	void addTerrainLayer(Terrain* baseTerrain);
	void setActiveLevel(Level* level)	{mActiveLevel = level;}
private:
	std::vector<DrawableTex2D*> mTerrainBlender;

	Level* mActiveLevel;
	//Editor flags
	bool	mShadowEnabled;
	TOOL	mToolSelected;
	int		mTerrainSelected;
	float	mDrawRadius;
	float	mDrawIntensity;

	//Art assets
	IDirect3DTexture9* mArt_ShadowOn;
	IDirect3DTexture9* mArt_ShadowOff;

	//Helpers
	int mCounter;
	bool mInitialized;
};