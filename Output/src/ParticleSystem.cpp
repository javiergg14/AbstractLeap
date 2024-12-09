#include "ParticleSystem.h"
#include "Engine.h"
#include "Render.h"
#include <cstdlib>  // Para números aleatorios

// Constructor
ParticleSystem::ParticleSystem() {}

// Destructor
ParticleSystem::~ParticleSystem() {
    particles.clear();  // Limpiar las partículas al destruir el sistema
}

// Actualizar todas las partículas
void ParticleSystem::Update(float dt) {
    if (isActive) {  // Solo actualizar si la animación está activa
        for (auto it = particles.begin(); it != particles.end();) {
            it->Update(dt);  // Actualizar la partícula

            // Eliminar las partículas que han muerto
            if (it->IsDead()) {
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }

        // Si todas las partículas están muertas, desactivar la animación
        if (particles.empty()) {
            StopAnimation();  // Desactivar la animación
        }
    }
}

// Dibujar las partículas
void ParticleSystem::Draw() {
    // Recorrer las partículas y dibujarlas
    if (isActive) {  // Solo dibujar si la animación está activa
        for (const auto& particle : particles) {
            if (!particle.IsDead()) {
                int x = static_cast<int>(particle.position.getX());
                int y = static_cast<int>(particle.position.getY());
                int radius = static_cast<int>(particle.size);  // Tamaño de la partícula

                // Calcular la opacidad en función del tiempo de vida
                Uint8 alpha = static_cast<Uint8>(255 * (particle.lifetime / 10.0f));  // 10 es el lifetime máximo

                // Dibujar la partícula
                Engine::GetInstance().render.get()->DrawCircle(x, y, radius, 255, 255, 0, alpha, false);  // Amarillo
            }
        }
    }
}

// Verificar si todas las partículas han muerto
bool ParticleSystem::IsFinished() const {
    return particles.empty();
}

void ParticleSystem::CreateParticles(const Vector2D& position, const Vector2D& velocity, float lifetime) {
    if (isActive) {  // Solo crear partículas si la animación está activa
        for (int i = 0; i < 5; ++i) {
            // Aleatorizar la velocidad
            float randomX = (rand() % 20 - 10) * 0.1f;
            float randomY = (rand() % 20 - 10) * 0.1f;
            Particle p(position, Vector2D(velocity.getX() + randomX, velocity.getY() + randomY), lifetime);
            particles.push_back(p);
        }
    }
}
