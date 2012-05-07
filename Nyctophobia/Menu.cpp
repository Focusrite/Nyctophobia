#include "Menu.h"
#include "Graphics.h"
#include "Defines.h"
#include "Input.h"

Menu::Menu()
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\menuOutline.png", &mOutline));
	//HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\Nyctophobia.png", &mLogo));
	mLogo = gGraphics->loadTexture("Data\\Images\\Nyctophobia.png");
	mDescription = gGraphics->loadTexture("Data\\Images\\menuDescription.png");
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\bg.png", &mBG));
	mTimer = 0;
	mStartGame = false;
}

Menu::~Menu()
{
	ReleaseCOM(mOutline);
	ReleaseCOM(mLogo);
	ReleaseCOM(mBG);
	ReleaseCOM(mDescription);
}

void Menu::draw()
{
	gGraphics->drawBlendedTexture(mBG, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT);
	gGraphics->drawTexture(mLogo,SCREEN_WIDTH/2,SCREEN_HEIGHT/4, 800,300);
	gGraphics->drawTexture(mDescription,SCREEN_WIDTH/2,3*SCREEN_HEIGHT/4+cosf(mTimer*1.5)*2, 400,200);
}

void Menu::drawAlpha()
{
	if(mTimer<=PI)
		gGraphics->drawCircle(Vector(900,100), 1000, D3DCOLOR_ARGB((int)(200*(abs(sin(mTimer)))), 200,200,200),true, true);
	if(mTimer<=PI*3 && mTimer>=PI*2)
		gGraphics->drawCircle(Vector(100,620), 1000, D3DCOLOR_ARGB((int)(150*(abs(sin(mTimer-(2*PI))))), 200,200,200),true, true);
}

void Menu::update(float dt)
{
	mTimer+=dt;
	if(mTimer>=4*PI)
		mTimer=0;
	handleInput();
}

void Menu::handleInput()
{
	if(gInput->keyDown('S'))
	{
		mStartGame = true;
		mStateSelect = PLAY;
	}
	if(gInput->keyDown('O'))
	{
		mStartGame = true;
		mStateSelect = EDITOR;
	}
}