#pragma once

#include "d3dUtil.h"

class Menu
{
public:
	Menu();
	~Menu();
	
	void update(float dt);
	void draw();
	void drawAlpha();
	void handleInput();

	bool startGame()	{return mStartGame;}

private:
	float mTimer;
	bool mStartGame;

	//Art assets
	IDirect3DTexture9* mOutline;
	IDirect3DTexture9* mLogo;
	IDirect3DTexture9* mBG;
};