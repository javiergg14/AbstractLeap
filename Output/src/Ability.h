#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

struct SDL_Texture;

class Ability : public Entity
{
public:

	Ability();
	virtual ~Ability();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* abilityTexture;
	const char* texturePath;
	int texW, texH;

	int pickAbility;

	//L08 TODO 4: Add a physics to a Ability
	PhysBody* pbody;
};
