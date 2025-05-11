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
	void mover(float dx,float dy) override;
	void saltar();
	void atacar(Personaje* objetivo) override;
	void golpedeLuz(Personaje* enemigo);
	void supergolpedeLuz(Personaje* enemigo);
	void recolectarsemilla();
	void recolectarHongo();
	void setPosition(float x,float y);
	sf::FloatRect getBounds()  override;
	sf::Vector2f getPosition();
	void setScale(float scaleX, float scaleY);
	virtual void dibujar(sf::RenderWindow& ventana);
};
#endif