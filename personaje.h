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
	float posicion_x;
	float posicion_y;
	bool vivo;

public:
	Personaje(const string& archivo_textura, int vidaMax);
	virtual ~Personaje();

	void dibujar(sf::RenderWindow& ventana);
	void mover(float dx, float dy);
	void recibirdano(int cantidad);
	void atacar(Personaje* objetivo);
	int getvida();
};
