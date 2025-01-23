#pragma once

#include "Vector2D.h"
#include "GuiControl.h"

struct SDL_Texture;

class GuiHUD : public GuiControl
{

public:

	GuiHUD(int id, SDL_Rect bounds, const char* text);
	virtual ~GuiHUD();

	// Called each loop iteration
	bool Update(float dt);

	void DrawLives(int maxLives, int currentLives);

	void DrawDiamonds(int maxDiamonds, int currentDiamonds);

private:
	SDL_Texture* diamondTexture;
	SDL_Texture* diamondOutlineTexture;
	SDL_Texture* lifeTexture;
	SDL_Texture* lifeOutlineTexture;

	bool canClick = true;
	bool drawBasic = false;

};