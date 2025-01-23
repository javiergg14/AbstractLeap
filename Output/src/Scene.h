#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Diamond.h"
#include "GuiHUD.h"
#include <vector>
#include "GuiControlButton.h"
#include "Timer.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Return the player position
	Vector2D GetPlayerPosition();

	PlayerState GetPlayerCurrentState();

	//L15 TODO 1: Implement the Load function
	void LoadState();
	//L15 TODO 2: Implement the Save function
	void SaveState();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool showStartScreen = true;
	bool showPlayScreen = true;
	int level = 1;

	int maxDiamonds = 3;
	int currentDiamonds = 0;
	int maxLives = 3;
	int currentLives = 3;

private:
	SDL_Texture* mouseTileTex = nullptr;
	std::string tilePosDebug = "[0,0]";
	bool once = false;

	SDL_Texture* img;
	SDL_Texture* helpTexture = NULL;
	bool help = false;

	SDL_Texture* startScreenTexture;
	SDL_Texture* playScreenTexture;
	int screenDuration = 0.0f;
	Timer screenTimer;

	//L03: TODO 3b: Declare a Player attribute
	Player* player;
	std::vector<Enemy*> enemyList;
	std::vector<Diamond*> diamondList;
	std::vector<Vector2D> enemyPos;
	bool checkpoint = false;

	int music;
	int respawn;

	// L16: TODO 2: Declare a GUI Control Button 
	GuiControlButton* guiBt;
	GuiHUD* guiHUD;

};
