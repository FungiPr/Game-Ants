#include "ray.h"
#include"personaje.h"
#include <SFML/Graphics.hpp>
using namespace std;

Ray::Ray()
    : Personaje("RayParados.png",100),
    bastonEnergia(100),
    numeroVidas(3),
    SemillasRecolectadas(0),
    hongosbiolumicentesrecolectados(0)
{
    sprite.setPosition(100,400);
    sprite.setScale(1.0f, 1.0f);
}

void Ray::mover(float dx,float dy){
    sprite.move(dx,dy);
}

void Ray::saltar(){
    sprite.move(0,-50);
}

void Ray::atacar(Personaje* enemigo) {
    enemigo->recibirdano(15);
}

void Ray::golpedeLuz(Personaje* enemigo){
    if (bastonEnergia == 50){
        enemigo->recibirdano(25);
        bastonEnergia -= 50;
    }
}

void Ray::supergolpedeLuz(Personaje* enemigo){
    if (bastonEnergia == 100){
        enemigo->recibirdano(50);
        bastonEnergia -= 100;
    }
}

void Ray::recolectarsemilla(){
    SemillasRecolectadas++;
}

void Ray::recolectarHongo(){
    hongosbiolumicentesrecolectados++;
}
void Ray::setPosition(float x, float y) {
    sprite.setPosition(x,y);
}

void Ray::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de Ray ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

sf::FloatRect Ray::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    // Reducir el rectángulo de colisión para que sea más pequeño
    float scaleReduction = 0.7f; // Usar la mitad del tamaño escalado
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

sf::Vector2f Ray::getPosition() {
    return sprite.getPosition();
}

void Ray::dibujar(sf::RenderWindow& ventana){
    ventana.draw(sprite);
}