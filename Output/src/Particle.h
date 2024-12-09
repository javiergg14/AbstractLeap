#pragma once

#include "Vector2D.h"  // Necesitas definir o incluir esta clase para la representaci�n de vectores
#include "SDL2/SDL.h"

class Particle {
public:
    Vector2D position;
    Vector2D velocity;
    float lifetime = 10;  // Tiempo de vida de la part�cula en segundos
    float maxLifetime;  // Para restaurar el tiempo de vida original
    SDL_Color color; // Color de la part�cula
    float size; // Tama�o de la part�cula
    float angle; // �ngulo de rotaci�n de la part�cula

    // Constructor para inicializar la part�cula con posici�n, velocidad y tiempo de vida
    Particle(const Vector2D& pos, const Vector2D& vel, float life);

    // M�todo para actualizar la posici�n de la part�cula y disminuir su tiempo de vida
    void Update(float dt);

    // Verificar si la part�cula ha muerto (cuando su tiempo de vida es 0 o menor)
    bool IsDead() const;

    // M�todo para calcular la opacidad en funci�n del tiempo de vida
    Uint8 GetAlpha() const;
};
