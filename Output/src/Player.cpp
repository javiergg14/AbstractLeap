#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Scene2.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(0 ,0);
	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(170);
	position.setY(50);
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	run_right.LoadAnimations(parameters.child("animations").child("run_right"));
	run_left.LoadAnimations(parameters.child("animations").child("run_left"));
	jump.LoadAnimations(parameters.child("animations").child("jump"));
	fall.LoadAnimations(parameters.child("animations").child("fall"));
	duck.LoadAnimations(parameters.child("animations").child("duck"));
	die.LoadAnimations(parameters.child("animations").child("die"));
	currentAnimation = &idle;

	// Initilize sfx
	pickCoinSound = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/coin.ogg");
	jumpSound = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/jump.ogg");
	deathSound = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/death.ogg");
	checkPointSound = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/checkpoint.ogg");
	habilityDesactivated = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/habilityDesactivated.ogg");
	habilityActivated = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/habilityActivated.ogg");

	doubleJumpIcon = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/blueJewel.png");

	// L08 TODO 5: Add physics to the player - initialize physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);
	/*pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), texW,texH, bodyType::DYNAMIC);*/

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	return true;
}

bool Player::Update(float dt)
{
	if (Engine::GetInstance().scene.get()->showStartScreen)
	{
		return true;
	}

	if (Engine::GetInstance().scene.get()->showPlayScreen)
	{
		return true;
	}
	
	currentAnimation = &idle;

	// L08 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	// God mode
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	// Duck
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		currentAnimation = &duck;
	}

	// Move left
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x -= speed * 16 ;
		currentAnimation = &run_left;
	}

	// Move right
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speed * 16;
		currentAnimation = &run_right;
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		hability = true;
		Engine::GetInstance().audio.get()->PlayFx(habilityActivated);
		habilityTimer.Start();
	}

	// Jump
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false || Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && doubleJump == true) {
		// Apply an initial upward force
		Engine::GetInstance().audio.get()->PlayFx(jumpSound);
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
		jumpCount += 1;
	}

	if (hability == true)
	{
		Engine::GetInstance().render.get()->DrawTexture(doubleJumpIcon, position.getX()+10, position.getY()-30);

		if (jumpCount == 0)
		{
			doubleJump = true;
		}

		if (jumpCount == 2)
		{
			doubleJump = false;
		}

		if (hability && habilityTimer.ReadSec() >= habilityDuration)
		{
			hability = false; // Desactivar la habilidad despu�s de 30 segundos
			jumpCount = 2;
			Engine::GetInstance().audio.get()->PlayFx(habilityDesactivated);
		}

	}

	// If the player is jumpling, we don't want to apply gravity, we use the current velocity prduced by the jump
	if(isJumping == true)
	{
		currentAnimation = &jump;
		velocity.y = pbody->body->GetLinearVelocity().y;
	}

	if (velocity.y > 0.5f) {
		currentAnimation = &fall;
		currentState = PlayerState::ATTACK;
		doubleJump = false;
	}
	else {
		currentState = PlayerState::PASIVE;
	}

	// Apply the velocity to the player
	// God mode
	if (godMode) {
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			velocity.y = -6;
		}
		else if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			velocity.y = 6;
		}
		else
		{
			velocity.y = 0;
		}
	}


	if (isDead)
	{
		currentAnimation = &die;

		pbody->body->SetLinearVelocity({0 , 0});

		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_W) == KEY_DOWN ||
			Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
			Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_DOWN ||
			Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_DOWN ||
			Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			Engine::GetInstance().scene.get()->LoadState();
			isDead = false;
		}
	}
	else {
		pbody->body->SetLinearVelocity(velocity);
	}

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		//reset the jump flag when touching the ground
		jump.Reset();
		isJumping = false;
		if (hability)
		{
			jumpCount = 0;
			doubleJump = true;
		}

		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::NEWLVL:
		break;
	case ColliderType::DEATH:
		Engine::GetInstance().audio.get()->PlayFx(deathSound);
		if (!godMode)
		{
			isDead = true;
		}
		break;
	case ColliderType::ENEMY:
		if (!godMode && currentState == PlayerState::PASIVE)
		{
			Engine::GetInstance().audio.get()->PlayFx(deathSound);
			isDead = true;
		}
		break;

	case ColliderType::CHECKPOINT:
		checkpoint = true;
		break;

	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		break;
	case ColliderType::DIAMOND:
		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::DEATH:
		break;
	case ColliderType::CHECKPOINT:
		Engine::GetInstance().audio.get()->PlayFx(checkPointSound);
		break;
	case ColliderType::NEWLVL:
		break;
	default:
		break;
	}
}

void Player::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Player::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

PlayerState Player::GetCurrentState() {
	return currentState;
}