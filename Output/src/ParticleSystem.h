#pragma once

#include "Particle.h"
#include "Vector2D.h"
#include <vector>

class ParticleSystem {
public:
    std::vector<Particle> particles;  // Lista de part�culas

    // Constructor y destructor
    ParticleSystem();
    ~ParticleSystem();

    // Crear part�culas en una posici�n espec�fica
    void CreateParticles(const Vector2D& position, const Vector2D& velocity, float lifetime);

    // Actualizar todas las part�culas
    void Update(float dt);

    // Dibujar las part�culas
    void Draw();

    // Verificar si todas las part�culas han muerto
    bool IsFinished() const;

    void StartAnimation() { isActive = true; }  // Activar animaci�n
    void StopAnimation() { isActive = false; }  // Desactivar animaci�n

    bool IsActive() const { return isActive; }  // Comprobar si la animaci�n est� activa

private:
    bool isActive = false;
};
