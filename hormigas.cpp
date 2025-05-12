#include "hormigas.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
#include "hormigas.h"

Hormigas::Hormigas(bool infectadas)
: Personaje(infectadas ? "NpcInfectados.png" : "Npcparado.png", 50), infectadas(infectadas) {
    sprite.setScale(1.0f, 1.0f);
    std::cout << "Hormiga creada (" << (infectadas ? "infectada" : "no infectada")
              << ") en posición (" << posicion_x << ", " << posicion_y << "), vida inicial: "
              << vida_actual << std::endl;
}



void Hormigas::atacar(Personaje* objetivo) {
    if (infectadas) {
        objetivo->recibirdano(10);
}
}

bool Hormigas::getInfectadas(){
    return infectadas;
}
void Hormigas::setPosition(float x, float y) {
    sprite.setPosition(x,y);
}

void Hormigas::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de hormiga ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

sf::FloatRect Hormigas::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    // Reducir el rectángulo de colisión para que sea más pequeño
    float scaleReduction = 0.75f; // Usar la mitad del tamaño escalado
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

void Hormigas::dibujar(sf::RenderWindow& ventana){
    ventana.draw(sprite);
}


