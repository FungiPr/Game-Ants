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
	void atacar(Personaje* objetivo) override;
    virtual void dibujar(sf::RenderWindow& ventana);
	bool getInfectadas();
	void setPosition(float x,float y);
	sf::FloatRect getBounds() override;
	void setScale(float scaleX, float scaleY);
};
#endif