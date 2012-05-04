#include "EditorState.h"
#include "Level.h"
#include "Player.h"
#include "Wall.h"
#include "BasicLight.h"
#include "Graphics.h"
#include "LightHandler.h"
#include "Defines.h"

EditorState EditorState::mEditorState;

void EditorState::init(Game* game)
{
	mActiveLevel = new Level();
	mActiveLevel->loadLevel("Data\\Scripts\\Levels\\template.xml");
	//mActiveLevel->addObject(new Player(100, 200));
	//mActiveLevel->addObject(new Wall(Vector(430,200), Vector(430, 300)));
	//mActiveLevel->addObject(new Wall(Vector(200,200), Vector(300, 300)));
	//mActiveLevel->addObject(new BasicLight(280, 300));
	//mActiveLevel->addObject(new BasicLight(100, 200));
	//important!
	setGame(game);
}
void EditorState::cleanup(void)
{
	delete mActiveLevel;
	//Release COMS
}

void EditorState::pause()
{

}
void EditorState::resume()
{

}

void EditorState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{

}
void EditorState::update(double dt)
{
	mActiveLevel->update(dt);
}
void EditorState::draw()
{
	gGraphics->drawBlendedTexture(gGraphics->mTest,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT);
	mActiveLevel->draw();
}

void EditorState::drawAlpha()
{
	gGraphics->getAlphaTexture()->beginScene();
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0x00));
	drawToAlpha(true);
	gLightHandler->doShadow();
	drawToAlpha(false);
	gGraphics->getAlphaTexture()->endScene();
}