#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Scene2.h"
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
	patrolTimer.Start();

	return true;
}

bool Enemy::Update(float dt)
{
	if (chaseTimer.ReadMSec() >= 1000) {
		if (pathfinding->pathTiles.empty())
		{
			currentState = EnemyState::PATROL;
			currentAnimation = &idle;
			pathfinding->prePathTiles.clear();
		}
		else {
			currentState = EnemyState::CHASING;
			currentAnimation = &run;
		}

		ResetPath();
		chaseTimer.Start();
	}
	else {
		if (type == EnemyType::FLYING)
		{
			pathfinding->PropagateAStar(ASTAR_HEURISTICS::SQUARED);
		}
		else if (type == EnemyType::GROUND)
		{
			pathfinding->PropagateAStar(ASTAR_HEURISTICS::MANHATTAN);
		}
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
	if (type == EnemyType::GROUND)
	{
		Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY() - 5, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
	else if (type == EnemyType::FLYING)
	{
		Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX() - 16, (int)position.getY() - 10, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}



	if (Engine::GetInstance().physics.get()->debug)
	{
		pathfinding->DrawPath();
	}

	return true;
}

bool Enemy::CleanUp() {
	if (pbody) {
		Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
		pbody = nullptr;  // Evita punteros colgantes
	}
	isDead = true;
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
		if (!Engine::GetInstance().scene.get()->finalBoss)
		{
			if (Engine::GetInstance().scene.get()->GetPlayerCurrentState() == PlayerState::ATTACK)
			{
				LOG("Collided with player - DESTROY");

				// Encuentra y elimina el enemigo de enemyList
				auto& enemyList = Engine::GetInstance().scene.get()->enemyList;
				auto it = std::find(enemyList.begin(), enemyList.end(), this);
				if (it != enemyList.end()) {
					enemyList.erase(it);  // Elimina el puntero de la lista
				}
				else {
					LOG("Enemy not found in enemyList: %p", this);
				}

				// Reproduce el efecto de muerte
				Engine::GetInstance().audio.get()->PlayFx(death);

				// Destruye el enemigo
				Engine::GetInstance().entityManager.get()->DestroyEntity(this);
			}
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
	constexpr float chaseSpeed = 0.2f * 16;  // Velocidad base
	constexpr float smoothFactor = 0.1f;     // Factor de suavizado normal
	constexpr float initialBoostFactor = 0.5f;  // Factor de suavizado inicial para arrancar

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();
	b2Vec2 targetVelocity(0, currentVelocity.y);

	if (!pathfinding->prePathTiles.empty()) {
		Vector2D nextTile = pathfinding->prePathTiles.front();
		Vector2D nextTileWorld = Engine::GetInstance().map.get()->MapToWorld(nextTile.getX(), nextTile.getY());

		// Direcciones objetivo para el movimiento
		targetVelocity.x = (nextTileWorld.getX() > position.getX()) ? chaseSpeed :
			(nextTileWorld.getX() < position.getX() ? -chaseSpeed : 0);

		if (type == EnemyType::FLYING) {
			targetVelocity.y = (nextTileWorld.getY() > position.getY()) ? chaseSpeed :
				(nextTileWorld.getY() < position.getY() ? -chaseSpeed : 0);
		}
	}

	// Si la velocidad inicial es 0, aplicar un impulso inicial más rápido
	float actualSmoothFactor = (currentVelocity.LengthSquared() == 0) ? initialBoostFactor : smoothFactor;

	// Interpolación para suavizar el cambio de velocidad
	currentVelocity.x += actualSmoothFactor * (targetVelocity.x - currentVelocity.x);
	currentVelocity.y += actualSmoothFactor * (targetVelocity.y - currentVelocity.y);

	pbody->body->SetLinearVelocity(currentVelocity);
}




void Enemy::Patrol(float dt)
{
	static float patrolSpeed = 1.2f;  // Velocidad de movimiento
	static int state = 0;  // 0: derecha, 1: quieto, 2: izquierda, 3: quieto
	static const int stateDuration = 1000;  // Duración de cada estado en milisegundos

	b2Vec2 velocity = pbody->body->GetLinearVelocity();

	if (patrolTimer.ReadMSec() >= stateDuration)
	{
		state = (state + 1) % 4;  // Cambiar al siguiente estado en un ciclo de 4
		patrolTimer.Start();  // Reiniciar el temporizador
	}

	// Asignar velocidad según el estado
	switch (state)
	{
	case 0:  // Mover a la derecha
		velocity.x = patrolSpeed;
		currentAnimation = &run;
		break;
	case 1:  // Quedarse quieto
		velocity.x = 0;
		currentAnimation = &idle;
		break;
	case 2:  // Mover a la izquierda
		velocity.x = -patrolSpeed;
		currentAnimation = &run;
		break;
	case 3:  // Quedarse quieto
		velocity.x = 0;
		currentAnimation = &idle;
		break;
	}
	
	velocity.x = 0;
	velocity.y = 0;

	pbody->body->SetLinearVelocity(velocity);
}