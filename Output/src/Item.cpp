#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name = "item";
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {

	pickItem = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/coin.ogg");

	//initilize textures
	keyGreen = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/keyGreen.png");
	
	// L08 TODO 4: Add a physics to an item - initialize the physics body


	Engine::GetInstance().textures.get()->GetSize(keyGreen, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle(240, 500, 10, bodyType::STATIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	pbody->listener = this;

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
	Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collided with player - DESTROY");
		Engine::GetInstance().audio.get()->PlayFx(pickItem);
		Engine::GetInstance().entityManager.get()->DestroyEntity(this);
		break;
	}
}