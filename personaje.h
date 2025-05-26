#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
using namespace std;

class Personaje {
protected:
	sf::Sprite sprite; // Sprite que representa al personaje en la pantalla.
	static map<string, sf::Texture> texturasCompartidas; // Mapa estático de texturas compartidas entre personajes.
	int vidamaxima; // Vida máxima del personaje.
	int vida_actual; // Vida actual del personaje.
	bool vivo; // Indica si el personaje está vivo.
public:
	float posicion_x; // Coordenada X de la posición del personaje.
	float posicion_y; // Coordenada Y de la posición del personaje.

	// Constructor que inicializa el personaje con una textura y vida máxima.
	Personaje(const string& archivo_textura, int vidaMax);

	// Destructor virtual para permitir una correcta destrucción en clases derivadas.
	virtual ~Personaje();

	// Dibuja el sprite del personaje en la ventana.
	virtual void dibujar(sf::RenderWindow& ventana);

	// Reduce la vida del personaje según la cantidad de daño recibida.
	void recibirdano(int cantidad);

	// Metodo puro virtual para definir el ataque a otro personaje.
	virtual void atacar(Personaje* objetivo) = 0;

	// Obtiene la vida actual del personaje.
	int getVida();

	// Metodo puro virtual para obtener el área de colisión del personaje.
	virtual sf::FloatRect getBounds() = 0;

	// Metodo puro virtual para establecer la posición del personaje.
	virtual void setPosition(float x, float y) = 0;

	// Obtiene la posición actual del sprite del personaje.
	sf::Vector2f getPosition() { return sprite.getPosition(); }

	// Metodo puro virtual para escalar el sprite del personaje.
	virtual void setScale(float scaleX, float scaleY) = 0;

	// Restablece la vida del personaje a su valor máximo.
	void restablecerVida();

};