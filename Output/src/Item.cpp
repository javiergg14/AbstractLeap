#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name = "item";
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {

	//initilize textures
	keyGreen = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/keyGreen.png");
	
	// L08 TODO 4: Add a physics to an item - initialize the physics body

	Engine::GetInstance().textures.get()->GetSize(keyGreen, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle(240, 500, 10, bodyType::STATIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Item::Update(float dt)
{
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos1 = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos1.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos1.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(keyGreen, (int)position.getX(), (int)position.getY());

	return true;
}

bool Item::CleanUp()
{
	return true;
}