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
    static constexpr float distanciaObjetivo = 500.0f; // Distancia a mantener de Ray
    static constexpr float velocidad = 100.0f; // Velocidad en píxeles por segundo
    public:
    Reyhongo();
    void atacar(Personaje* objetivo) override;
    void controlarHormigas(std::vector<std::unique_ptr<Personaje>>& npc, const std::vector<sf::Vector2f>& posiciones);
    void setPosition(float x, float y) override;
    void setScale(float scaleX, float scaleY) override;
    virtual void dibujar(sf::RenderWindow& ventana);
    sf::FloatRect getBounds() override;
    void moverHaciaJugador(const sf::Vector2f& posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse);
    bool estaEnPosicionDisparo() const;
    void dispararEspora(std::vector<sf::CircleShape>& esporas, std::vector<sf::Vector2f>& direcciones, Personaje* jugador);
};


#endif