#include "ParticleSystem.h"
#include "Engine.h"
#include <cstdlib>  // Para n�meros aleatorios, si quieres aleatorizar la velocidad

// Constructor
ParticleSystem::ParticleSystem() {}

// Destructor
ParticleSystem::~ParticleSystem() {
    particles.clear();  // Limpiar las part�culas al destruir el sistema
}

// Crear part�culas en una posici�n espec�fica
void ParticleSystem::CreateParticles(const Vector2D& position, const Vector2D& velocity, float lifetime) {
    // Crear varias part�culas
    for (int i = 0; i < 5; ++i) {  // Crear 5 part�culas por ejemplo
        // Crear una velocidad aleatoria para cada part�cula
        float randomX = (rand() % 20 - 10) * 0.1f;  // Aleatorizar la velocidad en el rango [-1, 1]
        float randomY = (rand() % 20 - 10) * 0.1f;
        Particle p(position, Vector2D(velocity.getX() + randomX, velocity.getY() + randomY), lifetime);
        particles.push_back(p);
    }
}

// Actualizar todas las part�culas
void ParticleSystem::Update(float dt) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->Update(dt);  // Actualizar cada part�cula

        // Si la part�cula ha muerto, eliminarla
        if (it->IsDead()) {
            it = particles.erase(it);
        }
        else {
            ++it;
        }
    }
}

// Dibujar las part�culas
void ParticleSystem::Draw() {
    for (const auto& particle : particles) {
        // Verificar si la part�cula sigue viva
        if (particle.lifetime > 0) {
            // Establecer el color de la part�cula si se tiene una propiedad de color
            SDL_SetTextureColorMod(particle.texture, particle.color.r, particle.color.g, particle.color.b);

            // Dibujar la part�cula (por ejemplo, con una textura, si la tienes)
            /*Engine::GetInstance().render.get()->DrawTexture(particle.texture,
                static_cast<int>(particle.position.getX()),
                static_cast<int>(particle.position.getY()));*/
        }
    }
}

// Verificar si todas las part�culas han muerto
bool ParticleSystem::IsFinished() const {
    return particles.empty();
}
