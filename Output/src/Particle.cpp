#include "Particle.h"

// Constructor para inicializar una part�cula con su posici�n, velocidad y tiempo de vida
Particle::Particle(const Vector2D& pos, const Vector2D& vel, float life)
    : position(pos), velocity(vel), lifetime(life), maxLifetime(life), size(20.0f), angle(0.0f) {
    color = { 255, 255, 0, 255 };  // Color amarillo
}

// Actualizar la posici�n de la part�cula seg�n su velocidad y reducir su tiempo de vida
void Particle::Update(float dt) {
    position.setX(position.getX() + velocity.getX() * dt);  // Actualiza la componente x
    position.setY(position.getY() + velocity.getY() * dt);  // Actualiza la componente y

    // Reducir el tiempo de vida de la part�cula
    lifetime -= 0.001;

    // Limitar el tiempo de vida al valor m�nimo de 0
    if (lifetime < 0) {
        lifetime = 0;
    }
}

// Verificar si la part�cula ha muerto (cuando su tiempo de vida es 0 o menor)
bool Particle::IsDead() const {
    return lifetime <= 0.0f;
}

// Calcular la opacidad de la part�cula en funci�n del tiempo de vida
Uint8 Particle::GetAlpha() const {
    // La opacidad se desvanece de 255 (totalmente opaco) a 0 (totalmente transparente)
    return static_cast<Uint8>((lifetime / maxLifetime) * 255);
}
