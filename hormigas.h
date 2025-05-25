#ifndef HORMIGAS_H      
#define HORMIGAS_H      
#include <iostream>
#include "personaje.h"
#include <functional>
#include <valarray>
#include <iostream>
#include <vector>
using namespace std;

class Hormigas : public Personaje {
private:
	bool infectadas;
	float velocidad = 50.0f; // Velocidad en píxeles por segundo
	float distanciaObjetivo = 400.0f; // Distancia a mantener de Ray
	bool enPosicionDisparo; // Indica si está a la distancia correcta para disparar
	sf::Clock relojDisparo;
	std::vector<sf::Texture> texturasAnimacion; // 0: pie derecho, 1: pie izquierdo
	int frameActual;
	float tiempoFrame;
	bool enMovimiento;
	 float duracionFrame = 0.2f; // Tiempo por frame en segundos
	sf::Texture texturaAtaque;
	bool estaAtacando; // Nuevo estado para la animación de ataque
	float tiempoAtaqueRestante; // Temporizador para la animación de ataque
	 float duracionAtaque = 0.3f; // Duración de la animación de ataque
    public:
	Hormigas(bool infectadas, sf::Vector2f posicion = sf::Vector2f(0, 0));
	void moverHaciaJugador(sf::Vector2f posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse);
	void atacar(Personaje* objetivo) override;
    void dibujar(sf::RenderWindow& ventana) override;
	bool getInfectadas();
	bool estaEnPosicionDisparo() { return enPosicionDisparo; }
	void setPosition(float x,float y)override;
	sf::FloatRect getBounds() override;
	void setScale(float scaleX, float scaleY) override;
	void dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador, sf::Texture& esporaTexture);
	void actualizar(float deltaTime); // Nuevo método para actualizar la animación
};
#endif