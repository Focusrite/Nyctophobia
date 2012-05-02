#include "PlayState.h"
#include "Level.h"
#include "Player.h"
#include "Wall.h"
#include "BasicLight.h"
#include "Graphics.h"
#include "LightHandler.h"
#include "Defines.h"

PlayState PlayState::mPlayState;

void PlayState::init(Game* game)
{
	mActiveLevel = new Level();
	mActiveLevel->loadLevel("Data\\Scripts\\Levels\\template.xml");
	mActiveLevel->addObject(new Player(100, 200));
	//mActiveLevel->addObject(new Wall(Vector(430,200), Vector(430, 300)));
	//mActiveLevel->addObject(new Wall(Vector(200,200), Vector(300, 300)));
	//mActiveLevel->addObject(new BasicLight(280, 300));
	//mActiveLevel->addObject(new BasicLight(100, 200));
	drawToAlpha(true);
	//important!
	setGame(game);
}
void PlayState::cleanup(void)
{
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
	mActiveLevel->update(dt);
}
void PlayState::draw()
{
	gGraphics->drawBlendedTexture(gGraphics->mTest,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT);
	mActiveLevel->draw();
}

void PlayState::drawAlpha()
{
	gGraphics->getAlphaTexture()->beginScene();
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0x00));
	drawToAlpha(true);
	//mActiveLevel->draw();
	gLightHandler->doShadow();
	drawToAlpha(false);
	gGraphics->getAlphaTexture()->endScene();
}