#ifndef PUNTAJE_H
#define PUNTAJE_H
#include <SFML/Graphics.hpp>
class Puntaje {
private:
    int puntaje; // Almacena el puntaje actual
    sf::RectangleShape fondo; // Rectángulo marrón de fondo
    sf::Text textoPuntaje; // Texto que muestra el puntaje
public:
    Puntaje(sf::Font& fuente); // Constructor que recibe la fuente para el texto
    void setPuntaje(int nuevoPuntaje); // Actualiza el puntaje
    void incrementarPuntaje(int incremento); // Incrementa el puntaje
    void dibujar(sf::RenderWindow& ventana, int nivel, sf::RenderStates estados = sf::RenderStates::Default); // Dibuja el rectángulo y texto del puntaje
};

#endif