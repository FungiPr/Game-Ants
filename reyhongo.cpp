#include "reyhongo.h"
#include "hormigas.h"


Reyhongo::Reyhongo()
    : Personaje("Reyhongoparado.png", 200) {
    energiaHormigas = 0;
}


void Reyhongo::atacar(Personaje* objetivo) {
    objetivo->recibirdano(20);
}


void Reyhongo::controlarhormigas(vector<Personaje*>& npc) {
    Hormigas* nuevaInfectada = new Hormigas(true);
    npc.push_back(nuevaInfectada);
    cout << "El Rey Hongo ha controlado una nueva hormiga infectada!" << endl;


    energiaHormigas = 0;
    for (auto& hormiga : npc) {
        Hormigas* h = dynamic_cast<Hormigas*>(hormiga);
        if (h && h->getInfectadas()) {
            energiaHormigas += 10;
        }
    }
    cout << "Energía del Rey Hongo ahora: " << energiaHormigas << endl;
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

// Dibujar
void Reyhongo::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}
