#include "ParticleSystem.h"
#include "Engine.h"
#include "Render.h"
#include <cstdlib>  // Para n�meros aleatorios

// Constructor
ParticleSystem::ParticleSystem() {}

// Destructor
ParticleSystem::~ParticleSystem() {
    particles.clear();  // Limpiar las part�culas al destruir el sistema
}

// Actualizar todas las part�culas
void ParticleSystem::Update(float dt) {
    if (isActive) {  // Solo actualizar si la animaci�n est� activa
        for (auto it = particles.begin(); it != particles.end();) {
            it->Update(dt);  // Actualizar la part�cula

            // Eliminar las part�culas que han muerto
            if (it->IsDead()) {
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }

        // Si todas las part�culas est�n muertas, desactivar la animaci�n
        if (particles.empty()) {
            StopAnimation();  // Desactivar la animaci�n
        }
    }
}

// Dibujar las part�culas
void ParticleSystem::Draw() {
    // Recorrer las part�culas y dibujarlas
    if (isActive) {  // Solo dibujar si la animaci�n est� activa
        for (const auto& particle : particles) {
            if (!particle.IsDead()) {
                int x = static_cast<int>(particle.position.getX());
                int y = static_cast<int>(particle.position.getY());
                int radius = static_cast<int>(particle.size);  // Tama�o de la part�cula

                // Calcular la opacidad en funci�n del tiempo de vida
                Uint8 alpha = static_cast<Uint8>(255 * (particle.lifetime / 10.0f));  // 10 es el lifetime m�ximo

                // Dibujar la part�cula
                Engine::GetInstance().render.get()->DrawCircle(x, y, radius, 255, 255, 0, alpha, false);  // Amarillo
            }
        }
    }
}

// Verificar si todas las part�culas han muerto
bool ParticleSystem::IsFinished() const {
    return particles.empty();
}

void ParticleSystem::CreateParticles(const Vector2D& position, const Vector2D& velocity, float lifetime) {
    if (isActive) {  // Solo crear part�culas si la animaci�n est� activa
        for (int i = 0; i < 5; ++i) {
            // Aleatorizar la velocidad
            float randomX = (rand() % 20 - 10) * 0.1f;
            float randomY = (rand() % 20 - 10) * 0.1f;
            Particle p(position, Vector2D(velocity.getX() + randomX, velocity.getY() + randomY), lifetime);
            particles.push_back(p);
        }
    }
}
