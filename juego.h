#ifndef JUEGO_H
#define JUEGO_H
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ray.h"
#include "reyhongo.h"
#include "Puntaje.h"
#include "hormigas.h"
#include <cmath>

class Juego {
private:
    enum EstadoJuego { BIENVENIDA, TUTORIAL, JUGANDO, JEFEFINAL, GAMEOVER, WIN };//Representa las diferentes pantallas o fases del juego: bienvenida, tutorial, nivel principal (laberinto),
    EstadoJuego estado;                  // Estado actual del juego.

    // Ventana y recursos globales
    sf::RenderWindow ventana;            // Ventana principal del juego usando SFML.
    sf::Font fuente;                     // Fuente para textos en el juego.
    sf::Text textoNivel;                 // Texto que muestra el estado o nivel actual.
    bool animando;                       // Bandera que indica si se está ejecutando una animación.

    // Jugador y NPCs
    Ray jugador;                         // Objeto que representa al jugador principal (Ray).
    Reyhongo jefefinal;                  // Objeto que representa al jefe final del juego.
    vector<unique_ptr<Personaje>> npc;   // Vector de punteros únicos a personajes no jugadores (NPCs).
    int nivel;                           // Nivel actual del juego (e.g., 1 para laberinto).

    sf::Clock reloj;                     // Reloj para medir el tiempo entre actualizaciones.

    // Recursos para escena de bienvenida
    sf::Texture fondoBienvenida;         // Textura del fondo de la pantalla de bienvenida.
    sf::Sprite spriteFondoBienvenida;    // Sprite que muestra el fondo de bienvenida.
    sf::Text textoBienvenida;            // Texto que muestra el título en la pantalla de bienvenida.
    sf::RectangleShape botonIniciar;     // Rectángulo que representa el botón de iniciar el juego.
    sf::Text textoBoton;                 // Texto dentro del botón de iniciar.
    sf::Cursor cursorNormal;             // Cursor predeterminado (flecha).
    sf::Cursor cursorMano;               // Cursor con forma de mano (usado en botones interactivos).

    // Recursos para tutorial
    sf::Texture fondoTutorial;           // Textura del fondo de la pantalla de tutorial.
    sf::Sprite spriteFondoTutorial;      // Sprite que muestra el fondo del tutorial.
    std::vector<sf::Sprite> esporas;     // Vector que almacena los sprites de las esporas disparadas.
    std::vector<sf::Vector2f> direccionesEsporas; // Vector con las direcciones de las esporas.
    std::vector<sf::Sprite> ataquesLuz;  // Vector que almacena los sprites de los ataques de luz del jugador.
    std::vector<sf::Vector2f> direccionesAtaques; // Vector con las direcciones de los ataques de luz.
    std::vector<float> dañosAtaques;     // Vector que almacena el daño de cada ataque de luz.
    sf::Texture hongoTexture;            // Textura para los sprites de hongos recolectables.
    std::vector<sf::Sprite> hongos;      // Vector de sprites que representan hongos en el juego.
    std::vector<std::string> fuentesEsporas; // Vector que podría almacenar las fuentes de las esporas (no implementado completamente).
    std::vector<float> rotacionesEsporas; // Vector que almacena las rotaciones de las esporas para animación.

    // Recursos para Gameover
    sf::RectangleShape botonReiniciar;   // Rectángulo que representa el botón de reiniciar en game over.
    sf::Text textoReiniciar;             // Texto dentro del botón de reiniciar.
    sf::Texture fondoGameover;           // Textura del fondo de la pantalla de game over.
    sf::Sprite spriteFondoGameover;      // Sprite que muestra el fondo de game over.

    // Recursos para Winner
    sf::RectangleShape botonVolverJugar; // Rectángulo que representa el botón de volver a jugar en victoria.
    sf::Text textoVolverJugar;           // Texto dentro del botón de volver a jugar.
    sf::RectangleShape botonSalir;       // Rectángulo que representa el botón de salir en victoria.
    sf::Text textoSalir;                 // Texto dentro del botón de salir.
    sf::Texture fondoWinner;             // Textura del fondo de la pantalla de victoria.
    sf::Sprite spriteFondoWinner;        // Sprite que muestra el fondo de victoria.

    // Recursos nivel 1
    sf::Texture fondoLaberinto;          // Textura del fondo del nivel 1 (laberinto).
    sf::Sprite spriteLaberinto;          // Sprite que muestra el fondo del laberinto.
    sf::Vector2f posicionInicialRayNivel1; // Posición inicial del jugador en el nivel 1.
    std::vector<sf::Vector2f> posicionesHormigasNivel1; // Posiciones iniciales de las hormigas en el nivel 1.
    std::vector<sf::Vector2f> posicionesHormigasNoInfectadas; // Posiciones de NPCs no infectados en el nivel 1.
    bool laberintoCompletado;            // Bandera que indica si el laberinto ha sido completado.
    sf::FloatRect finalLaberinto;        // Rectángulo que define el área de finalización del laberinto.
    sf::Image laberintoColision;         // Imagen utilizada para el mapa de colisión del laberinto.
    bool laberintoCargado;               // Bandera que indica si el mapa de colisión del laberinto fue cargado.
    vector<sf::FloatRect> laberintoMuros; // Vector de rectángulos que representan los muros del laberinto.
    vector<sf::RectangleShape> laberintoMurosVisuales; // Vector de rectángulos visuales para mostrar los muros.

    // Recursos último nivel
    sf::Texture fondoJefefinal;          // Textura del fondo del enfrentamiento con el jefe final.
    sf::Sprite spritefondojefefinal;     // Sprite que muestra el fondo del jefe final.
    std::vector<sf::Vector2f> posicionesHormigasJefeFinal; // Posiciones de hormigas en el nivel del jefe.
    int oleadasJefeFinal;                // Número de oleadas completadas contra el jefe.
    std::vector<std::pair<sf::Vector2f, bool>> posicionesOriginalesHongos; // Posiciones y estados de hongos.
    sf::Texture semillaTexture;          // Textura para los sprites de semillas recolectables.
    std::vector<sf::Sprite> semillas;    // Vector de sprites que representan semillas en el juego.
    std::vector<std::pair<sf::Vector2f, bool>> posicionesOriginalesSemillas; // Posiciones y estados de semillas.
    sf::Texture esporaHormigaTexture;    // Textura para esporas disparadas por hormigas.
    sf::Texture esporaReyHongoTexture;   // Textura para esporas disparadas por el rey hongo.
    bool hongosGeneradosDespuesSegundaOleada; // Bandera para controlar generación de hongos tras la segunda oleada.

    // Puntuación
    Puntaje marcadorPuntaje;             // Objeto que gestiona la visualización y actualización del puntaje.

    // Métodos internos
    void procesareventos();              // Maneja los eventos de entrada (teclado, ratón, etc.).
    virtual void actualizar(float deltaTime); // Actualiza la lógica del juego según el tiempo transcurrido.
    void renderizar();                   // Renderiza los elementos gráficos en la ventana.
    void cargarnivel(int nivel);         // Carga el nivel especificado.
    void inicializarTutorial();          // Inicializa los elementos del tutorial.
    void inicializarJefeFinal();         // Inicializa el enfrentamiento con el jefe final.
    bool PuedeMoverse(float Xnew, float Ynew, Personaje* personajeActual); // Verifica si un movimiento es válido.
    void dibujarBarraSalud(sf::RenderWindow& ventana, Personaje& personaje, float vidaMax); // Dibuja la barra de salud.
    void dibujarBarraEnergia(sf::RenderWindow& ventana, Ray& ray, float alturaBase); // Dibuja la barra de energía.
    void inicializarMurosLaberinto();    // Inicializa los muros del laberinto.
    void limpiarEsporas();               // Limpia las esporas de la escena.

    // Inicializa elementos del tutorial

public:
    Juego();                             // Constructor de la clase Juego.
    void iniciar();                      // Inicia la pantalla de bienvenida y el bucle principal del juego.
};

#endif