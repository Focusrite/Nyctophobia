#include "EditorState.h"
#include "Level.h"
#include "Player.h"
#include "Wall.h"
#include "BasicLight.h"
#include "Graphics.h"
#include "LightHandler.h"
#include "Editor.h"
#include "Defines.h"

EditorState EditorState::mEditorState;

void EditorState::init(Game* game)
{
	mActiveLevel = new Level();
	mActiveLevel->loadLevel("Data\\Scripts\\Levels\\template.xml");
	mEditor = new Editor();
	mEditor->setActiveLevel(mActiveLevel);
	//important!
	setGame(game);
}
void EditorState::cleanup(void)
{
	gLightHandler->clean();
	delete mActiveLevel;
	delete mEditor;
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
	mEditor->update(dt);
}
void EditorState::draw()
{
	mEditor->draw();
}

void EditorState::drawAlpha()
{
	mEditor->drawAlpha();
}