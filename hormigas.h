#ifndef HORMIGAS_H      
#define HORMIGAS_H      
#include <iostream>
#include "personaje.h" 
#include <iostream>
#include <vector>
using namespace std;

class Hormigas : public Personaje{

    private:
    bool infectadas;
    public:
	Hormigas(bool infectadas);
	void ataque(Personaje* objetivo);
    virtual void dibujar(sf::RenderWindow& ventana);
	bool getInfectadas();
	void setPosition(float x,float y);
	sf::FloatRect getBounds();
};
#endif