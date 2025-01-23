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
#include "Diamond.h"
#include "Enemy.h"
#include "GuiControl.h"
#include "GuiManager.h"

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
	
	////L08 Create a new item using the entity manager and set the position to (200, 672) to test
	for (int i = 0; i < 3; ++i) {
		Diamond* diamond = (Diamond)Engine::GetInstance().entityManager->CreateEntity(EntityType::DIAMOND);
		diamond->position = Vector2D(200 + i 100, 500);
		diamondList.push_back(diamond);
	}


	// Create a enemy using the entity manager 
	for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		Enemy* enemy = nullptr;

		if (enemyNode.attribute("type").as_string() == std::string("ground"))
		{
			enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY_GROUND);
		}
		else if (enemyNode.attribute("type").as_string() == std::string("flying"))
		{
			enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY_FLYING);
		}

		if (enemy) {  // Valida que el enemigo fue creado
			enemy->SetParameters(enemyNode);
			enemyList.push_back(enemy);
		}

		if (finalBoss)
		{

		}
		else {
			LOG("Failed to create enemy for node: %s", enemyNode.name());
		}
	}

	// L16: TODO 2: Instantiate a new GuiControlButton in the Scene
	guiBt = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", { 520, 350, 120,20 }, this);
	guiHUD = (GuiHUD*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::HUD, 1, "HUD", { 520, 350, 120,20 }, this);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	startScreenTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/startScreen.png");

	playScreenTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/playScreen.png");


	//L06 TODO 3: Call the function to load the map. 
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());
	Engine::GetInstance().audio.get()->SetMusicVolume(0.1f);
	Engine::GetInstance().audio.get()->PlayMusic("Assets/Audio/Music/music.ogg"); 
	respawn = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/Respawn.ogg");
	helpTexture = Engine::GetInstance().textures.get()->Load("Assets/Menu/help.png");
	screenTimer.Start();
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

	if (showStartScreen)
		{
			Engine::GetInstance().render.get()->DrawTexture(startScreenTexture, 0, 0);

			// Detectar si el jugador presiona la tecla Intro
			if (screenTimer.ReadSec() >= screenDuration)
			{
				showStartScreen = false; // Ocultar la pantalla inicial
			}

			return true; // No continuar con el resto del juego mientras se muestra la pantalla inicial
	}

	if (showPlayScreen)
	{
		Engine::GetInstance().render.get()->DrawTexture(playScreenTexture, 0, 0);

			// Detectar si el jugador presiona la tecla Intro
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			showPlayScreen = false; // Ocultar la pantalla inicial
		}

		return true; // No continuar con el resto del juego mientras se muestra la pantalla inicial
	}

	
	// Help button
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		help = !help;
	}

	if (help)
	{
		Engine::GetInstance().render.get()->DrawTexture(helpTexture, (player->position.getX() + 340), 0);
	}


	//L03 TODO 3: Make the camera movement independent of framerate
	Engine::GetInstance().render.get()->camera.x = -(player->position.getX() - 500);
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
	if (Engine::GetInstance().render.get()->camera.y < 0)
	{
		Engine::GetInstance().render.get()->camera.y = 0;
	}
	// L10 TODO 6: Implement a method that repositions the player in the map with a mouse click

	//Get mouse position and obtain the map coordinate
	/*Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	Vector2D mouseTile = Engine::GetInstance().map.get()->WorldToMap(mousePos.getX() - Engine::GetInstance().render.get()->camera.x,
		mousePos.getY() - Engine::GetInstance().render.get()->camera.y);*/


	//Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	/*Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld(mouseTile.getX(), mouseTile.getY());
	SDL_Rect rect = { 0,0,32,32 };
	Engine::GetInstance().render.get()->DrawTexture(mouseTileTex,
		highlightTile.getX(),
		highlightTile.getY(),
		&rect);*/

	// saves the tile pos for debugging purposes
	/*if (mouseTile.getX() >= 0 && mouseTile.getY() >= 0 || once) {
		tilePosDebug = "[" + std::to_string((int)mouseTile.getX()) + "," + std::to_string((int)mouseTile.getY()) + "] ";
		once = true;
	}*/

	//If mouse button is pressed modify enemy position
	/*if (Engine::GetInstance().input.get()->GetMouseButtonDown(1) == KEY_DOWN) {
		
		enemyList[0]->SetPosition(Vector2D(highlightTile.getX(), highlightTile.getY()));
		enemyList[0]->ResetPath();
		
	}*/

	if (changeLevel)
	{
		ChangeLevel();
		changeLevel = false;
		return true;
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		LoadState();

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN || player->checkpoint)
	{
		player->checkpoint = false;
		SaveState();
	}

	return ret;
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

PlayerState Scene::GetPlayerCurrentState()
{
	return player->GetCurrentState();
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
	
	/*int i = 0;
	for (pugi::xml_node enemyNode = sceneNode.child("entities").child("enemies").child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		if (!enemyList[i]->isDead) {
			enemyList[i]->SetPosition(Vector2D(enemyNode.attribute("x").as_int(),
				enemyNode.attribute("y").as_int()-5));
		}
		
		i++;
	}*/

	Engine::GetInstance().audio.get()->PlayFx(respawn);

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
	sceneNode.child("entities").child("player").attribute("y").set_value(player->GetPosition().getY()-30);

	int i = 0;
	/*for (pugi::xml_node enemyNode = sceneNode.child("entities").child("enemies").child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		if (!enemyList[i]->isDead) {
			enemyNode.attribute("x").set_value(enemyList[i]->GetPosition().getX());
			enemyNode.attribute("y").set_value(enemyList[i]->GetPosition().getY());
		}
		i++;
	}*/

	//enemies
	// ...


	//Saves the modifications to the XML 
	loadFile.save_file("config.xml");
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

void Scene::ChangeLevel() {
	LOG("Changing to the next level...");

	LOG("Before cleaning: diamondList size: %d", diamondList.size());
	LOG("Before cleaning: enemyList size: %d", enemyList.size());

	CleanEntities();

	LOG("After cleaning: diamondList size: %d", diamondList.size());
	LOG("After cleaning: enemyList size: %d", enemyList.size());

	// Limpia el mapa actual
	Engine::GetInstance().map.get()->CleanUp();

	// Obtén la ruta y nombre del segundo mapa desde el archivo XML
	const char* mapPath = "Assets/Maps/";
	const char* mapName = "Map2.tmx";

	// Construye la ruta completa
	std::string fullPath = std::string(mapPath);

	// Carga el segundo mapa
	if (!Engine::GetInstance().map->Load(fullPath.c_str(), mapName)) {
		LOG("Failed to load map: %s", fullPath.c_str());
	}
	else {
		LOG("Map loaded successfully: %s", fullPath.c_str());

		// Reubica al jugador al inicio del segundo mapa
		player->SetPosition(Vector2D(170, 20)); // Cambia la posición inicial según el nuevo mapa
	}
}

void Scene::CleanEntities() {
	LOG("Cleaning entities...");

	// Limpia los diamantes
	for (auto* diamond : diamondList) {
		if (diamond) {  // Valida que el puntero no sea nulo
			LOG("Cleaning diamond at address: %p", diamond);
			diamond->CleanUp();
			delete diamond;  // Libera la memoria
		}
		else {
			LOG("Found a null pointer in diamondList.");
		}
	}
	diamondList.clear();  // Limpia la lista

	// Limpia los enemigos
	for (auto it = enemyList.begin(); it != enemyList.end();) {
		if (*it) {
			LOG("Cleaning enemy at address: %p", *it);
			(*it)->CleanUp();
			delete* it;  // Libera la memoria
			it = enemyList.erase(it);  // Borra del contenedor y actualiza el iterador
		}
		else {
			LOG("Null pointer found in enemyList.");
			it = enemyList.erase(it);  // Elimina punteros nulos del contenedor
		}
	}
	LOG("Entities cleaned successfully.");
}


