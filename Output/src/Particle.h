#pragma once

#include "Vector2D.h"  // Necesitas definir o incluir esta clase para la representación de vectores
#include "SDL2/SDL.h"

class Particle {
public:
    Vector2D position;
    Vector2D velocity;
    float lifetime = 10;  // Tiempo de vida de la partícula en segundos
    float maxLifetime;  // Para restaurar el tiempo de vida original
    SDL_Color color; // Color de la partícula
    float size; // Tamaño de la partícula
    float angle; // Ángulo de rotación de la partícula

    // Constructor para inicializar la partícula con posición, velocidad y tiempo de vida
    Particle(const Vector2D& pos, const Vector2D& vel, float life);

    // Método para actualizar la posición de la partícula y disminuir su tiempo de vida
    void Update(float dt);

    // Verificar si la partícula ha muerto (cuando su tiempo de vida es 0 o menor)
    bool IsDead() const;

    // Método para calcular la opacidad en función del tiempo de vida
    Uint8 GetAlpha() const;
};
