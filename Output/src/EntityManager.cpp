#include "EntityManager.h"
#include "Player.h"
#include "Engine.h"
#include "ItemBoss.h"
#include "Textures.h"
#include "Scene.h"
#include "Scene2.h"
#include "Log.h"
#include "Diamond.h"
#include "Ability.h"
#include "Enemy.h"

EntityManager::EntityManager() : Module()
{
	name = "entitymanager";
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake()
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;

	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->CleanUp();
	}

	entities.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L04: TODO 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::DIAMOND:
		entity = new Diamond();
		break;
	case EntityType::ENEMY_GROUND:
		entity = new Enemy(EnemyType::GROUND);
		break;
	case EntityType::ENEMY_FLYING:
		entity = new Enemy(EnemyType::FLYING);
		break;
	case EntityType::ABILITY:
		entity = new Ability();
		break;
	case EntityType::ITEMBOSS:
		entity = new ItemBoss();
		break;
	default:
		break;
	}

	entities.push_back(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		if (*it == entity) {
			(*it)->CleanUp();
			delete* it; // Free the allocated memory
			entities.erase(it); // Remove the entity from the list
			break; // Exit the loop after removing the entity
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.push_back(entity);
}

bool EntityManager::Update(float dt)
{
	if (Engine::GetInstance().scene.get()->showStartScreen ||
		Engine::GetInstance().scene.get()->showPlayScreen ||
		Engine::GetInstance().scene.get()->showSettingsScreen ||
		Engine::GetInstance().scene.get()->showPauseScreen ||
		Engine::GetInstance().scene.get()->currentLives == 0 ||
		Engine::GetInstance().scene.get()->showCreditsScreen)
	{
		return true;
	}


	bool ret = true;
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Update(dt);
	}
	return ret;
}