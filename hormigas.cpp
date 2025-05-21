#include "hormigas.h"
#include <SFML/Graphics.hpp>
using namespace std;

Hormigas::Hormigas(bool infectadas, sf::Vector2f posicion)
    : Personaje(infectadas ? "hormigainfectadavistasuperior.png" : "Npcparado.png", 50),
      infectadas(infectadas),
      enPosicionDisparo(false)
{
    sprite.setPosition(posicion);
    sprite.setScale(1.0f, 1.0f);
    posicion_x = posicion.x;
    posicion_y = posicion.y;
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    std::cout << "Hormiga creada (" << (infectadas ? "infectada" : "no infectada")
              << ") en posición (" << posicion_x << ", " << posicion_y << "), vida inicial: "
              << vida_actual << std::endl;
}

void Hormigas::moverHaciaJugador(sf::Vector2f posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse) {
    if (!infectadas) {
        std::cout << "Hormiga no infectada, no se mueve" << std::endl;
        return;
    }

    // Calcular vector de dirección hacia el jugador
    sf::Vector2f posicionActual = sprite.getPosition();
    sf::Vector2f direccion = posicionJugador - posicionActual;
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    std::cout << "Hormiga en (" << posicionActual.x << ", " << posicionActual.y
              << "), Jugador en (" << posicionJugador.x << ", " << posicionJugador.y
              << "), Distancia: " << distancia << ", deltaTime: " << deltaTime << std::endl;

    // Normalizar dirección
    if (distancia > 0) {
        direccion.x /= distancia;
        direccion.y /= distancia;
    } else {
        std::cout << "Distancia cero, no se mueve" << std::endl;
        return;
    }

    // Determinar si moverse hacia el jugador o detenerse
    float movimiento = 0.0f;
    if (distancia > distanciaObjetivo + 10.0f) {
        movimiento = velocidad * deltaTime; // Mover hacia el jugador
        enPosicionDisparo = false;
        std::cout << "Moviendo hacia jugador, movimiento: " << movimiento << std::endl;
    } else {
        enPosicionDisparo = true;
        std::cout << "En posición de disparo, no se mueve" << std::endl;
        return;
    }

    // Calcular desplazamiento
    float dx = direccion.x * movimiento;
    float dy = direccion.y * movimiento;

    // Verificar si el movimiento es válido
    float newX = posicionActual.x + dx;
    float newY = posicionActual.y + dy;
    bool movimientoValido = puedeMoverse(newX, newY);
    std::cout << "Intentando mover a (" << newX << ", " << newY << "), PuedeMoverse: " << (movimientoValido ? "true" : "false") << std::endl;

    if (movimientoValido) {
        sprite.move(dx, dy);
        posicion_x = newX;
        posicion_y = newY;
        std::cout << "Hormiga movida a (" << newX << ", " << newY << ")" << std::endl;
    } else {
        std::cout << "Movimiento bloqueado por colisión" << std::endl;
    }
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
    float scaleReduction = 0.70f;// Reducir el tamaño para colisiones más precisas
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    sf::FloatRect adjustedBounds(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
    std::cout << "Hormiga bounds: [" << adjustedBounds.left << ", " << adjustedBounds.top << ", "
              << adjustedBounds.width << ", " << adjustedBounds.height << "]" << std::endl;
    return adjustedBounds;
}

void Hormigas::dispararEspora(std::vector<sf::CircleShape>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, Personaje* jugador) {
    if (relojDisparo.getElapsedTime().asSeconds() < 3.0f) return;

    // Crear esporaw
    sf::CircleShape espora(20.0f);
    espora.setFillColor(sf::Color(128, 0, 128)); // Morado
    espora.setPosition(sprite.getPosition());

    // Obtener el centro del sprite del jugador (Ray)
    sf::FloatRect jugadorBounds = jugador->getBounds();
    sf::Vector2f centroJugador(jugadorBounds.left + jugadorBounds.width / 2.0f,
                               jugadorBounds.top + jugadorBounds.height / 2.0f);

    // Calcular dirección hacia el centro del jugador
    sf::Vector2f direccion(centroJugador.x - sprite.getPosition().x, centroJugador.y - sprite.getPosition().y);
    float magnitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    if (magnitud > 0) {
        direccion.x /= magnitud;
        direccion.y /= magnitud;
    }

    esporas.push_back(espora);
    direccionesEsporas.push_back(direccion);
    relojDisparo.restart();
    std::cout << "Hormiga disparó espora desde (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << ") hacia el centro de Ray (" << centroJugador.x << ", " << centroJugador.y << ")" << std::endl;
}

void Hormigas::dibujar(sf::RenderWindow& ventana){
    ventana.draw(sprite);
}


