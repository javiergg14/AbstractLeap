#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

struct SDL_Texture;

class Diamond : public Entity
{
public:

	Diamond();
	virtual ~Diamond();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* diamondTexture;
	const char* texturePath;
	int texW, texH;

	int pickDiamond;

	//L08 TODO 4: Add a physics to a diamond
	PhysBody* pbody;
};
