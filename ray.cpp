#include "ray.h"
#include <iostream>

Ray::Ray()
    : Personaje("Rayvistasuperiors.png", 100),
      bastonEnergia(100),
      numeroVidas(3),
      SemillasRecolectadas(0),
      hongosbiolumicentesrecolectados(0),
      frameActual(0),
      tiempoFrame(0.0f),
    enMovimiento(false)
{
    // Cargar las texturas de animación
    texturasAnimacion.resize(3); // Base, pie derecho, pie izquierdo
    if (!texturasAnimacion[0].loadFromFile("Rayvistasuperiors.png")) {
        std::cerr << "Error: No se pudo cargar la textura Rayvistasuperiors.png" << std::endl;
        throw std::runtime_error("Fallo al cargar Rayvistasuperiors.png");
    }
    if (!texturasAnimacion[1].loadFromFile("Raycaminandoderechas.png")) {
        std::cerr << "Error: No se pudo cargar la textura Raycaminandoderechas.png" << std::endl;
        throw std::runtime_error("Fallo al cargar Raycaminandoderechas.png");
    }
    if (!texturasAnimacion[2].loadFromFile("Raycaminandoizquierdos.png")) {
        std::cerr << "Error: No se pudo cargar la textura Raycaminandoizquierdos.png" << std::endl;
        throw std::runtime_error("Fallo al cargar Raycaminandoizquierdos.png");
    }

    sprite.setTexture(texturasAnimacion[0]); // Iniciar con la textura base
    sprite.setPosition(100, 400);
    sprite.setScale(1.0f, 1.0f);
    std::cout << "Ray creado con vida inicial: " << vida_actual
              << ", posición: (100, 400), escala: (1.0, 1.0)" << std::endl;
}

void Ray::mover(float dx, float dy) {
    sprite.move(dx, dy);
    enMovimiento = true;
    std::cout << "Ray moviéndose, nueva posición: (" << sprite.getPosition().x << ", "
              << sprite.getPosition().y << ")" << std::endl;
    tiempoFrame += 1.0f / 60.0f; // Asumimos 60 FPS
    if (tiempoFrame >= duracionFrame) {
        tiempoFrame = 0.0f;
        frameActual = (frameActual + 1) % 3; // Ciclo: 0 -> 1 -> 2 -> 0
        sprite.setTexture(texturasAnimacion[frameActual]);
        std::cout << "Cambiando a frame " << frameActual << " ("
                  << (frameActual == 0 ? "base" : frameActual == 1 ? "pie derecho" : "pie izquierdo")
                  << ")" << std::endl;
    }
}

void Ray::saltar() {
    sprite.move(0, -50);
    std::cout << "Ray saltando, nueva posición: (" << sprite.getPosition().x << ", "
              << sprite.getPosition().y << ")" << std::endl;
    // No cambiamos la animación para el salto, pero podrías añadir una textura específica si quieres
}

void Ray::atacar(Personaje* enemigo) {
    enemigo->recibirdano(15);
    std::cout << "Ray ataca, causando 15 de daño" << std::endl;
}

void Ray::golpedeLuz(Personaje* enemigo) {
    if (bastonEnergia >= 50) {
        enemigo->recibirdano(25);
        bastonEnergia -= 50;
        std::cout << "Ray usa golpedeLuz, causando 25 de daño, energía restante: "
                  << bastonEnergia << std::endl;
    } else {
        std::cout << "Ray no tiene suficiente energía para golpedeLuz (necesita 50, tiene "
                  << bastonEnergia << ")" << std::endl;
    }
}

void Ray::supergolpedeLuz(Personaje* enemigo) {
    if (bastonEnergia >= 100) {
        enemigo->recibirdano(50);
        bastonEnergia -= 100;
        std::cout << "Ray usa supergolpedeLuz, causando 50 de daño, energía restante: "
                  << bastonEnergia << std::endl;
    } else {
        std::cout << "Ray no tiene suficiente energía para supergolpedeLuz (necesita 100, tiene "
                  << bastonEnergia << ")" << std::endl;
    }
}

void Ray::recolectarsemilla() {
    SemillasRecolectadas++;
    std::cout << "Ray recolecta una semilla, total: " << SemillasRecolectadas << std::endl;
}

void Ray::recolectarHongo() {
    hongosbiolumicentesrecolectados++;
    std::cout << "Ray recolecta un hongo bioluminiscente, total: "
              << hongosbiolumicentesrecolectados << std::endl;
}

void Ray::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    frameActual = 0; // Volver a la imagen base cuando se reposiciona
    sprite.setTexture(texturasAnimacion[0]);
    std::cout << "Ray posicionado en (" << x << ", " << y << "), frame reiniciado a base" << std::endl;
}

void Ray::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de Ray ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

sf::FloatRect Ray::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float scaleReduction = 0.7f;
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

sf::Vector2f Ray::getPosition() {
    return sprite.getPosition();
}

void Ray::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
    std::cout << "Dibujando Ray en (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << ") con frame " << frameActual << " ("
              << (frameActual == 0 ? "base" : frameActual == 1 ? "pie derecho" : "pie izquierdo")
              << ")" << std::endl;
}