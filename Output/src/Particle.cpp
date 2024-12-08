#include "Particle.h"

// Constructor para inicializar una part�cula con su posici�n, velocidad y tiempo de vida
Particle::Particle(const Vector2D& pos, const Vector2D& vel, float life)
    : position(pos), velocity(vel), lifetime(life) {}

// Actualizar la posici�n de la part�cula seg�n su velocidad y reducir su tiempo de vida
void Particle::Update(float dt) {
    position.setX(position.getX() + velocity.getX() * dt);  // Actualiza la componente x
    position.setY(position.getY() + velocity.getY() * dt);  // Actualiza la componente y
}

// Verificar si la part�cula ha muerto (cuando su tiempo de vida es 0 o menor)
bool Particle::IsDead() const {
    return lifetime <= 0.0f;
}