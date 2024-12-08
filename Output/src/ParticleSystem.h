#pragma once

#include "Particle.h"  // Incluir la definición de Particle
#include <vector>
#include "SDL2/SDL.h"

class ParticleSystem {
public:
    // Constructor
    ParticleSystem();

    // Destructor
    ~ParticleSystem();

    // Crear partículas en una posición específica con una velocidad y tiempo de vida
    void CreateParticles(const Vector2D& position, const Vector2D& velocity, float lifetime);

    // Actualizar todas las partículas
    void Update(float dt);

    // Dibujar las partículas (esto depende de tu motor de renderizado)
    void Draw();

    // Verificar si el sistema de partículas está vacío
    bool IsFinished() const;

private:
    std::vector<Particle> particles;  // Lista de partículas
};