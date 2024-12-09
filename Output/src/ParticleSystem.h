#pragma once

#include "Particle.h"
#include "Vector2D.h"
#include <vector>

class ParticleSystem {
public:
    std::vector<Particle> particles;  // Lista de partículas

    // Constructor y destructor
    ParticleSystem();
    ~ParticleSystem();

    // Crear partículas en una posición específica
    void CreateParticles(const Vector2D& position, const Vector2D& velocity, float lifetime);

    // Actualizar todas las partículas
    void Update(float dt);

    // Dibujar las partículas
    void Draw();

    // Verificar si todas las partículas han muerto
    bool IsFinished() const;

    void StartAnimation() { isActive = true; }  // Activar animación
    void StopAnimation() { isActive = false; }  // Desactivar animación

    bool IsActive() const { return isActive; }  // Comprobar si la animación está activa

private:
    bool isActive = false;
};
