#include "Menustate.h"
#include "Playstate.h"
#include "Graphics.h"
#include "Defines.h"

MenuState MenuState::mMenuState;

void MenuState::init(Game* game)
{
	//important!
	mMenu = new Menu();
	setGame(game);
	mMenuCountdown = PI/2;
}
void MenuState::cleanup(void)
{
	//Release COMS
	delete mMenu;
}

void MenuState::pause()
{

}
void MenuState::resume()
{

}

void MenuState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{

}
void MenuState::update(double dt)
{
	mTimer+=dt;
	if(mTimer>=2*PI)
		mTimer=0;
	mMenu->update(dt);

	//Check for Gamestate change
	if(mMenu->startGame()) 
	{
		mMenuCountdown -= dt;
		if(mMenuCountdown <= EPSILON)
			changeState(PlayState::Instance());
	}
}
void MenuState::draw()
{
	mMenu->draw();
	if(mMenu->startGame())
		gGraphics->drawRect(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT, D3DCOLOR_ARGB((int)(255* abs(cos(mMenuCountdown))), 0,0,0), true, false, 0.2);
}

void MenuState::drawAlpha()
{
	gGraphics->getAlphaTexture()->beginScene();
	int i = (int)(25*(sin(mTimer)+1)/8)+125;
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(i,i,i), 1.0f, 0));
	mMenu->drawAlpha();
	gGraphics->getAlphaTexture()->endScene();
}