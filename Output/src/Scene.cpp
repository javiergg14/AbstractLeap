#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"

Scene::Scene() : Module()
{
	name = "scene";
	img = nullptr;
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	//L04: TODO 3b: Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));
	
	//L08 Create a new item using the entity manager and set the position to (200, 672) to test
	Item* item = (Item*) Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	item->position = Vector2D(200, 672);
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//L06 TODO 3: Call the function to load the map. 
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

	helpTexture = Engine::GetInstance().textures.get()->Load("Assets/Menu/help.png");
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Help button
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		help = !help;
	}

	if (help)
	{
		Engine::GetInstance().render.get()->DrawTexture(helpTexture, (player->position.getX() + 670), 0);
	}

	//L03 TODO 3: Make the camera movement independent of framerate

	Engine::GetInstance().render.get()->camera.x = -(player->position.getX() - 170);
	Engine::GetInstance().render.get()->camera.y = -(player->position.getY() - 660);

	if (Engine::GetInstance().render.get()->camera.x > 0)
	{
		Engine::GetInstance().render.get()->camera.x = 0;
	}
	if (Engine::GetInstance().render.get()->camera.y > 0)
	{
		Engine::GetInstance().render.get()->camera.y = 0;
	}
	if (Engine::GetInstance().render.get()->camera.x < -9920)
	{
		Engine::GetInstance().render.get()->camera.x = -9920;
	}
	if (Engine::GetInstance().render.get()->camera.y < -1)
	{
		Engine::GetInstance().render.get()->camera.y = 0;
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	check_Checkpoint();

	printf("\n%f", player->GetPosition().getX());

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		LoadState();

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN || checkpoint)
	{
		checkpoint = false;
		SaveState();
	}

	return ret;
}

void Scene::check_Checkpoint()
{
	if (player->GetPosition().getX() == 403)
	{
		checkpoint = true;
		printf("Checkpoint!!!!");
	}
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	SDL_DestroyTexture(img);

	return true;
}

Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

void Scene::LoadState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	//Read XML and restore information

	//Player position
	Vector2D playerPos = Vector2D(sceneNode.child("entities").child("player").attribute("x").as_int(),
		sceneNode.child("entities").child("player").attribute("y").as_int());
	player->SetPosition(playerPos);

	//enemies
	// ...

}

void Scene::SaveState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	//Save info to XML 

	//Player position
	sceneNode.child("entities").child("player").attribute("x").set_value(player->GetPosition().getX());
	sceneNode.child("entities").child("player").attribute("y").set_value(player->GetPosition().getY());

	//enemies
	// ...

	//Saves the modifications to the XML 
	loadFile.save_file("config.xml");
}