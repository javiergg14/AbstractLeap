#include "GuiHUD.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include "Textures.h"
#include "Scene.h"

GuiHUD::GuiHUD(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::HUD, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiHUD::~GuiHUD()
{
}

bool GuiHUD::Update(float dt)
{
	if (!diamondTexture) {
		diamondTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/blueCrystal.png");
	}
	if (!diamondOutlineTexture) {
		diamondOutlineTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/outlineCrystal.png");
	}
	if (!lifeTexture) {
		lifeTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/redGem.png");
	}
	if (!lifeOutlineTexture) {
		lifeOutlineTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/PNG/Items/outlineGem.png");
	}

	DrawLives(Engine::GetInstance().scene.get()->maxLives, Engine::GetInstance().scene.get()->currentLives);
	DrawDiamonds(Engine::GetInstance().scene.get()->maxDiamonds, Engine::GetInstance().scene.get()->currentDiamonds);

	return true;
}

void GuiHUD::DrawLives(int maxLives, int currentLives)
{
	SDL_Rect camera = Engine::GetInstance().render.get()->camera;

	for (int i = 0; i < maxLives; i++)
	{
		if (i < currentLives) {
			Engine::GetInstance().render.get()->DrawTexture(lifeTexture, -camera.x + (i * 100), -camera.y + 200);
		}
		else {
			Engine::GetInstance().render.get()->DrawTexture(lifeOutlineTexture, -camera.x + (i * 100), -camera.y + 200);
		}
	}
}
void GuiHUD::DrawDiamonds(int maxDiamonds, int currentDiamonds)
{
	SDL_Rect camera = Engine::GetInstance().render.get()->camera;

	for (int i = 0; i < maxDiamonds; i++)
	{
		if (i < currentDiamonds) {
			Engine::GetInstance().render.get()->DrawTexture(diamondTexture, -camera.x +500+ (i * 100), -camera.y + 200);
		}
		else {
			Engine::GetInstance().render.get()->DrawTexture(diamondOutlineTexture, -camera.x +500+ (i * 100), -camera.y + 200);
		}
	}
}

void GuiHUD::DrawAbility()
{
	SDL_Rect camera = Engine::GetInstance().render.get()->camera;

	if (true)
	{
		Engine::GetInstance().render.get()->DrawTexture(diamondTexture, -camera.x + camera.w, -camera.y + 200);
	}
}
