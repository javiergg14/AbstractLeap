#include "GuiControlButton.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();

		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {
		
			state = GuiControlState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}
			
			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//L16: TODO 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			// Botón en estado deshabilitado: gris con opacidad reducida
			Engine::GetInstance().render->DrawRectangle(bounds, 150, 150, 150, 180, true, false); // Fondo gris claro
			Engine::GetInstance().render->DrawRectangle(bounds, 100, 100, 100, 200, false, false); // Borde más oscuro
			break;

		case GuiControlState::NORMAL:
			// Botón en estado normal: azul claro
			Engine::GetInstance().render->DrawRectangle(bounds, 0, 122, 204, 255, true, false); // Fondo azul claro
			Engine::GetInstance().render->DrawRectangle(bounds, 0, 100, 170, 255, false, false); // Borde más oscuro
			break;

		case GuiControlState::FOCUSED:
			// Botón enfocado: azul más oscuro con un borde blanco
			Engine::GetInstance().render->DrawRectangle(bounds, 0, 90, 150, 255, true, false);  // Fondo azul oscuro
			Engine::GetInstance().render->DrawRectangle(bounds, 255, 255, 255, 255, false, false); // Borde blanco
			break;

		case GuiControlState::PRESSED:
			// Botón presionado: verde con un efecto de sombra (borde negro)
			Engine::GetInstance().render->DrawRectangle(bounds, 50, 200, 50, 255, true, false);  // Fondo verde
			Engine::GetInstance().render->DrawRectangle(bounds, 0, 0, 0, 255, false, false);     // Borde negro
			break;
		}

		Engine::GetInstance().render->DrawText(text.c_str(), bounds.x, bounds.y, bounds.w, bounds.h);

	}

	return false;
}

