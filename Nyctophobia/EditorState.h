#pragma once

#include "GameState.h"

class Level;
class Editor;

class EditorState : public GameState 
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
	static EditorState* Instance() {
		return &mEditorState;
	}
protected:
	EditorState() {};
private:
	static EditorState mEditorState;
	Level* mActiveLevel;

	Editor* mEditor;
};