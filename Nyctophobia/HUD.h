#pragma once
#include "d3dUtil.h"

class Player;

class HUD
{
public:
	HUD(Player *player);
	~HUD();

	void draw();
	void drawFear();
	void drawScreenFX();
	
	void update(float dt);
private:
	float *mFear;
	IDirect3DSurface9 *mReadSurface;
	IDirect3DTexture9 *mOutline;
	float mTime;
};