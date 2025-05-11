#include "juego.h"
#include <iostream>

// Constructor: inicializa ventana, estado y carga nivel inicial
Juego::Juego()
    : ventana(sf::VideoMode(1920, 1080), "The Last Of Ants"),
      jugador(),
      jefefinal(),
      puntaje(0),
      nivel(1),
      estado(BIENVENIDA),
    posicionInicialRayNivel1(100, 200)
{
    ventana.setFramerateLimit(60);
    // Carga fuente una sola vez
    if (!fuente.loadFromFile("pressgothic.ttf")) {
        cout << "Error: no se pudo cargar pressgothic.ttf" << endl;
    }

    // Carga fondo de bienvenida
    if (!fondoBienvenida.loadFromFile("FondoInicio.png")) {
        cout << "Error cargando fondo de bienvenida" << endl;
    }
    spriteFondoBienvenida.setTexture(fondoBienvenida);

    // Configura texto de bienvenida
    textoBienvenida.setFont(fuente);
    textoBienvenida.setString("The \nLast \nOf \nAnts");
    textoBienvenida.setCharacterSize(210);
    textoBienvenida.setFillColor(sf::Color::White);
    textoBienvenida.setPosition(100, 40);
    textoBienvenida.setLineSpacing(0.8f);

    // Configura botón de inicio
    botonIniciar.setSize(sf::Vector2f(400, 200));
    botonIniciar.setFillColor(sf::Color(255, 255, 255, 80));
    botonIniciar.setPosition(1450, 780);
    textoBoton.setFont(fuente);
    textoBoton.setString("INICIAR");
    textoBoton.setCharacterSize(120);
    textoBoton.setFillColor(sf::Color::White);
    textoBoton.setPosition(1519, 810);

    if (!cursorNormal.loadFromSystem(sf::Cursor::Arrow) ||
        !cursorMano.loadFromSystem(sf::Cursor::Hand)) {
        }

    // Carga fondo del tutorial
    if (!fondoTutorial.loadFromFile("fondotutorial.jpg")) {
        cout << "Error cargando fondo del tutorial" << endl;
    }
    spriteFondoTutorial.setTexture(fondoTutorial);

    // Cargar nivel base (puede usarse luego)
    cargarnivel(nivel);

    if (!fondoLaberinto.loadFromFile("laberintocompletos.png")) {
        cout << "Error cargando fondo de laberinto" << endl;
    }
    spriteLaberinto.setTexture(fondoLaberinto);

    posicionesHormigasNivel1 = {
        {1600, 50},  // Hormiga 1
        {1600, 200},  // Hormiga 2
        {1600, 600},  // Hormiga 3
        {1600, 800},  // Hormiga 4
    };

}



// Método que inicia el flujo de bienvenida y luego bucle principal
void Juego::iniciar() {
    while (ventana.isOpen()) {
        procesareventos();
        actualizar();
        renderizar();
        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
        sf::FloatRect bounds = botonIniciar.getGlobalBounds();

        if (bounds.contains(mousePos.x, mousePos.y)) {
            botonIniciar.setFillColor(sf::Color(200, 200, 200, 150)); // Color más oscuro
            ventana.setMouseCursor(cursorMano); // Cambiar cursor a mano
        } else {
            botonIniciar.setFillColor(sf::Color(255, 255, 255, 80)); // Color original
            ventana.setMouseCursor(cursorNormal); // Restaurar cursor normal
        }


    }
}

// Maneja eventos según estado
void Juego::procesareventos() {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            ventana.close();
        }
        if (estado == BIENVENIDA && evento.type == sf::Event::MouseButtonPressed &&
            evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
            if (botonIniciar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                estado = TUTORIAL;
                std::cout << "Estado cambiado a TUTORIAL" << std::endl;
                inicializarTutorial();
            }
        }
        if (estado == TUTORIAL || estado == JUGANDO) {
            if (evento.type == sf::Event::KeyPressed) {
                sf::Vector2f currentPos = jugador.getPosition();
                float newX = currentPos.x;
                float newY = currentPos.y;

                if (evento.key.code == sf::Keyboard::A) {
                    newX -= 5.f;
                    if (PuedeMoverse(newX, newY)) {
                        jugador.mover(-5.f, 0);
                        std::cout << "Moviendo izquierda a (" << newX << ", " << newY << ")" << std::endl;
                    }
                }
                if (evento.key.code == sf::Keyboard::D) {
                    newX += 5.f;
                    if (PuedeMoverse(newX, newY)) {
                        jugador.mover(5.f, 0);
                        std::cout << "Moviendo derecha a (" << newX << ", " << newY << ")" << std::endl;
                    }
                }
                if (evento.key.code == sf::Keyboard::W) {
                    newY -= 5.f;
                    if (PuedeMoverse(newX, newY)) {
                        jugador.mover(0, -5.f);
                        std::cout << "Moviendo arriba a (" << newX << ", " << newY << ")" << std::endl;
                    }
                }
                if (evento.key.code == sf::Keyboard::S) {
                    newY += 5.f;
                    if (PuedeMoverse(newX, newY)) {
                        jugador.mover(0, 5.f);
                        std::cout << "Moviendo abajo a (" << newX << ", " << newY << ")" << std::endl;
                    }
                }
                if (evento.key.code == sf::Keyboard::F) {
                    for (auto it = npc.begin(); it != npc.end();) {
                        Personaje* personaje = it->get();
                        if (jugador.getBounds().intersects(personaje->getBounds())) {
                            std::cout << "Atacando personaje en (" << personaje->getBounds().left << ", "
                                      << personaje->getBounds().top << "), vida antes: " << personaje->getVida() << std::endl;
                            jugador.atacar(personaje);
                            std::cout << "Vida después: " << personaje->getVida() << std::endl;
                            if (personaje->getVida() <= 0) {
                                it = npc.erase(it);
                                puntaje += 10;
                                std::cout << "Personaje eliminado, puntaje: " << puntaje << std::endl;
                            } else {
                                ++it;
                            }
                        } else {
                            ++it;
                        }
                    }
                }
            }
        }
    }
}

void Juego::actualizar() {
    if (estado == TUTORIAL) {
        std::cout << "Actualizando TUTORIAL, NPCs: " << npc.size() << std::endl;
        if (npc.empty()) {
            estado = JUGANDO;
            textoNivel.setString("¡Tutorial completado!");
            std::cout << "Tutorial completado, cambiando a JUGANDO" << std::endl;
            cargarnivel(nivel);
        }
    }
}


// Dibuja elementos según estado
void Juego::renderizar() {
    ventana.clear(sf::Color::Black);
    if (estado == BIENVENIDA) {
        ventana.draw(spriteFondoBienvenida);
        ventana.draw(textoBienvenida);
        ventana.draw(botonIniciar);
        ventana.draw(textoBoton);
        cout << "Dibujando BIENVENIDA" << endl;
    } else if (estado == TUTORIAL) {
        ventana.draw(spriteFondoTutorial);
        cout << "Dibujando fondo TUTORIAL" << endl;
        jugador.dibujar(ventana);
        // Dibujar hormigas primero
        for (auto& hormiga : npc) {
            hormiga->dibujar(ventana);
            std::cout << "Dibujando hormiga en (" << hormiga->getBounds().left << ", "
                      << hormiga->getBounds().top << ")" << std::endl;
        }
        // Dibujar Ray después para que esté detrás
        jugador.dibujar(ventana);
        std::cout << "Dibujando Ray en (" << jugador.getBounds().left << ", "
                  << jugador.getBounds().top << ")" << std::endl;
        ventana.draw(textoNivel);
        std::cout << "Dibujando textoNivel" << std::endl;
    } else if (estado == JUGANDO) {
        if (nivel == 1) {
            ventana.draw(spriteLaberinto);
            std::cout << "Dibujando fondo NIVEL 1 (laberinto)" << std::endl;
        } else {
            ventana.draw(spriteFondoTutorial);
            std::cout << "Dibujando fondo JUGANDO (nivel " << nivel << ")" << std::endl;
        }
        for (auto& personaje : npc) {
            if (personaje) {
                personaje->dibujar(ventana);
                std::cout << "Dibujando personaje en (" << personaje->getBounds().left << ", "
                          << personaje->getBounds().top << ")" << std::endl;
            }
        }
        jugador.dibujar(ventana);
        std::cout << "Dibujando Ray en (" << jugador.getBounds().left << ", "
                  << jugador.getBounds().top << ")" << std::endl;
        ventana.draw(textoNivel);
        std::cout << "Dibujando textoNivel" << std::endl;
    }
    ventana.display();
}

// Inicializa NPCs y posición de jugador para tutorial
void Juego::inicializarTutorial() {
    cout << "Inicializando TUTORIAL" << endl;
    jugador.setScale(1.0f, 1.0f);
    jugador.setPosition(10, 10);
    cout << "Ray dibujado en 100 , 800" << endl;
    npc.clear();
    npc.push_back(make_unique<Hormigas>(true));
    npc.back()->setPosition(1600, 100);
    cout << "Hormiga dibujado en 600 , 800" << endl;
    npc.push_back(make_unique<Hormigas>(true));
    npc.back()->setPosition(1600, 500);
    cout << "Hormiga dibujado en 1000 , 800" << endl;
}

bool Juego::PuedeMoverse(float Xnew, float Ynew) {
    sf::FloatRect rayBounds = jugador.getBounds();
    rayBounds.left = Xnew;
    rayBounds.top = Ynew;

    for (const auto& hormiga : npc) {
        sf::FloatRect hormigaBounds = hormiga->getBounds();
        if (rayBounds.intersects(hormigaBounds)) {
            std::cout << "Colisión detectada con hormiga en (" << hormigaBounds.left << ", "
                      << hormigaBounds.top << ")" << std::endl;
            return false;
        }
    }
    return true;
}


// Resto de métodos (vacios o mínimos)
void Juego::cargartop10() {}

void Juego::cargarnivel(int nivel) {
    npc.clear();
    std::cout << "Cargando nivel " << nivel << ", NPCs limpiados, tamaño: " << npc.size() << std::endl;

    if (nivel == 1) {
        jugador.setPosition(posicionInicialRayNivel1.x, posicionInicialRayNivel1.y);
        jugador.setScale(0.5f, 0.5f); // Escala más pequeña para el laberinto
        std::cout << "Ray reposicionado en (" << posicionInicialRayNivel1.x << ", "
                  << posicionInicialRayNivel1.y << ") con escala (0.5, 0.5) para nivel " << nivel << std::endl;

        // Crear hormigas en posiciones personalizadas con escala más pequeña
        for (size_t i = 0; i < posicionesHormigasNivel1.size() && i < 5; ++i) {
            npc.push_back(std::make_unique<Hormigas>(true));
            npc.back()->setPosition(posicionesHormigasNivel1[i].x, posicionesHormigasNivel1[i].y);
            npc.back()->setScale(0.5f, 0.5f); // Escala más pequeña para el laberinto
            std::cout << "Hormiga infectada creada en (" << posicionesHormigasNivel1[i].x << ", "
                      << posicionesHormigasNivel1[i].y << ") con escala (0.5, 0.5), vida: "
                      << npc.back()->getVida() << std::endl;
        }
    } else {
        // Para otros niveles, usar posiciones predeterminadas y escala normal
        jugador.setPosition(100, 800);
        jugador.setScale(1.0f, 1.0f); // Escala normal
        std::cout << "Ray reposicionado en (100, 800) con escala (1.0, 1.0) para nivel " << nivel << std::endl;
        for (int i = 0; i < nivel * 5; ++i) {
            npc.push_back(std::make_unique<Hormigas>(true));
            npc.back()->setPosition(300 + i * 100, 800);
            npc.back()->setScale(1.0f, 1.0f); // Escala normal

        }
    }

    textoNivel.setString("Nivel " + std::to_string(nivel));
    std::cout << "Nivel " << nivel << " cargado, NPCs: " << npc.size() << std::endl;
}
void Juego::cambiarnivel() {
    nivel++;
    textoNivel.setString("Desafio final");
}