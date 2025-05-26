#ifndef RAY_H
#define RAY_H
#include <iostream>
#include "personaje.h"
#include <iostream>


class Ray : public Personaje {
private:
    int bastonEnergia;                           // Energía del bastón de Ray, usada para ataques especiales.
    int numeroVidas;                             // Número de vidas restantes de Ray (no implementado completamente).
    int SemillasRecolectadas;                    // Contador de semillas recolectadas por Ray.
    int hongosbiolumicentesrecolectados;         // Contador de hongos bioluminiscentes recolectados.
    std::vector<sf::Texture> texturasAnimacion;  // Vector que almacena las texturas para la animación de caminar.
    int frameActual;                             // Índice del frame actual en la animación (0: base, 1: pie derecho, 2: pie izquierdo).
    float tiempoFrame;                           // Temporizador para controlar el cambio de frames en la animación.
    bool enMovimiento;                           // Bandera que indica si Ray está en movimiento.
    float duracionFrame = 0.1f;                  // Duración de cada frame de animación, en segundos.
    float velocidad = 200.0f;                    // Velocidad de movimiento de Ray, en píxeles por segundo.
    bool estaSaltando;                           // Bandera que indica si Ray está saltando.
    float alturaSalto;                           // Altura máxima del salto, en píxeles.
    sf::Clock relojSalto;                        // Reloj para medir el tiempo durante el salto.
    sf::Texture texturaSalto;                    // Textura para la animación de salto (RaySaltando.png).
    sf::Vector2f posicionOriginal;               // Posición de Ray antes de iniciar el salto, para animación.
    int MAX_ENERGIA = 100;                       // Constante que define la energía máxima del bastón.
    sf::Texture golpeNormalTexture;              // Textura para el ataque normal de Ray.
    sf::Texture golpeLuzTexture;                 // Textura para el ataque de luz de Ray.
    sf::Texture superGolpeLuzTexture;            // Textura para el super ataque de luz de Ray.
    std::vector<sf::Sprite> animacionesAtaques;  // Vector que almacena los sprites de las animaciones de ataque.
    std::vector<float> tiemposAnimaciones;       // Vector que almacena los tiempos de las animaciones de ataque.
    bool estaAtacando;                           // Bandera que indica si Ray está en modo ataque.
    float tiempoAtaqueRestante;                  // Tiempo restante para la animación de ataque.
    sf::Sprite spriteAtaque;                     // Sprite usado para la animación de ataque.
    sf::Texture textureAtaque;                   // Textura general para animaciones de ataque.
    sf::Texture textureGolpe;                    // Textura específica para el golpe normal.
    sf::Texture textureSuperGolpe;               // Textura específica para el super golpe de luz.
    sf::Vector2f position;                       // Posición actual de Ray en el juego.
    bool fWasPressed;                            // Estado anterior de la tecla F (para detectar pulsaciones únicas).
    bool qWasPressed;                            // Estado anterior de la tecla Q (para detectar pulsaciones únicas).
    float tiempoEsperaAtaque;                    // Tiempo de espera entre ataques consecutivos.
    sf::Clock relojAtaque;                       // Reloj para medir el tiempo entre ataques.
    sf::Texture projectileGolpeTexture;          // Textura para el proyectil del golpe de luz.
    sf::Texture projectileSuperGolpeTexture;     // Textura para el proyectil del super golpe de luz.

public:
    // Inicializa las propiedades del personaje, como energía, vidas, animaciones, y texturas.
    Ray();

    //  Mueve a Ray en la dirección especificada.
    void mover(float dx, float dy);

    //Activa la animación de salto y ajusta las variables necesarias.
    void saltar();


    //  Realiza un ataque normal contra un objetivo.
    void atacar(Personaje* objetivo) override;

    //  Realiza un ataque de luz contra un enemigo.
    void golpedeLuz(Personaje* enemigo);

    // Realiza un super ataque de luz contra un enemigo.
    void supergolpedeLuz(Personaje* enemigo);

    // Incrementa el contador de semillas recolectadas.
    void recolectarsemilla();

    // Incrementa el contador de hongos bioluminiscentes recolectados.
    void recolectarHongo();

    // Aumenta la vida de Ray.
    void aumentarVida(int vida);

    // Establece la posición de Ray.
    void setPosition(float x, float y) override;

    // Obtiene el área de colisión de Ray.
    sf::FloatRect getBounds() override;

    // Verifica si Ray está atacando.
    bool isAtacando() { return estaAtacando; }

    // Establece la textura y duración de la animación de ataque.
    void setAtaqueSprite(sf::Texture& texture, float duracion);

    // Verifica si Ray está saltando.
    bool isSaltando();

    // Actualiza el estado de Ray.
    void actualizar(float deltaTime);

    //Escala el sprite de Ray.
    void setScale(float scaleX, float scaleY) override;

    //Dibuja a Ray en la ventana.
    void dibujar(sf::RenderWindow& ventana) override;

    // Actualiza la animación de salto.
    void actualizarSalto(float deltaTime);

    // Obtiene la energía actual del bastón.
    int getBastonEnergia() { return bastonEnergia; }

    // Obtiene la energía máxima del bastón.
    int getMaxEnergia() { return MAX_ENERGIA; }

    // Reduce la energía del bastón.
    void consumirEnergia(int cantidad);

    //Establece la energía del bastón.
    void setBastonEnergia(int energia);

    // Obtiene el sprite de ataque de Ray.
    sf::Sprite& getSpriteAtaque();

    // Obtiene la textura del golpe normal.
    sf::Texture& getTextureGolpe();

    // Obtiene la textura del super golpe de luz.
    sf::Texture& getTextureSuperGolpe();

    // Inicializa las texturas de animación y ataque.
    void inicializarTexturas();

    //Establece el estado de ataque de Ray.
    void setEstaAtacando(bool atacando);

    // Establece el tiempo restante de la animación de ataque.
    void setTiempoAtaqueRestante(float tiempo);

    // Procesa un ataque según la tecla presionada.
    void procesarAtaque(sf::Keyboard::Key key, Personaje* enemigo);

    // Obtiene la textura del proyectil del golpe de luz.
    sf::Texture& getProjectileGolpeTexture();

    // Obtiene la textura del proyectil del super golpe de luz.
    sf::Texture& getProjectileSuperGolpeTexture();
};
#endif