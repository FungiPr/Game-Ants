// juego.h
#ifndef JUEGO_H
#define JUEGO_H
#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ray.h"
#include "reyhongo.h"
#include "hormigas.h"
#include <cmath>
using namespace std;

class Juego {
private:
	// Estados del juego: bienvenida, tutorial, juego principal
	enum EstadoJuego { BIENVENIDA, TUTORIAL, JUGANDO,JEFEFINAL, GAMEOVER };
	EstadoJuego estado;                  // Estado actual

	// Ventana y recursos globales
	sf::RenderWindow ventana;
	sf::Font fuente;
	sf::Text textoNivel;
	bool animando = false;


	// Jugador y NPCs
	Ray jugador;
	Reyhongo jefefinal;
	vector<unique_ptr<Personaje>> npc;
	int puntaje;
	int nivel;
	sf::Clock reloj;

	// Recursos para escena de bienvenida
	sf::Texture fondoBienvenida;
	sf::Sprite spriteFondoBienvenida;
	sf::Text textoBienvenida;
	sf::RectangleShape botonIniciar;
	sf::Text textoBoton;
	sf::Cursor cursorNormal;
	sf::Cursor cursorMano;


	// Recursos para tutorial
	sf::Texture fondoTutorial;
	sf::Sprite spriteFondoTutorial;

	//recursos nivel 1

	sf::Texture fondoLaberinto; // Nuevo: textura para el laberinto
	sf::Sprite spriteLaberinto;
	sf::Vector2f posicionInicialRayNivel1; // Nuevo: posición inicial de Ray para nivel 1
	std::vector<sf::Vector2f> posicionesHormigasNivel1;
	bool laberintoCompletado;
	sf::FloatRect finalLaberinto; // Área del final del laberinto

	//recursos ultimo nivel
	sf::Texture fondoJefefinal; // Nuevo: textura para el laberinto
	sf::Sprite spritefondojefefinal;
	std::vector<sf::Vector2f> posicionesHormigasJefeFinal;
	int oleadasJefeFinal;

	// Métodos internos
	void procesareventos();
	void actualizar(float deltaTime);
	void renderizar();
	void cargartop10();
	void guardarpuntaje(string& nombre);
	void cargarnivel(int nivel);
	void cambiarnivel();
	void inicializarTutorial();
	void inicializarJefeFinal();
	bool PuedeMoverse(float Xnew, float Ynew, Personaje* personajeActual);



	// Inicializa elementos del tutorial

public:
	Juego();
	void iniciar();               // Ejecuta la pantalla de bienvenida y bucle principal
};

#endif
