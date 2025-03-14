#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

enum class PlayerState {
	PASIVE,
	ATTACK
};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L08 TODO 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	PlayerState GetCurrentState();

public:

	//Declare player parameters
	float speed = 0.25f;
	SDL_Texture* texture = NULL;
	int texW, texH;

	//God Mode
	bool godMode;

	// Audio fx
	int pickCoinSound;
	int jumpSound;
	int checkPointSound;
	int deathSound;
	int habilityDesactivated;
	int habilityActivated;

	// Death
	bool isDead = false;
	bool checkpoint = false;
	// L08 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	float jumpForce = 3.8f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping
	int jumpCount = 0;
	bool doubleJump = false;
	bool hability = false;
	int habilityDuration = 30.0f;
	Timer habilityTimer;
	PlayerState currentState = PlayerState::PASIVE;

	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation run_right;
	Animation run_left;
	Animation jump;
	Animation fall;
	Animation duck;
	Animation die;

	SDL_Texture* doubleJumpIcon;
};