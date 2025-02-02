#include "Diamond.h"
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

Diamond::Diamond() : Entity(EntityType::DIAMOND)
{
	name = "diamond";
}

Diamond::~Diamond() {}

bool Diamond::Awake() {
	return true;
}

bool Diamond::Start() {

	pickDiamond = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/coin.ogg");

	//initilize textures
	diamondTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/blueCrystal.png");
	
	// L08 TODO 4: Add a physics to an item - initialize the physics body


	Engine::GetInstance().textures.get()->GetSize(diamondTexture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::STATIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::DIAMOND;

	pbody->listener = this;

	return true;
}

bool Diamond::Update(float dt)
{

	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos1 = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos1.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos1.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(diamondTexture, (int)position.getX(), (int)position.getY());

	return true;
}

bool Diamond::CleanUp() {
	if (pbody) {
		Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
		pbody = nullptr;  // Evita punteros colgantes
	}
	return true;
}

void Diamond::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		Engine::GetInstance().audio.get()->PlayFx(pickDiamond);
		Engine::GetInstance().scene.get()->currentDiamonds++;

		// Encuentra y elimina el puntero de diamondList
		auto& diamondList = Engine::GetInstance().scene.get()->diamondList;
		auto it = std::find(diamondList.begin(), diamondList.end(), this);
		if (it != diamondList.end()) {
			diamondList.erase(it);  // Elimina el puntero de la lista
		}
		else {
			LOG("Diamond not found in diamondList: %p", this);
		}

		// Destruye el diamante
		Engine::GetInstance().entityManager.get()->DestroyEntity(this);
		break;
	}
}