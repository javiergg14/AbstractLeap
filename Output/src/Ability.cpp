#include "Ability.h"
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

Ability::Ability() : Entity(EntityType::ABILITY)
{
	name = "ability";
}

Ability::~Ability() {}

bool Ability::Awake() {
	return true;
}

bool Ability::Start() {

	pickAbility = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/coin.ogg");

	//initilize textures
	abilityTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/doubleJump.png");
	
	// L08 TODO 4: Add a physics to an item - initialize the physics body


	Engine::GetInstance().textures.get()->GetSize(abilityTexture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::STATIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ABILITY;

	pbody->listener = this;

	return true;
}

bool Ability::Update(float dt)
{
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  

	static float oscillationAmplitude = 10.0f;
	static float oscillationSpeed = 0.05f;
	static float initialY = 0.0f;
	initialY += oscillationSpeed;

	// Aplicar el movimiento oscilante
	position.setY(METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - texH / 2 + oscillationAmplitude * sin(initialY));

	// Actualizar la posición X de acuerdo con la física
	b2Transform pbodyPos1 = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos1.p.x) - texH / 2);

	// Dibujar el objeto
	Engine::GetInstance().render.get()->DrawTexture(abilityTexture, (int)position.getX(), (int)position.getY());

	return true;
}

bool Ability::CleanUp() {
	if (pbody) {
		Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
		pbody = nullptr;  // Evita punteros colgantes
	}
	return true;
}

void Ability::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		Engine::GetInstance().audio.get()->PlayFx(pickAbility);
		//Engine::GetInstance().scene.get()->currentAbilitys++;

		// Encuentra y elimina el puntero de diamondList
		//auto& diamondList = Engine::GetInstance().scene.get()->abilityList;
		//auto it = std::find(diamondList.begin(), diamondList.end(), this);
		//if (it != diamondList.end()) {
		//	diamondList.erase(it);  // Elimina el puntero de la lista
		//}
		//else {
		//	LOG("Ability not found in diamondList: %p", this);
		//}

		// Destruye el diamante
		Engine::GetInstance().entityManager.get()->DestroyEntity(this);
		break;
	}
}