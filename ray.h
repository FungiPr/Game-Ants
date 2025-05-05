#ifndef RAY_H
#define RAY_H
#include <iostream>
#include "personaje.h" 
#include <iostream>
using namespace std;

class Ray : public Personaje{   
    private:
    int bastonEnergia;
	int numeroVidas;
	int SemillasRecolectadas;
	int hongosbiolumicentesrecolectados;
    public:
	Ray();
	void Caminar(float dx,float dy);
	void saltar();
	void golpear(Personaje* enemigo);
	void golpedeLuz(Personaje* enemigo);
	void supergolpedeLuz(Personaje* enemigo);
	void recolectarsemilla();
	void recolectarHongo();
	void setPosition(float x,float y);
	sf::FloatRect getBounds();
	virtual void dibujar(sf::RenderWindow& ventana);
};
#endif