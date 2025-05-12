#include "reyhongo.h"
#include "hormigas.h"


Reyhongo::Reyhongo()
    : Personaje("Reyhongoparado.png", 200) {
    energiaHormigas = 0;
}


void Reyhongo::atacar(Personaje* objetivo) {
    objetivo->recibirdano(20);
}


void Reyhongo::controlarHormigas(std::vector<std::unique_ptr<Personaje>>& npc, const std::vector<sf::Vector2f>& posiciones) {
    std::cout << "Rey Hongo: Iniciando controlarHormigas, posiciones recibidas: " << posiciones.size() << std::endl;
    npc.clear();
    std::cout << "Rey Hongo: NPCs limpiados, tamaño actual: " << npc.size() << std::endl;
    if (posiciones.empty()) {
        std::cerr << "Error: El vector de posiciones está vacío, no se pueden generar hormigas" << std::endl;
        return;
    }
    for (size_t i = 0; i < posiciones.size() && i < 4; ++i) {
        std::cout << "Rey Hongo: Creando hormiga " << i + 1 << " en posición ("
                  << posiciones[i].x << ", " << posiciones[i].y << ")" << std::endl;
        npc.push_back(std::make_unique<Hormigas>(true));
        if (npc.back()) {
            npc.back()->setPosition(posiciones[i].x, posiciones[i].y);
            npc.back()->setScale(1.0f, 1.0f);
            std::cout << "Rey Hongo: Hormiga infectada creada en (" << posiciones[i].x << ", "
                      << posiciones[i].y << ") con escala (1.0, 1.0), vida: "
                      << npc.back()->getVida() << std::endl;
        } else {
            std::cerr << "Error: No se pudo crear hormiga en posición (" << posiciones[i].x << ", "
                      << posiciones[i].y << ")" << std::endl;
        }
    }
    std::cout << "Rey Hongo: Hormigas generadas, NPCs: " << npc.size() << std::endl;
}

sf::FloatRect Reyhongo::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    // Reducir el rectángulo de colisión para que sea más pequeño
    float scaleReduction = 0.75f; // Usar la mitad del tamaño escalado
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

void Reyhongo::setPosition(float x, float y) {
    sprite.setPosition(x,y);
}

void Reyhongo::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de Ray ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

// Dibujar
void Reyhongo::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}
