#pragma once

#include "d3dUtil.h"

enum SWITCH_STATE 
{
	PLAY = 0, LOAD, EDITOR
};

class Menu
{
public:
	Menu();
	~Menu();
	
	void update(float dt);
	void draw();
	void drawAlpha();
	void handleInput();

	bool startGame()				{return mStartGame;}
	SWITCH_STATE getSelectedState()	{return mStateSelect;}
private:
	float mTimer;
	bool mStartGame;
	SWITCH_STATE mStateSelect;

	//Art assets
	IDirect3DTexture9* mOutline;
	IDirect3DTexture9* mLogo;
	IDirect3DTexture9* mBG;
	IDirect3DTexture9* mDescription;
};