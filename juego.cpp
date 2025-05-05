#include "juego.h"
#include <iostream>

// Constructor: inicializa ventana, estado y carga nivel inicial
Juego::Juego()
    : ventana(sf::VideoMode(1920, 1080), "The Last Of Ants"),
      jugador(),
      jefefinal(),
      puntaje(0),
      nivel(1),
      estado(BIENVENIDA)
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
    if (!fondoTutorial.loadFromFile("tierrajuegoo.png")) {
        cout << "Error cargando fondo del tutorial" << endl;
    }
    spriteFondoTutorial.setTexture(fondoTutorial);

    // Cargar nivel base (puede usarse luego)
    cargarnivel(nivel);
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
        // Manejo de clic para pasar de BIENVENIDA a TUTORIAL
        if (estado == BIENVENIDA && evento.type == sf::Event::MouseButtonPressed
            && evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
            if (botonIniciar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                estado = TUTORIAL;
                std::cout << "Estado cambiado a TUTORIAL" << std::endl;
                inicializarTutorial();
            }
        }
    }
    // Movimiento del jugador solo en tutorial o juego
    if (estado == TUTORIAL || estado == JUGANDO) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) jugador.Caminar(-5.f, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) jugador.Caminar(5.f, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) jugador.Caminar(0, -5.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) jugador.Caminar(0, 5.f);
    }
}

// Lógica de colisiones y transición de estados
void Juego::actualizar() {
    if (estado == TUTORIAL) {
        for (auto it = npc.begin(); it != npc.end();) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
                && jugador.getBounds().intersects((*it)->getBounds())) {
                it = npc.erase(it);
                cout << "Hormiga eliminada.." <<endl;
            } else {
                ++it;
            }
        }
        if (npc.empty()) {
            estado = JUGANDO;
            textoNivel.setString("¡Tutorial completado!");
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
        cout << "Dibujando Ray" << endl;
        for (auto& hormiga : npc) {
            hormiga->dibujar(ventana);
            cout << "Dibujando hormigas" <<endl;
        }
    } else {
        // Estado de juego principal: dibuja jugador, NPCs, HUD, etc.
        jugador.dibujar(ventana);
        ventana.draw(textoNivel);
        // ... más renderizado
    }
    ventana.display();
}

// Inicializa NPCs y posición de jugador para tutorial
void Juego::inicializarTutorial() {
    cout << "Inicializando TUTORIAL" << endl;
    jugador.setPosition(100, 800);
    cout << "Ray dibujado en 100 , 800" << endl;
    npc.clear();
    npc.push_back(std::make_unique<Hormigas>(false));
    npc.back()->setPosition(600, 800);
    cout << "Ray dibujado en 600 , 800" << endl;
    npc.push_back(std::make_unique<Hormigas>(true));
    npc.back()->setPosition(1000, 800);
    cout << "Ray dibujado en 1000 , 800" << endl;
}

// Resto de métodos (vacios o mínimos)
void Juego::cargartop10() {}

void Juego::cargarnivel(int nivel) {
    npc.clear();
    for (int i = 0; i < nivel * 5; ++i) {
        npc.push_back(std::make_unique<Hormigas>(true));
        npc.back()->setPosition(300 + i * 100, 800);
    }
}

void Juego::cambiarnivel() {
    nivel++;
    textoNivel.setString("Desafio final");
}