#ifndef HORMIGAS_H      
#define HORMIGAS_H      
#include <iostream>
#include "personaje.h"
#include <functional>
#include <valarray>
#include <iostream>
#include <vector>
using namespace std;

class Hormigas : public Personaje{

    private:
    bool infectadas;
	static constexpr float velocidad = 40.0f; // Velocidad en píxeles por segundo
	static constexpr float distanciaObjetivo = 300.0f; // Distancia a mantener de Ray
	bool enPosicionDisparo; // Indica si está a la distancia correcta para disparar
    public:
	Hormigas(bool infectadas, sf::Vector2f posicion = sf::Vector2f(0, 0));
	void moverHaciaJugador(sf::Vector2f posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse);
	void atacar(Personaje* objetivo) override;
    virtual void dibujar(sf::RenderWindow& ventana);
	bool getInfectadas();
	bool estaEnPosicionDisparo() { return enPosicionDisparo; }
	void setPosition(float x,float y)override;
	sf::FloatRect getBounds() override;
	void setScale(float scaleX, float scaleY) override;
};
#endif