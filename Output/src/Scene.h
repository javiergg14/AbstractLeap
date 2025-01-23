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

	void CreateEntities();

	SDL_Rect CalculateButtonBounds(int y, const std::string& text);
	void ChangeLevel();

	void CleanEntities();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool showStartScreen = true;
	bool showPlayScreen = true;
	bool showCreditsScreen = false;
	bool showPauseScreen = false;
	bool showSettingsScreen = false;
	int level = 1;

	int maxDiamonds = 3;
	int currentDiamonds = 0;
	int maxLives = 3;
	int currentLives = 3;

	bool changeLevel = false;

	std::vector<Diamond*> diamondList;
	std::vector<Enemy*> enemyList;

	bool finalBoss = false;

	int bossLive = 3;

private:
	SDL_Texture* mouseTileTex = nullptr;
	std::string tilePosDebug = "[0,0]";
	bool once = false;

	SDL_Texture* img;
	SDL_Texture* helpTexture = NULL;
	bool help = false;

	SDL_Texture* startScreenTexture;
	SDL_Texture* playScreenTexture;
	SDL_Texture* pauseScreenTexture;
	SDL_Texture* creditsScreenTexture;
	SDL_Texture* settingsScreenTexture;
	SDL_Texture* gameOverScreen;
	SDL_Texture* winnerScreen;
	int screenDuration = 0.0f;
	Timer screenTimer;
	Timer gameOverTimer;
	int gameOverDuration = 3.0f;

	//L03: TODO 3b: Declare a Player attribute
	Player* player;
	std::vector<Vector2D> enemyPos;
	bool checkpoint = false;

	int music;
	int respawn;

	bool isExitPressed = false;

	// L16: TODO 2: Declare a GUI Control Button 
	
	GuiHUD* guiHUD;
	float changeMap = 11300.0f;

};
