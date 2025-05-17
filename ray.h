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
	std::vector<sf::Texture> texturasAnimacion; // Vector para las texturas de animación
	int frameActual; // Índice del frame actual (0: base, 1: pie derecho, 2: pie izquierdo)
	float tiempoFrame; // Temporizador para controlar el cambio de frames
	bool enMovimiento; // Indica si Ray está en movimiento
	static constexpr float duracionFrame = 0.1f; // Duración de cada frame en segundos
	static constexpr float velocidad = 100.0f;
	public:
	Ray();
	void mover(float dx,float dy) override;
	void saltar();
	void atacar(Personaje* objetivo) override;
	void golpedeLuz(Personaje* enemigo);
	void supergolpedeLuz(Personaje* enemigo);
	void recolectarsemilla();
	void recolectarHongo();
	void setPosition(float x,float y) override;
	sf::FloatRect getBounds()  override;
	sf::Vector2f getPosition();
	void setScale(float scaleX, float scaleY) override;
	virtual void dibujar(sf::RenderWindow& ventana);
};
#endif