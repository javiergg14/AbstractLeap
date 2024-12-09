#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "EntityManager.h"

Enemy::Enemy(EnemyType enemyType) : Entity()
{
	type = enemyType;
}

Enemy::~Enemy() {
	delete pathfinding;
}

bool Enemy::Awake() {
	return true;
}

bool Enemy::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	death = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/EnemyDeath.ogg");

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	run.LoadAnimations(parameters.child("animations").child("run"));
	currentAnimation = &idle;
	
	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->listener = this;

	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	ResetPath();
	chaseTimer.Start();

	return true;
}

bool Enemy::Update(float dt)
{
	printf("%d", type);
	if (chaseTimer.ReadMSec() >= 1000) {
		if (pathfinding->pathTiles.empty())
		{
			currentState = EnemyState::PATROL;
			currentAnimation = &idle;
		}
		else {
			currentState = EnemyState::CHASING;
			currentAnimation = &run;
		}

		ResetPath();
		chaseTimer.Start();
	}
	else {
		pathfinding->PropagateAStar(ASTAR_HEURISTICS::SQUARED);
	}

	if (currentState == EnemyState::PATROL) {
		Patrol(dt);
	}
	else if (currentState == EnemyState::CHASING) {
		Chase(dt);
	}

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	if (Engine::GetInstance().physics.get()->debug)
	{
		pathfinding->DrawPath();
	}

	return true;
}

bool Enemy::CleanUp()
{
	Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
	return true;
}

void Enemy::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Enemy::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

void Enemy::ResetPath() {
	Vector2D pos = GetPosition();
	Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
	pathfinding->ResetPath(tilePos);
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		if (Engine::GetInstance().scene.get()->GetPlayerCurrentState() == PlayerState::ATTACK)
		{
			LOG("Collided with player - DESTROY");
			Engine::GetInstance().audio.get()->PlayFx(death);
			Engine::GetInstance().entityManager.get()->DestroyEntity(this);
		}
		
		break;
	}
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		
		break;
	}
}

void Enemy::Chase(float dt)
{
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	if (!pathfinding->prePathTiles.empty()) {
		Vector2D nextTile = pathfinding->prePathTiles.front();
		Vector2D nextTileWorld = Engine::GetInstance().map.get()->MapToWorld(nextTile.getX(), nextTile.getY());


		if (nextTileWorld.getX() > position.getX()) {
			velocity.x = 0.1 * 16;
		}
		else if (nextTileWorld.getX() < position.getX()) {
			velocity.x = -0.1 * 16;
		}

		if (type == EnemyType::FLYING)
		{
			if (nextTileWorld.getY() > position.getY()) {
				velocity.y = 0.1 * 16;
			}
			else if (nextTileWorld.getY() < position.getY()) {
				velocity.y = -0.1 * 16;
			}
		}
	}

	if (velocity.y == 0) { // Verifica si está en el suelo y la condición de salto
		velocity.y = 0.1 * 16;; // Ajusta la fuerza de salto según sea necesario
	}

	pbody->body->SetLinearVelocity(velocity);
}


void Enemy::Patrol(float dt)
{
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);
	velocity.x = 0;
	pbody->body->SetLinearVelocity(velocity);
}