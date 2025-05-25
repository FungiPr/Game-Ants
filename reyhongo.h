#ifndef REYHONGO_H      
#define REYHONGO_H      
#include <iostream>
#include "personaje.h" 
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
using namespace std;

class Reyhongo : public Personaje{   
    private:
    int energiaHormigas;
    float tiempoDisparo;
    float intervaloDisparo;
    sf::Clock relojDisparo;
    bool enPosicionDisparo;
    float distanciaObjetivo = 500.0f; // Distancia a mantener de Ray
    float velocidad = 100.0f; // Velocidad en píxeles por segundo
    std::vector<sf::Texture> texturasAnimacion; // 0: pie derecho, 1: pie izquierdo
    int frameActual; // Nuevo: frame actual de la animación
    float tiempoFrame; // Nuevo: temporizador para cambiar frames
    bool enMovimiento; // Nuevo: indica si está en movimiento
    float duracionFrame = 0.2f; // Tiempo por frame en segundos
    sf::Texture texturaAtaque;
    bool estaAtacando; // Nuevo: indica si está atacando
    float tiempoAtaqueRestante; // Nuevo: temporizador para la animación de ataque
    float duracionAtaque = 0.5f; // Nuevo: duración de la animación de ataque
    public:
    Reyhongo();
    void atacar(Personaje* objetivo) override;
    void controlarHormigas(std::vector<std::unique_ptr<Personaje>>& npc,  std::vector<sf::Vector2f>& posiciones);
    void setPosition(float x, float y) override;
    void setScale(float scaleX, float scaleY) override;
    void dibujar(sf::RenderWindow& ventana)override;
    sf::FloatRect getBounds() override;
    void actualizar(float deltaTime);
    void moverHaciaJugador(sf::Vector2f& posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse);
    bool estaEnPosicionDisparo() ;
    void dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador, sf::Texture& esporaTexture);
};


#endif