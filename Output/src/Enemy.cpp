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

Enemy::Enemy() : Entity(EntityType::ENEMY)
{

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

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
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

	return true;
}

bool Enemy::Update(float dt)
{
	// Obtener la posición del jugador
	Vector2D playerPos = Engine::GetInstance().scene.get()->GetPlayerPosition();

	// Verificar la proximidad del jugador y cambiar de estado
	CheckPlayerProximity(playerPos);

	// Lógica para el estado de patrullaje o persecución
	if (currentState == EnemyState::PATROL) {
		Patrol(dt);
	}
	else if (currentState == EnemyState::CHASING) {
		Chase(dt);
	}

	// Actualización de la posición y renderizado
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	// Dibujar la textura del enemigo
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	// Dibujar el camino para depuración
	pathfinding->DrawPath();

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
		LOG("Collided with player - DESTROY");
		Engine::GetInstance().entityManager.get()->DestroyEntity(this);
		break;
	}
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision player");
		break;
	}
}

void Enemy::Chase(float dt)
{
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	counter++;

	if (counter == 15) {
		ResetPath();
		counter = 0;
	}
	else {
		pathfinding->PropagateAStar(ASTAR_HEURISTICS::MANHATTAN);
	}

	if (!pathfinding->prePathTiles.empty()) {
		Vector2D nextTile = pathfinding->prePathTiles.front();  // Obtener el siguiente tile del camino
		Vector2D nextTileWorld = Engine::GetInstance().map.get()->MapToWorld(nextTile.getX(), nextTile.getY());

		// Mover el enemigo hacia el siguiente tile (podrías agregar interpolación para suavizar el movimiento)
		if (nextTileWorld.getX() > position.getX()) {
			velocity.x = 0.1 * 16;
		}
		else if (nextTileWorld.getX() < position.getX()) {
			velocity.x = -0.1 * 16;
		}

		if (nextTileWorld.getY() > position.getY()) {
			velocity.y = 0.1 * 16;
		}
		else if (nextTileWorld.getY() < position.getY()) {
			velocity.y = -0.1 * 16;
		}
	}

	pbody->body->SetLinearVelocity(velocity);

}

void Enemy::Patrol(float dt)
{
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	// Movimiento de patrullaje de izquierda a derecha (como un patrón cíclico)
	static bool movingRight = true;  // Determina si el enemigo se mueve a la derecha o a la izquierda
	float patrolSpeed = 0.1 * 16;    // Velocidad del patrullaje

	// Movimiento a la derecha
	if (movingRight) {
		velocity.x = patrolSpeed;
	}
	// Movimiento a la izquierda
	else {
		velocity.x = -patrolSpeed;
	}

	// Cambiar dirección al llegar a los límites (puedes ajustar los límites según el mapa)
	if (position.getX() >= 500) {  // Límite derecho (ajústalo a tu mapa)
		movingRight = false;
	}
	else if (position.getX() <= 50) {  // Límite izquierdo (ajústalo a tu mapa)
		movingRight = true;
	}

	// Movimiento vertical opcional (puedes agregarlo si lo necesitas)
	velocity.y = 0;

	pbody->body->SetLinearVelocity(velocity);

}

void Enemy::CheckPlayerProximity(Vector2D playerPos) {
	Vector2D enemyPos = GetPosition();

	float dx = playerPos.getX() - enemyPos.getX();
	float dy = playerPos.getY() - enemyPos.getY();
	float distance = sqrt(dx * dx + dy * dy);

	if (distance < 200) {
		currentState = EnemyState::CHASING;
	}
	else {
		currentState = EnemyState::PATROL; // Si el jugador se aleja, vuelve al patrullaje
	}
}