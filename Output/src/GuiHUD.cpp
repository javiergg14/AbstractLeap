#include "GuiHUD.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include "Textures.h"
#include "Scene.h"
#include "Timer.h"

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
	DrawTimer();

	return true;
}

void GuiHUD::DrawLives(int maxLives, int currentLives)
{
	SDL_Rect camera = Engine::GetInstance().render.get()->camera;

	for (int i = 0; i < maxLives; i++)
	{
		if (i < currentLives) {
			Engine::GetInstance().render.get()->DrawTexture(lifeTexture, -camera.x + 30 + (i * 70), -camera.y + 20);
		}
		else {
			Engine::GetInstance().render.get()->DrawTexture(lifeOutlineTexture, -camera.x + 30 + (i * 70), -camera.y + 20);
		}
	}
}
void GuiHUD::DrawDiamonds(int maxDiamonds, int currentDiamonds)
{
	SDL_Rect camera = Engine::GetInstance().render.get()->camera;

	for (int i = 0; i < maxDiamonds; i++)
	{
		if (i < currentDiamonds) {
			Engine::GetInstance().render.get()->DrawTexture(diamondTexture, -camera.x + 1070 + (i * 70), -camera.y + 20);
		}
		else {
			Engine::GetInstance().render.get()->DrawTexture(diamondOutlineTexture, -camera.x + 1070 + (i * 70), -camera.y + 20);
		}
	}
}
void GuiHUD::DrawTimer()
{
	SDL_Rect camera = Engine::GetInstance().render.get()->camera;

	int seconds = Engine::GetInstance().scene.get()->gameTimer.ReadSec();

	char text[64];
	snprintf(text, sizeof(text), "%d s", seconds);

	Engine::GetInstance().render->DrawText(text, camera.w/2-25, 20, 75, 45);

}
