#pragma once

typedef struct sKeyState {
	bool bPressed;
	bool bReleased;
	bool bHeld;
}sKeyState;

void KeyUpdate();
sKeyState GetKey(int nKeyID);
sKeyState GetMouse(int nMouseButtonID);
void ssetXY(int x, int y);
int getdx();
int getdy();