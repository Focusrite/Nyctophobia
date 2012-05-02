#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "Runnable.h"
#include "Math.h"
#include "Stackwalker.h"
#include <stdlib.h>
#include <crtdbg.h>
#include "PlayState.h"
#include "Menustate.h"
#include "Vector.h"
#include "Defines.h"
#include "Effects.h"
#include "Script.h"
#include "LightHandler.h"
#include "FileInput.h"


Graphics*			gGraphics		= NULL;
GameState*			gGameState		= NULL;
Input*				gInput			= NULL;
Runnable*			gGame			= NULL;
IDirect3DDevice9*	gd3dDevice		= NULL;
Math*				gMath			= NULL;
Script*				gScript			= NULL;
FileInput*			gFileInput		= NULL;
LightHandler*		gLightHandler	= NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	InitAllocCheck(ACOutput_XML); // ACOutput_XML

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif
		
	Game app(hInstance, "Nyctophobia", SCREEN_WIDTH, SCREEN_HEIGHT, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gGame = &app;

	gInput = new Input();
	gMath = new Math();
	gScript = new Script();
	gFileInput = new FileInput();
	gLightHandler = new LightHandler();

	/* this is where the gamegets runned */
	return gGame->run();
}


Game::Game(HINSTANCE hInstance, std::string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP)
	:Runnable(hInstance, caption, width, height, devType, requestedVP)
{
	gGraphics = new Graphics();
	gGraphics->getFXhandler()->updateWVP();
	
	
	mGameState = NULL;
	changeState(MenuState::Instance());
	mGameState->init(this);
	gGameState = mGameState;
	mGfxStats = new GfxStats();
}

Game::~Game()
{
	// :WARNING: Maybe needs to call ~Runnable()
	mGameState->cleanup();

	// Print memory leaks to xml file
	DeInitAllocCheck();

	delete gInput;
	delete gGraphics;
	delete gScript;
	delete gFileInput;
	delete gLightHandler;
	delete gMath;
	//delete gGameState;
	delete mGfxStats;
}

void Game::init()
{
	SetCursor(LoadCursor(0, IDC_ARROW));
}

void Game::changeState(GameState* state)
{
	if(mGameState != NULL)
		mGameState->cleanup();

	mGameState = state;
	gGameState = state;
	mGameState->init(this);
}

void Game::update(float t, float dt)
{
	mGameState->update(dt);
	mGfxStats->update(dt);
	gInput->update(dt);
}

void Game::draw()
{
	mGameState->draw();
	//gInput->draw();
	//:FPS: :NOTE:
	//mGfxStats->display();
}


void Game::drawAlpha()
{
	mGameState->drawAlpha();
}

//! The starting msgProc function.
LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	mGameState->handleEvents(msg, wParam, lParam);
	LRESULT result = gInput->msgProc(msg, wParam, lParam);
	result = Runnable::msgProc(msg, wParam, lParam);
	return result;
}