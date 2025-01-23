#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

struct SDL_Texture;

class ItemBoss : public Entity
{
public:

	ItemBoss();
	virtual ~ItemBoss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* itemBossTexture;
	const char* texturePath;
	int texW, texH;

	int pickItemBoss;

	//L08 TODO 4: Add a physics to a ItemBoss
	PhysBody* pbody;
};
