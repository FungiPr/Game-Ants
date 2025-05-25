#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
using namespace std;

class Personaje {
protected:
	sf::Sprite sprite;
	static map<string, sf::Texture> texturasCompartidas; //  mapa estático de texturas
	int vidamaxima;
	int vida_actual;
	bool vivo;
public:
	float posicion_x;
	float posicion_y;
	Personaje(const string& archivo_textura, int vidaMax);
	virtual ~Personaje();
	virtual void dibujar(sf::RenderWindow& ventana);
	void recibirdano(int cantidad);
	virtual void atacar(Personaje* objetivo) = 0;
	int getVida();
	virtual sf::FloatRect getBounds() = 0;
	virtual void setPosition(float x, float y) = 0;
	sf::Vector2f getPosition()  { return sprite.getPosition(); }
	virtual void setScale(float scaleX, float scaleY) = 0;
	void restablecerVida();
};
