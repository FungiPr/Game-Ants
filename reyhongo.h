#ifndef REYHONGO_H
#define REYHONGO_H
#include <iostream>
#include "personaje.h"
#include <iostream>
#include <vector>
#include <memory>
#include <functional>


class Reyhongo : public Personaje{
    private:
    int energiaHormigas; // Energía que el Reyhongo usa para controlar a las hormigas.
    float tiempoDisparo; // Tiempo transcurrido desde el último disparo.
    float intervaloDisparo; // Intervalo de tiempo entre disparos.
    sf::Clock relojDisparo; // Reloj para medir el tiempo entre disparos.
    bool enPosicionDisparo; // Indica si está a la distancia correcta para disparar.
    float distanciaObjetivo = 500.0f; // Distancia a mantener de Ray.
    float velocidad = 100.0f; // Velocidad en píxeles por segundo.
    std::vector<sf::Texture> texturasAnimacion; // 0: pie derecho, 1: pie izquierdo. Texturas para la animación de caminar.
    int frameActual; // Nuevo: frame actual de la animación.
    float tiempoFrame; // Nuevo: temporizador para cambiar frames.
    bool enMovimiento; // Nuevo: indica si está en movimiento.
    float duracionFrame = 0.2f; // Tiempo por frame en segundos.
    sf::Texture texturaAtaque; // Textura para la animación de ataque.
    bool estaAtacando; // Nuevo: indica si está atacando.
    float tiempoAtaqueRestante; // Nuevo: temporizador para la animación de ataque.
    float duracionAtaque = 0.5f; // Nuevo: duración de la animación de ataque.

    public:
    // Constructor de la clase Reyhongo.
    Reyhongo();

    // Realiza un ataque contra un objetivo.
    void atacar(Personaje* objetivo) override;

    // Controla y posiciona a las hormigas bajo el mando del Reyhongo.
    void controlarHormigas(std::vector<std::unique_ptr<Personaje>>& npc, std::vector<sf::Vector2f>& posiciones);

    // Establece la posición del Reyhongo.
    void setPosition(float x, float y) override;

    // Escala el sprite del Reyhongo.
    void setScale(float scaleX, float scaleY) override;

    // Dibuja al Reyhongo en la ventana.
    void dibujar(sf::RenderWindow& ventana) override;

    // Obtiene el área de colisión del Reyhongo.
    sf::FloatRect getBounds() override;

    // Actualiza el estado del Reyhongo, incluyendo animaciones.
    void actualizar(float deltaTime);

    // Mueve al Reyhongo hacia el jugador manteniendo una distancia objetivo.
    void moverHaciaJugador(sf::Vector2f& posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse);

    // Verifica si el Reyhongo está en posición de disparo.
    bool estaEnPosicionDisparo();

    // Dispara una espora hacia el jugador.
    void dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador, sf::Texture& esporaTexture);
};

#endif