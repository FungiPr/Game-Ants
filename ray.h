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
	static constexpr float velocidad = 200.0f;
	bool estaSaltando;
	float alturaSalto; // Altura máxima del salto (píxeles)
	sf::Clock relojSalto; // Temporizador para la duración del salto
	sf::Texture texturaSalto; // Textura para el salto (RaySaltando.png)
	sf::Vector2f posicionOriginal; // Posición antes del salto (para animación)
	static constexpr int MAX_ENERGIA = 100; // Declaración de la constante dentro de la clase

	public:
	Ray();
	void mover(float dx,float dy) override;
	void saltar();
	void atacar(Personaje* objetivo) override;
	void golpedeLuz(Personaje* enemigo);
	void supergolpedeLuz(Personaje* enemigo);
	void recolectarsemilla();
	void recolectarHongo();
	void aumentarVida(int vida);
	void setPosition(float x,float y) override;
	sf::FloatRect getBounds()  override;

	bool isSaltando();
	void setScale(float scaleX, float scaleY) override;
	virtual void dibujar(sf::RenderWindow& ventana);
	void actualizarSalto(float deltaTime);
	int getBastonEnergia() const { return bastonEnergia; } // Método para obtener la energía
	int getMaxEnergia() const { return MAX_ENERGIA; } // Nuevo método para obtener MAX_ENERGIA
	void consumirEnergia(int cantidad); // Método para consumir energía
	void setBastonEnergia(int energia);
};
#endif