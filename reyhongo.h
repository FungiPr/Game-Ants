#ifndef REYHONGO_H      
#define REYHONGO_H      
#include <iostream>
#include "personaje.h" 
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Reyhongo : public Personaje{   
    private:
    int energiaHormigas;
    public:
    Reyhongo();
    void atacar(Personaje* objetivo) override;
    void controlarHormigas(std::vector<std::unique_ptr<Personaje>>& npc, const std::vector<sf::Vector2f>& posiciones);
    void setPosition(float x,float y)override;
    void setScale(float scaleX, float scaleY) override;
    virtual void dibujar(sf::RenderWindow& ventana);
    sf::FloatRect getBounds()  override;
};


#endif