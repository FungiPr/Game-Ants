#ifndef HORMIGAS_H
#define HORMIGAS_H
#include <iostream>
#include "personaje.h"
#include <functional>
#include <valarray>
#include <iostream>
#include <vector>


class Hormigas : public Personaje {
private:
    bool infectadas; // Indica si la hormiga está infectada o no.
    float velocidad = 50.0f; // Velocidad en píxeles por segundo.
    float distanciaObjetivo = 400.0f; // Distancia a mantener de Ray para disparar.
    bool enPosicionDisparo; // Indica si está a la distancia correcta para disparar.
    sf::Clock relojDisparo; // Reloj para medir el tiempo entre disparos.
    std::vector<sf::Texture> texturasAnimacion; // 0: pie derecho, 1: pie izquierdo. Texturas para la animación de caminar.
    int frameActual; // Índice del frame actual en la animación.
    float tiempoFrame; // Temporizador para controlar el cambio de frames en la animación.
    bool enMovimiento; // Bandera que indica si la hormiga está en movimiento.
    float duracionFrame = 0.2f; // Tiempo por frame en segundos.
    sf::Texture texturaAtaque; // Textura para la animación de ataque.
    bool estaAtacando; // Nuevo estado para la animación de ataque.
    float tiempoAtaqueRestante; // Temporizador para la animación de ataque.
    float duracionAtaque = 0.3f; // Duración de la animación de ataque.

public:
    // Constructor de la clase Hormigas, inicializa si está infectada y su posición.
    Hormigas(bool infectadas, sf::Vector2f posicion = sf::Vector2f(0, 0));

    // Mueve la hormiga hacia el jugador manteniendo una distancia objetivo.
    void moverHaciaJugador(sf::Vector2f posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse);

    // Realiza un ataque contra un objetivo.
    void atacar(Personaje* objetivo) override;

    // Dibuja la hormiga en la ventana.
    void dibujar(sf::RenderWindow& ventana) override;

    // Devuelve si la hormiga está infectada.
    bool getInfectadas();

    // Verifica si la hormiga está en posición de disparo.
    bool estaEnPosicionDisparo() { return enPosicionDisparo; }

    // Establece la posición de la hormiga.
    void setPosition(float x, float y) override;

    // Obtiene el área de colisión de la hormiga.
    sf::FloatRect getBounds() override;

    // Escala el sprite de la hormiga.
    void setScale(float scaleX, float scaleY) override;

    // Dispara una espora hacia el jugador.
    void dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador, sf::Texture& esporaTexture);

    // Actualiza el estado de la hormiga, incluyendo animaciones.
    void actualizar(float deltaTime);
};
#endif