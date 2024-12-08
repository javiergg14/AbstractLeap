#pragma once

#include "Vector2D.h"  // Necesitas definir o incluir esta clase para la representación de vectores
#include "SDL2/SDL.h"

class Particle {
public:
    // Atributos de la partícula
    Vector2D position;
    Vector2D velocity;
    float lifetime;  // Tiempo de vida de la partícula
    SDL_Texture* texture;  // Textura de la partícula
    SDL_Color color;  // Color de la partícula
    float size; // Tamaño de la partícula
    float angle; // Ángulo de rotación de la partícula

    // Constructor para inicializar la partícula con posición, velocidad y tiempo de vida
    Particle(const Vector2D& pos, const Vector2D& vel, float life);

    // Método para actualizar la posición de la partícula
    void Update(float dt);

    // Verificar si la partícula ha muerto
    bool IsDead() const;
};