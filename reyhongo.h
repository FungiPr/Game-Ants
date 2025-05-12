#ifndef REYHONGO_H      
#define REYHONGO_H      
#include <iostream>
#include "personaje.h" 
#include <iostream>
#include <vector>
using namespace std;

class Reyhongo : public Personaje{   
    private:
    int energiaHormigas;
    public:
    Reyhongo();
    void atacar(Personaje* objetivo) override;
    void controlarhormigas(vector<Personaje*>& npc);
    virtual void dibujar(sf::RenderWindow& ventana);
    sf::FloatRect getBounds()  override;
};


#endif