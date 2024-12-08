#pragma once

#include "Particle.h"  // Incluir la definici�n de Particle
#include <vector>
#include "SDL2/SDL.h"

class ParticleSystem {
public:
    // Constructor
    ParticleSystem();

    // Destructor
    ~ParticleSystem();

    // Crear part�culas en una posici�n espec�fica con una velocidad y tiempo de vida
    void CreateParticles(const Vector2D& position, const Vector2D& velocity, float lifetime);

    // Actualizar todas las part�culas
    void Update(float dt);

    // Dibujar las part�culas (esto depende de tu motor de renderizado)
    void Draw();

    // Verificar si el sistema de part�culas est� vac�o
    bool IsFinished() const;

private:
    std::vector<Particle> particles;  // Lista de part�culas
};