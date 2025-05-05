#include "hormigas.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
#include "hormigas.h"

Hormigas::Hormigas(bool infectadas)
    : Personaje(infectadas ? "npc infectado.png" : "npc.png", infectadas ? 50 : 50) {
    this->infectadas = infectadas;
}


void Hormigas::ataque(Personaje* objetivo) {
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

sf::FloatRect Hormigas::getBounds() {
    return sprite.getGlobalBounds();
}

void Hormigas::dibujar(sf::RenderWindow& ventana){
    ventana.draw(sprite);
}


