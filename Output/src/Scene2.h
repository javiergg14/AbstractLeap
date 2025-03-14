#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>

struct SDL_Texture;

class Scene2 : public Module
{
public:

	Scene2();

	// Destructor
	virtual ~Scene2();

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

private:
	SDL_Texture* mouseTileTex = nullptr;
	std::string tilePosDebug = "[0,0]";
	bool once = false;

	SDL_Texture* img;
	SDL_Texture* helpTexture = NULL;
	bool help = false;

	//L03: TODO 3b: Declare a Player attribute
	Player* player;
	std::vector<Enemy*> enemyList;
	std::vector<Vector2D> enemyPos;
	bool checkpoint = false;

	int music;
	int respawn;

};