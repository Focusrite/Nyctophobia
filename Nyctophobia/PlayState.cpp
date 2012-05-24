#include "PlayState.h"
#include "Level.h"
#include "Graphics.h"
#include "LightHandler.h"
#include "Defines.h"
#include "Input.h"
#include "MenuState.h"
#include "HUD.h"

PlayState PlayState::mPlayState;

void PlayState::init(Game* game)
{
	mActiveLevel = new Level();
	mActiveLevel->loadLevel("Data\\Scripts\\Levels\\level01.xml");
	mHUD = new HUD(mActiveLevel->getPlayer());
	//important!
	setGame(game);
}
void PlayState::cleanup(void)
{
	gLightHandler->clean();
	delete mActiveLevel;
	//Release COMS
}

void PlayState::pause()
{

}
void PlayState::resume()
{

}

void PlayState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{

}
void PlayState::update(double dt)
{
	if(gInput->keyPressed(VK_ESCAPE))
		changeState(MenuState::Instance());
	else
	{
		mActiveLevel->update(dt);
		mHUD->update(dt);
	}	
}
void PlayState::draw()
{
	mActiveLevel->draw();
	mHUD->draw();
}

void PlayState::drawAlpha()
{
	gGraphics->getAlphaTexture()->beginScene();
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255,35,0,0), 1.0f, 0x00));
	drawToAlpha(true);
	gLightHandler->doShadow();
	drawToAlpha(false);
	gGraphics->getAlphaTexture()->endScene();
}