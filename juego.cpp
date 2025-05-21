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
    oleadasJefeFinal(0),
    posicionInicialRayNivel1(5, 20),
    laberintoCompletado(false),
      finalLaberinto(1180, 400, 200, 200), // Rectángulo de 200x200
    laberintoCargado(false)
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
    if (!fondoTutorial.loadFromFile("fondotutorialoficial.png")) {
        cout << "Error cargando fondo del tutorial" << endl;
    }
    spriteFondoTutorial.setTexture(fondoTutorial);

    // Cargar nivel 1 (puede usarse luego)
    cargarnivel(nivel);

    // Cargar mapa de colisión del laberinto
    if (!laberintoColision.loadFromFile("laberintocompletos.png")) {
        cout << "Error cargando imagen de colisión del laberinto" << endl;
    } else {
        laberintoCargado = true;
        cout << "Mapa de colisión del laberinto cargado exitosamente" << endl;
    }

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

    posicionesHormigasJefeFinal = {
        {1000, 100},  // Hormiga 1
        {1000, 400},  // Hormiga 2S
        {1000, 700}, // Hormiga 3

    };

    //Cargar ultimo nivel
    if (!fondoJefefinal.loadFromFile("fondoJefefinalmorado.jpg")) {
        cout << "Error cargando fondo de laberinto" << endl;
    }
    spritefondojefefinal.setTexture(fondoJefefinal);
    inicializarMurosLaberinto(); // Inicializar los muros
}

void Juego::inicializarMurosLaberinto() {
    // Limpiar los muros anteriores
    laberintoMuros.clear();

    // Definir los muros ajustados con más espacio para los caminos
    laberintoMuros.push_back(sf::FloatRect(0, 50, 900, 50));      // Pared superior izquierda
    laberintoMuros.push_back(sf::FloatRect(100, 50, 50, 850));    // Pared vertical izquierda
    laberintoMuros.push_back(sf::FloatRect(200, 400, 600, 50));   // Pared central
    laberintoMuros.push_back(sf::FloatRect(500, 800, 900, 50));   // Pared inferior derecha
    laberintoMuros.push_back(sf::FloatRect(1200, 100, 50, 600));  // Pared vertical derecha
    laberintoMuros.push_back(sf::FloatRect(300, 200, 50, 300));   // Pared curva adicional

    // Crear visualización de los muros
    laberintoMurosVisuales.clear();
    for (const auto& muro : laberintoMuros) {
        sf::RectangleShape rect;
        rect.setPosition(muro.left, muro.top);
        rect.setSize(sf::Vector2f(muro.width, muro.height));
        rect.setFillColor(sf::Color(255, 0, 0, 100)); // Rojo semitransparente
        laberintoMurosVisuales.push_back(rect);
    }

    cout << "Muros del laberinto inicializados: " << laberintoMuros.size() << " muros" << endl;
    for (const auto& muro : laberintoMuros) {
        cout << "Muro: [" << muro.left << ", " << muro.top << ", " << muro.width << ", " << muro.height << "]" << endl;
    }
}



// Método que inicia el flujo de bienvenida y luego bucle principal
void Juego::iniciar() {
    while (ventana.isOpen()) {
        float deltaTime = reloj.restart().asSeconds();
        procesareventos();
        actualizar(deltaTime);
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
        if (estado == TUTORIAL || estado == JUGANDO || estado == JEFEFINAL) {
            if (evento.type == sf::Event::KeyPressed) {
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
                    if (estado == JEFEFINAL && jugador.getBounds().intersects(jefefinal.getBounds())) {
                        std::cout << "Atacando Rey Hongo en (" << jefefinal.getBounds().left << ", "
                                  << jefefinal.getBounds().top << "), vida antes: " << jefefinal.getVida() << std::endl;
                        jugador.atacar(&jefefinal);
                        std::cout << "Vida después: " << jefefinal.getVida() << std::endl;
                        if (jefefinal.getVida() <= 0) {
                            estado = GAMEOVER;
                            textoNivel.setString("¡Juego Completado!");
                            std::cout << "Rey Hongo eliminado, cambiando a GAMEOVER" << std::endl;
                        }
                    }
                }
                if (evento.key.code == sf::Keyboard::Space) {
                    jugador.saltar();
                    std::cout << "Saltando" << std::endl;
                }
            }
        }
    }
}

void Juego::actualizar(float deltaTime) {
    // Actualizar estado del salto de Ray
    jugador.actualizarSalto(deltaTime);
    // Manejar movimiento del jugador
    if (estado == TUTORIAL || estado == JUGANDO || estado == JEFEFINAL) {
        float velocidad = 150.0f; // 100 píxeles por segundo
        float dx = 0.0f, dy = 0.0f;
        bool moviendo = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            dx = -velocidad * deltaTime; // Mover a la izquierda
            moviendo = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            dx = velocidad * deltaTime; // Mover a la derecha
            moviendo = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            dy = -velocidad * deltaTime; // Mover arriba
            moviendo = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            dy = velocidad * deltaTime; // Mover abajo
            moviendo = true;
        }

        // Normalizar movimiento diagonal
        if (dx != 0.0f && dy != 0.0f) {
            float factor = 1.0f / std::sqrt(2.0f);
            dx *= factor;
            dy *= factor;
        }

        // Verificar si el movimiento es válido y está dentro de los límites
        if (moviendo) {
            sf::Vector2f pos = jugador.getPosition();
            float newX = pos.x + dx;
            float newY = pos.y + dy;

            // Obtener los límites de Ray
            sf::FloatRect bounds = jugador.getBounds();
            bounds.left = newX;
            bounds.top = newY;

            // Verificar límites de la pantalla con margen
            bool dentroLimites = bounds.left >= 0 && bounds.top >= 0 &&
                                 bounds.left + bounds.width <= 1920 &&
                                 bounds.top + bounds.height <= 1080;

            std::cout << "Intentando mover Ray a (" << newX << ", " << newY << "), Bounds: ["
                      << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height
                      << "]" << std::endl;

            if (PuedeMoverse(newX, newY, nullptr) && dentroLimites) {
                jugador.mover(dx, dy);
                std::cout << "Ray movido a (" << newX << ", " << newY << ")" << std::endl;
            } else {
                std::cout << "Movimiento de Ray bloqueado: "
                          << (dentroLimites ? "Colisión" : "Fuera de límites") << std::endl;
            }
        }

        // Verificar si Ray alcanzó el final del laberinto en nivel 1
        if (estado == JUGANDO && nivel == 1 && !laberintoCompletado) {
            sf::Vector2f posJugador = jugador.getPosition();
            if (finalLaberinto.contains(posJugador)) {
                laberintoCompletado = true;
                std::cout << "Ray alcanzó el final del laberinto en (" << posJugador.x << ", " << posJugador.y
                          << "), rectángulo: [" << finalLaberinto.left << ", " << finalLaberinto.top << ", "
                          << finalLaberinto.width << ", " << finalLaberinto.height << "]" << std::endl;
            }
        }

        // Mover hormigas infectadas y disparar esporas
        std::cout << "Actualizando hormigas, NPCs: " << npc.size() << ", Estado: " << estado << std::endl;
        sf::Vector2f posicionJugador = jugador.getPosition();
        for (auto& personaje : npc) {
            std::cout << "Procesando NPC en (" << personaje->getBounds().left << ", " << personaje->getBounds().top << ")" << std::endl;
            if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                std::cout << "Hormiga detectada, infectada: " << hormiga->getInfectadas() << std::endl;
                if (hormiga->getInfectadas()) {
                    // Solo mover hormigas en nivel 1 si el laberinto está completado
                    if (estado != JUGANDO || nivel != 1 || laberintoCompletado) {
                        hormiga->moverHaciaJugador(posicionJugador, deltaTime, [this, &personaje](float x, float y) {
                            return PuedeMoverse(x, y, personaje.get());
                        });
                        if (hormiga->estaEnPosicionDisparo()) {
                            hormiga->dispararEspora(esporas, direccionesEsporas, &jugador);
                        }
                    } else {
                        std::cout << "Hormiga en nivel 1 esperando, laberinto no completado" << std::endl;
                    }
                }
            } else {
                std::cout << "dynamic_cast a Hormigas falló" << std::endl;
            }
        }

        // Actualizar esporas
        for (size_t i = 0; i < esporas.size();) {
            // Mover espora
            float velocidadEspora = 100.0f; // 100 píxeles por segundo
            sf::Vector2f movimiento(direccionesEsporas[i].x * velocidadEspora * deltaTime, direccionesEsporas[i].y * velocidadEspora * deltaTime);
            esporas[i].move(movimiento);

            // Verificar colisión con Ray (ignorar si está saltando)
            sf::FloatRect esporaBounds(esporas[i].getPosition().x - esporas[i].getRadius(), esporas[i].getPosition().y - esporas[i].getRadius(),
                                      esporas[i].getRadius() * 2, esporas[i].getRadius() * 2);
            if (esporaBounds.intersects(jugador.getBounds()) && !jugador.isSaltando()) {
                for (auto& personaje : npc) {
                    if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                        if (hormiga->getInfectadas()) {
                            hormiga->atacar(&jugador);
                            break; // Cualquier hormiga infectada aplica el daño
                        }
                    }
                }
                esporas.erase(esporas.begin() + i);
                direccionesEsporas.erase(direccionesEsporas.begin() + i);
                std::cout << "Espora impactó a Ray, vida: " << jugador.getVida() << std::endl;
                if (jugador.getVida() <= 0) {
                    estado = GAMEOVER;
                    textoNivel.setString("¡Game Over!");
                    std::cout << "Ray eliminado, cambiando a GAMEOVER" << std::endl;
                }
                continue;
            } else if (esporaBounds.intersects(jugador.getBounds()) && jugador.isSaltando()) {
                std::cout << "Espora ignorada durante salto en (" << esporas[i].getPosition().x << ", " << esporas[i].getPosition().y << ")" << std::endl;
            }

            // Verificar si la espora salió de la pantalla
            if (esporas[i].getPosition().x < 0 || esporas[i].getPosition().x > 1920 || esporas[i].getPosition().y < 0 || esporas[i].getPosition().y > 1080) {
                esporas.erase(esporas.begin() + i);
                direccionesEsporas.erase(direccionesEsporas.begin() + i);
                std::cout << "Espora salió de la pantalla" << std::endl;
                continue;
            }

            ++i;
        }
    }

    // Lógica de estado
    if (estado == TUTORIAL) {
        if (npc.empty()) {
            estado = JUGANDO;
            textoNivel.setString("¡Tutorial completado!");
            std::cout << "Tutorial completado, cambiando a JUGANDO" << std::endl;
            cargarnivel(nivel);
        }
    } else if (estado == JUGANDO && nivel == 1) {
        if (npc.empty()) {
            estado = JEFEFINAL;
            textoNivel.setString("¡Desafío Final: Rey Hongo!");
            std::cout << "Todas las hormigas eliminadas en nivel 1, cambiando a JEFEFINAL" << std::endl;
            inicializarJefeFinal();
        }
    } else if (estado == JEFEFINAL) {
        if (npc.empty() && oleadasJefeFinal < 2) {
            std::cout << "Todas las hormigas eliminadas, generando oleada " << (oleadasJefeFinal + 1) << std::endl;
            oleadasJefeFinal++;
            jefefinal.controlarHormigas(npc, posicionesHormigasJefeFinal);
            std::cout << "Oleada " << oleadasJefeFinal << " generada, NPCs: " << npc.size() << std::endl;
        }
    }
}

// Función auxiliar para dibujar barras de salud
void Juego::dibujarBarraSalud(sf::RenderWindow& ventana, Personaje& personaje, float vidaMax) {
    // Obtener posición y tamaño del personaje
    sf::FloatRect bounds = personaje.getBounds();

    // Configurar dimensiones de la barra (ajustadas según escala del personaje)
    float escala = (estado == JUGANDO && nivel == 1) ? 0.5f : 1.0f; // Escala de Ray/hormigas
    if (&personaje == &jefefinal) escala = 1.5f; // Escala del Rey Hongo
    float anchoBarra = 80.0f * escala;
    float altoBarra = 10.0f * escala;
    float offsetY = 15.0f * escala; // Espacio encima del sprite

    // Posición centrada encima del personaje
    float x = bounds.left + (bounds.width - anchoBarra) / 2;
    float y = bounds.top - offsetY - altoBarra;

    // Barra de fondo (vida máxima, rojo)
    sf::RectangleShape fondoBarra(sf::Vector2f(anchoBarra, altoBarra));
    fondoBarra.setPosition(x, y);
    fondoBarra.setFillColor(sf::Color::Red);
    fondoBarra.setOutlineColor(sf::Color::Black);
    fondoBarra.setOutlineThickness(1.0f);

    // Barra de vida (vida actual, verde)
    float vida = personaje.getVida();
    float proporcion = std::max(0.0f, std::min(1.0f, vida / vidaMax));
    sf::RectangleShape barraVida(sf::Vector2f(anchoBarra * proporcion, altoBarra));
    barraVida.setPosition(x, y);
    barraVida.setFillColor(sf::Color::Green);
    barraVida.setOutlineColor(sf::Color::Black);
    barraVida.setOutlineThickness(1.0f);

    // Dibujar barras
    ventana.draw(fondoBarra);
    ventana.draw(barraVida);
}

// Dibuja elementos según estado
void Juego::renderizar() {
    ventana.clear(sf::Color::Black);
    if (estado == BIENVENIDA) {
        ventana.draw(spriteFondoBienvenida);
        ventana.draw(textoBienvenida);
        ventana.draw(botonIniciar);
        ventana.draw(textoBoton);
    } else if (estado == TUTORIAL) {
        ventana.draw(spriteFondoTutorial);
        for (auto& hormiga : npc) {
            hormiga->dibujar(ventana);
            dibujarBarraSalud(ventana, *hormiga, 50.0f);
        }
        jugador.dibujar(ventana);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        ventana.draw(textoNivel);
    } else if (estado == JUGANDO) {
        if (nivel == 1) {
            ventana.draw(spriteLaberinto);
            // Dibujar los muros visuales
            for (const auto& muro : laberintoMurosVisuales) {
                ventana.draw(muro);
            }
        } else {
            ventana.draw(spriteFondoTutorial);
        }
        for (auto& personaje : npc) {
            if (personaje) {
                personaje->dibujar(ventana);
                dibujarBarraSalud(ventana, *personaje, 50.0f);
            }
        }
        jugador.dibujar(ventana);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        ventana.draw(textoNivel);
    } else if (estado == JEFEFINAL) {
        ventana.draw(spritefondojefefinal);
        for (auto& personaje : npc) {
            if (personaje) {
                personaje->dibujar(ventana);
                dibujarBarraSalud(ventana, *personaje, 50.0f);
            }
        }
        jefefinal.dibujar(ventana);
        dibujarBarraSalud(ventana, jefefinal, 200.0f);
        jugador.dibujar(ventana);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        ventana.draw(textoNivel);
    } else if (estado == GAMEOVER) {
        ventana.draw(spriteFondoTutorial);
        ventana.draw(textoNivel);
    }
    ventana.display();
}

// Inicializa NPCs y posición de jugador para tutorial
void Juego::inicializarTutorial() {
    cout << "Inicializando TUTORIAL" << endl;
    jugador.setScale(1.0f, 1.0f);
    jugador.setPosition(10, 10);
    cout << "Ray dibujado en 10 , 10" << endl;
    npc.clear();
    npc.push_back(make_unique<Hormigas>(true));
    npc.back()->setPosition(1600, 200);
    cout << "Hormiga dibujado en 1600 , 100" << endl;
    npc.push_back(make_unique<Hormigas>(true));
    npc.back()->setPosition(1600, 600);
    cout << "Hormiga dibujado en 1600 , 500" << endl;
}

bool Juego::PuedeMoverse(float Xnew, float Ynew, Personaje* personajeActual) {
    sf::FloatRect bounds;
    if (personajeActual) {
        bounds = personajeActual->getBounds();
        bounds.left = Xnew;
        bounds.top = Ynew;
    } else {
        bounds = jugador.getBounds(); // Usar getBounds predeterminado
        bounds.left = Xnew;
        bounds.top = Ynew;
    }

    std::cout << "Verificando movimiento a (" << Xnew << ", " << Ynew << "), Bounds: ["
              << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height << "]" << std::endl;

    // Verificar colisión con muros del laberinto
    if (estado == JUGANDO && nivel == 1) {
        for (const auto& muro : laberintoMuros) {
            if (bounds.intersects(muro)) {
                std::cout << "Colisión detectada con muro en [" << muro.left << ", " << muro.top << ", "
                          << muro.width << ", " << muro.height << "]" << std::endl;
                return false;
            }
        }
    }

    // Verificar colisión con otros personajes
    for (const auto& personaje : npc) {
        if (personaje.get() == personajeActual) continue;
        sf::FloatRect personajeBounds = personaje->getBounds();
        if (bounds.intersects(personajeBounds)) {
            std::cout << "Colisión detectada con personaje en (" << personajeBounds.left << ", "
                      << personajeBounds.top << ")" << std::endl;
            return false;
        }
    }
    if (estado == JEFEFINAL && personajeActual != &jefefinal) {
        sf::FloatRect jefeBounds = jefefinal.getBounds();
        if (bounds.intersects(jefeBounds)) {
            std::cout << "Colisión detectada con Rey Hongo en (" << jefeBounds.left << ", "
                      << jefeBounds.top << ")" << std::endl;
            return false;
        }
    }

    // Verificar límites de la pantalla
    bool dentroLimites = bounds.left >= 0 && bounds.top >= 0 &&
                         bounds.left + bounds.width <= 1920 &&
                         bounds.top + bounds.height <= 1080;
    if (!dentroLimites) {
        std::cout << "Movimiento fuera de límites" << std::endl;
        return false;
    }

    return true;
}

void Juego::inicializarJefeFinal() {
    std::cout << "Inicializando JEFEFINAL" << std::endl;
    npc.clear();
    oleadasJefeFinal = 0;
    std::cout << "NPCs limpiados, tamaño: " << npc.size() << std::endl;
    jugador.setPosition(posicionInicialRayNivel1.x, posicionInicialRayNivel1.y);
    jugador.setScale(1.0f, 1.0f); // Escala normal para el jefe
    std::cout << "Ray posicionado en (" << posicionInicialRayNivel1.x << ", "
              << posicionInicialRayNivel1.y << ") con escala (1.0, 1.0)" << std::endl;
    jefefinal.setPosition(1500, 200);
    jefefinal.setScale(1.5f, 1.5f); // Escala normal para el jefe
    std::cout << "Rey Hongo posicionado en (1000, 800) con escala (1.0, 1.0), vida: "
              << jefefinal.getVida() << std::endl;
    oleadasJefeFinal++;
    jefefinal.controlarHormigas(npc, posicionesHormigasJefeFinal); // Generar primera oleada
    std::cout << "Primera oleada generada, NPCs: " << npc.size() << std::endl;
}

// Resto de métodos (vacios o mínimos)
void Juego::cargartop10() {}

void Juego::cargarnivel(int nivel) {
    npc.clear();
    std::cout << "Cargando nivel " << nivel << ", NPCs limpiados, tamaño: " << npc.size() << std::endl;

    if (nivel == 1) {
        jugador.setPosition(posicionInicialRayNivel1.x, posicionInicialRayNivel1.y); // Usar setPosition
        jugador.setScale(0.5f, 0.5f);
        jugador.restablecerVida();
        laberintoCompletado = false;
        std::cout << "Ray reposicionado en (" << posicionInicialRayNivel1.x << ", "
                  << posicionInicialRayNivel1.y << ") con escala (0.5, 0.5) para nivel " << nivel << std::endl;

        for (size_t i = 0; i < posicionesHormigasNivel1.size() && i < 5; ++i) {
            npc.push_back(std::make_unique<Hormigas>(true));
            npc.back()->setPosition(posicionesHormigasNivel1[i].x, posicionesHormigasNivel1[i].y);
            npc.back()->setScale(0.5f, 0.5f);
            std::cout << "Hormiga infectada creada en (" << posicionesHormigasNivel1[i].x << ", "
                      << posicionesHormigasNivel1[i].y << ") con escala (0.5, 0.5), vida: "
                      << npc.back()->getVida() << std::endl;
        }
    } else {
        jugador.setPosition(100, 800);
        jugador.setScale(1.0f, 1.0f);
        std::cout << "Ray reposicionado en (100, 800) con escala (1.0, 1.0) para nivel " << nivel << std::endl;
        for (int i = 0; i < nivel * 5; ++i) {
            npc.push_back(std::make_unique<Hormigas>(true));
            npc.back()->setPosition(300 + i * 100, 800);
            npc.back()->setScale(1.0f, 1.0f);
            std::cout << "Hormiga infectada creada en (" << 300 + i * 100 << ", 800) con escala (1.0, 1.0), vida: "
                      << npc.back()->getVida() << std::endl;
        }
    }
    textoNivel.setString("Nivel " + std::to_string(nivel));
    std::cout << "Nivel " << nivel << " cargado, NPCs: " << npc.size() << std::endl;
}

void Juego::cambiarnivel() {
    nivel++;
    textoNivel.setString("Desafio final");
}