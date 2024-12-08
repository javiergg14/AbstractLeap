#pragma once

#include "Vector2D.h"  // Necesitas definir o incluir esta clase para la representaci�n de vectores
#include "SDL2/SDL.h"

class Particle {
public:
    // Atributos de la part�cula
    Vector2D position;
    Vector2D velocity;
    float lifetime;  // Tiempo de vida de la part�cula
    SDL_Texture* texture;  // Textura de la part�cula
    SDL_Color color;  // Color de la part�cula
    float size; // Tama�o de la part�cula
    float angle; // �ngulo de rotaci�n de la part�cula

    // Constructor para inicializar la part�cula con posici�n, velocidad y tiempo de vida
    Particle(const Vector2D& pos, const Vector2D& vel, float life);

    // M�todo para actualizar la posici�n de la part�cula
    void Update(float dt);

    // Verificar si la part�cula ha muerto
    bool IsDead() const;
};