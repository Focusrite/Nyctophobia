#pragma once

#include "GameState.h"
#include "Menu.h"


class MenuState : public GameState 
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
	static MenuState* Instance() {
		return &mMenuState;
	}
protected:
	MenuState() {};
private:
	static MenuState mMenuState;
	float mTimer;
	float mMenuCountdown;

	Menu *mMenu;
};