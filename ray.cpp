#include "ray.h"
#include"personaje.h"
#include <SFML/Graphics.hpp>
using namespace std;

Ray::Ray()
    : Personaje("Ray.png",100),
    bastonEnergia(100),
    numeroVidas(3),
    SemillasRecolectadas(0),
    hongosbiolumicentesrecolectados(0)
{
    sprite.setPosition(100,400);
}

void Ray::Caminar(float dx,float dy){
    sprite.move(dx,dy);
}

void Ray::saltar(){
    sprite.move(0,-50);
}

void Ray::golpear(Personaje* enemigo){
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

sf::FloatRect Ray::getBounds() {
    return sprite.getGlobalBounds();
}

void Ray::dibujar(sf::RenderWindow& ventana){
    ventana.draw(sprite);
}