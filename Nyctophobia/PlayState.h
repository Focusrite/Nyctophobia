#pragma once

#include "GameState.h"

class Level;

class PlayState : public GameState 
{
public:
	void init(Game* game);
	void cleanup(void);

	void pause();
	void resume();

	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void update(double dt);
	void draw();
	void drawAlpha();
	static PlayState* Instance() {
		return &mPlayState;
	}
protected:
	PlayState() {};
private:
	static PlayState mPlayState;
	Level* mActiveLevel;
};