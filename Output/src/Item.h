#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* greenGem;
	SDL_Texture* greenCrystal;
	SDL_Texture* keyGreen;
	const char* texturePath;
	int texW, texH;

	int pickItem;

	//L08 TODO 4: Add a physics to an item
	PhysBody* pbody;
};
