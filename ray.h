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
	sf::Texture golpeNormalTexture; // Textura para el golpe normal
	sf::Texture golpeLuzTexture; // Textura para el golpe de luz
	sf::Texture superGolpeLuzTexture; // Textura para el super golpe de luz
	std::vector<sf::Sprite> animacionesAtaques; // Vector para los sprites de las animaciones
	std::vector<float> tiemposAnimaciones;
	bool estaAtacando; // Indicador de si está en modo ataque
	float tiempoAtaqueRestante; // Tiempo restante para la animación de ataque
	sf::Sprite spriteAtaque; // Sprite para la animación de ataque
	sf::Texture textureAtaque;
	sf::Texture textureGolpe;
	sf::Texture textureSuperGolpe;
	sf::Vector2f position;
	bool fWasPressed; // Estado anterior de la tecla F
	bool qWasPressed; // Estado anterior de la tecla Q
	float tiempoEsperaAtaque;
	sf::Clock relojAtaque;
	sf::Texture projectileGolpeTexture; // Textura para el proyectil del golpe de luz
	sf::Texture projectileSuperGolpeTexture; // Textura para el proyectil del supergolpe de luz

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
	bool isAtacando() const { return estaAtacando; }
	void setAtaqueSprite(sf::Texture& texture, float duracion);
	bool isSaltando();
	void actualizar(float deltaTime);
	void setScale(float scaleX, float scaleY) override;
	virtual void dibujar(sf::RenderWindow& ventana);
	void actualizarSalto(float deltaTime);
	int getBastonEnergia() const { return bastonEnergia; } // Método para obtener la energía
	int getMaxEnergia() const { return MAX_ENERGIA; } // Nuevo método para obtener MAX_ENERGIA
	void consumirEnergia(int cantidad); // Método para consumir energía
	void setBastonEnergia(int energia);
	sf::Sprite& getSpriteAtaque();
	sf::Texture& getTextureGolpe();
	sf::Texture& getTextureSuperGolpe();
	void inicializarTexturas();
	void setEstaAtacando(bool atacando);
	void setTiempoAtaqueRestante(float tiempo);
	void procesarAtaque(sf::Keyboard::Key key, Personaje* enemigo);
	sf::Texture& getProjectileGolpeTexture();
	sf::Texture& getProjectileSuperGolpeTexture();
};
#endif