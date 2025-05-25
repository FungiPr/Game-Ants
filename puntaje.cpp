#include "Puntaje.h"

Puntaje::Puntaje(sf::Font& fuente) : puntaje(0) {
    // Configurar el rectángulo marrón
    fondo.setSize(sf::Vector2f(140, 60));
    fondo.setFillColor(sf::Color(21, 67, 96)); // Color azul
    fondo.setPosition(1750, 20); // Esquina superior derecha (1920 - 300 - 10 de margen)

    // Configurar el texto del puntaje
    textoPuntaje.setFont(fuente);
    textoPuntaje.setCharacterSize(30);
    textoPuntaje.setFillColor(sf::Color::White);
    textoPuntaje.setPosition(1769, 32); // Centrado dentro del rectángulo
    textoPuntaje.setString("Puntaje: 0");
}

void Puntaje::setPuntaje(int nuevoPuntaje) {
    puntaje = nuevoPuntaje;
    textoPuntaje.setString("Puntaje: " + std::to_string(puntaje));
}

void Puntaje::incrementarPuntaje(int incremento) {
    puntaje += incremento;
    textoPuntaje.setString("Puntaje: " + std::to_string(puntaje));
}

void Puntaje::dibujar(sf::RenderWindow& ventana, int nivel, sf::RenderStates estados) {
    // Solo dibujar a partir del nivel 1
    if (nivel >= 1) {
        ventana.draw(fondo, estados);
        ventana.draw(textoPuntaje, estados);
    }
}