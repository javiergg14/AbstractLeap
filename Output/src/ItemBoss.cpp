#include "ItemBoss.h"
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

ItemBoss::ItemBoss() : Entity(EntityType::ITEMBOSS)
{
	name = "itemBoss";
}

ItemBoss::~ItemBoss() {}

bool ItemBoss::Awake() {
	return true;
}

bool ItemBoss::Start() {

	pickItemBoss = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/coin.ogg");

	//initilize textures
	itemBossTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/itemBoss.png");
	
	// L08 TODO 4: Add a physics to an item - initialize the physics body


	Engine::GetInstance().textures.get()->GetSize(itemBossTexture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::STATIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ITEMBOSS;

	pbody->listener = this;

	return true;
}

bool ItemBoss::Update(float dt)
{

	// Aplicar el movimiento oscilante
	position.setY(METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - texH / 2);

	// Actualizar la posición X de acuerdo con la física
	b2Transform pbodyPos1 = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos1.p.x) - texH / 2);

	// Dibujar el objeto
	Engine::GetInstance().render.get()->DrawTexture(itemBossTexture, (int)position.getX(), (int)position.getY());

	return true;
}

bool ItemBoss::CleanUp() {
	if (pbody) {
		Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
		pbody = nullptr;  // Evita punteros colgantes
	}
	return true;
}

void ItemBoss::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		Engine::GetInstance().audio.get()->PlayFx(pickItemBoss);
		Engine::GetInstance().scene.get()->bossLive -= 1;
		Engine::GetInstance().entityManager.get()->DestroyEntity(this);
		break;
	}
}