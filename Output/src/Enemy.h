#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Timer.h"

struct SDL_Texture;

enum class EnemyState {
	PATROL,
	CHASING
};

enum class EnemyType {
	GROUND,
	FLYING
};

class Enemy : public Entity
{
public:

	Enemy(EnemyType enemyType);
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	void ResetPath();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	void Chase(float dt);

	void Patrol(float dt);

public:
	int Death;
private:
	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation run;
	PhysBody* pbody;
	Pathfinding* pathfinding;
	EnemyState currentState = EnemyState::PATROL;
	Timer chaseTimer;
	EnemyType type;
};
