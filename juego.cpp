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
    marcadorPuntaje(fuente),
    oleadasJefeFinal(0),
    posicionInicialRayNivel1(5, 35),
    laberintoCompletado(false),
      finalLaberinto(1180, 400, 200, 200), // Rectángulo de 200x200
    laberintoCargado(false),
    hongosGeneradosDespuesSegundaOleada(false)
{
    ventana.setFramerateLimit(60);
    // Carga fuente una sola vez
    if (!fuente.loadFromFile("pressgothic.ttf")) {
        cout << "Error: no se pudo cargar pressgothic.ttf" << endl;
    }

    // Carga fondo de bienvenida
    if (!fondoBienvenida.loadFromFile("FondoInicios.png")) {
        cout << "Error cargando fondo de bienvenida" << endl;
    }
    spriteFondoBienvenida.setTexture(fondoBienvenida);

    if (!fondoGameover.loadFromFile("PantallaGAmeover.png")) {
        cout << "Error cargando fondo de bienvenida" << endl;
    }
    spriteFondoGameover.setTexture(fondoGameover);

    if (!fondoWinner.loadFromFile("fondoWinner.png")) {
        cout << "Error cargando fondo de bienvenida" << endl;
    }
    spriteFondoWinner.setTexture(fondoWinner);

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

    // Configura botón de reinicio
    botonReiniciar.setSize(sf::Vector2f(500, 200));
    botonReiniciar.setFillColor(sf::Color(255, 255, 255, 80));
    botonReiniciar.setPosition(760, 680); // Centrado debajo del texto "Game Over" (1920/2 - 400/2, 1080/2 + 150)
    textoReiniciar.setFont(fuente);
    textoReiniciar.setString("REINICIAR");
    textoReiniciar.setCharacterSize(120);
    textoReiniciar.setFillColor(sf::Color::White);
    textoReiniciar.setPosition(829, 710); // Ajustado para centrar dentro del botón

    // Configura botón de "Volver a Jugar"
    botonVolverJugar.setSize(sf::Vector2f(400, 200));
    botonVolverJugar.setFillColor(sf::Color(255, 255, 255, 80));
    botonVolverJugar.setPosition(510, 680); // A la izquierda del centro (960 - 450)
    textoVolverJugar.setFont(fuente);
    textoVolverJugar.setString("VOLVER A JUGAR");
    textoVolverJugar.setCharacterSize(80);
    textoVolverJugar.setFillColor(sf::Color::White);
    textoVolverJugar.setPosition(532, 735); // Ajustado para centrar dentro del botón

    // Configura botón de "Salir"
    botonSalir.setSize(sf::Vector2f(400, 200));
    botonSalir.setFillColor(sf::Color(255, 255, 255, 80));
    botonSalir.setPosition(1010, 680); // A la derecha del centro (960 + 50)
    textoSalir.setFont(fuente);
    textoSalir.setString("SALIR");
    textoSalir.setCharacterSize(120);
    textoSalir.setFillColor(sf::Color::White);
    textoSalir.setPosition(1100, 715); // Ajustado para centrar dentro del botón


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

    if (!semillaTexture.loadFromFile("semillas.png")) {
        std::cout << "Error al cargar la textura de la semilla" << std::endl;
    }

    if (!hongoTexture.loadFromFile("hongosbiolumicentes.png")) {
        std::cout << "No se pudo cargar hongo.png, usando círculo rojo" << std::endl;
    }

    if (!esporaHormigaTexture.loadFromFile("Esporasnpc.png")) {
        std::cout << "Error al cargar espora_hormiga.png, usando círculo rojo" << std::endl;
    }

    if (!esporaReyHongoTexture.loadFromFile("Esporasreyhongo.png")) {
        std::cout << "Error al cargar espora_reyhongo.png, usando círculo rojo" << std::endl;
    }

    posicionesHormigasNivel1 = {
        {1600, 100},  // Hormiga 1
        {1600, 300},  // Hormiga 2
        {1600, 600},  // Hormiga 3
        {1600, 800},  // Hormiga 4
    };

    posicionesHormigasJefeFinal = {
        {1000, 100},  // Hormiga 1
        {1000, 400},  // Hormiga 2S
        {1000, 700}, // Hormiga 3

    };

    posicionesHormigasNoInfectadas ={
        {440, 250},  // Hormiga 1
        {250, 950},
        {970, 760},
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

    laberintoMuros.push_back(sf::FloatRect(0, 0, 1240, 20));          // Barra superior horizontal grande (arriba)
    laberintoMuros.push_back(sf::FloatRect(0, 920, 1240, 200));       // Barra inferior horizontal grande (abajo)
    laberintoMuros.push_back(sf::FloatRect(800, 800, 280, 10));       // Rectángulo horizontal cerca abajo derecha
    laberintoMuros.push_back(sf::FloatRect(1049, 700, 40, 100));      // Rectángulo vertical pequeño, media derecha
    laberintoMuros.push_back(sf::FloatRect(980, 650, 50, 50));        //  Cuadro cuadrado cerca derecha centro
    laberintoMuros.push_back(sf::FloatRect(1040, 200, 30, 330));      //  Rectángulo vertical largo derecha central
    laberintoMuros.push_back(sf::FloatRect(10, 190, 500, 430));       // Gran rectángulo vertical izquierda central
    laberintoMuros.push_back(sf::FloatRect(600, 190, 70, 250));       // Rectángulo vertical pequeño derecha del anterior
    laberintoMuros.push_back(sf::FloatRect(480, 550, 100, 50));       // Rectángulo horizontal pequeño centro
    laberintoMuros.push_back(sf::FloatRect(720, 500, 20, 10));        //  Rectángulo horizontal pequeño centro
    laberintoMuros.push_back(sf::FloatRect(690, 500, 20, 5));         //  Rectángulo horizontal muy pequeño centro
    laberintoMuros.push_back(sf::FloatRect(750, 300, 20, 5));         // Rectángulo horizontal pequeño arriba centro
    laberintoMuros.push_back(sf::FloatRect(750, 550, 70, 250));       // Rectángulo vertical derecha-centro
    laberintoMuros.push_back(sf::FloatRect(300, 150, 180, 350));      //  Gran rectángulo vertical izquierda centro
    laberintoMuros.push_back(sf::FloatRect(700, 150, 130, 20));       // Rectángulo horizontal arriba derecha-centro
    laberintoMuros.push_back(sf::FloatRect(850, 200, 200, 20));       // Rectángulo horizontal centro derecha
    laberintoMuros.push_back(sf::FloatRect(950, 170, 100, 30));       //  Rectángulo mediano horizontal arriba derecha
    laberintoMuros.push_back(sf::FloatRect(900, 360, 50, 280));       //  Rectángulo vertical centro derecha
    laberintoMuros.push_back(sf::FloatRect(750, 350, 100, 40));       // Rectángulo horizontal centro derecha
    laberintoMuros.push_back(sf::FloatRect(1100, 530, 30, 25));       // Rectángulo pequeño vertical derecha abajo
    laberintoMuros.push_back(sf::FloatRect(0, 600, 640, 600));        // Gran rectángulo vertical izquierda abajo
    laberintoMuros.push_back(sf::FloatRect(1160, 0, 70, 400));        //  Rectángulo vertical muy derecha arriba
    laberintoMuros.push_back(sf::FloatRect(1160, 550, 70, 600));      // Rectángulo vertical muy derecha abajo

    // Crear visualización de los muros

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

        sf::FloatRect boundsIniciar = botonIniciar.getGlobalBounds();
        if (estado == BIENVENIDA && boundsIniciar.contains(mousePos.x, mousePos.y)) {
            botonIniciar.setFillColor(sf::Color(200, 200, 200, 150));
            ventana.setMouseCursor(cursorMano);
        } else {
            botonIniciar.setFillColor(sf::Color(255, 255, 255, 80));
        }

        sf::FloatRect boundsReiniciar = botonReiniciar.getGlobalBounds();
        if (estado == GAMEOVER && boundsReiniciar.contains(mousePos.x, mousePos.y)) {
            botonReiniciar.setFillColor(sf::Color(200, 200, 200, 150));
            ventana.setMouseCursor(cursorMano);
        } else {
            botonReiniciar.setFillColor(sf::Color(255, 255, 255, 80));
        }

        // Manejo del cursor para los botones en WIN
        sf::FloatRect boundsVolverJugar = botonVolverJugar.getGlobalBounds();
        sf::FloatRect boundsSalir = botonSalir.getGlobalBounds();
        if (estado == WIN) {
            if (boundsVolverJugar.contains(mousePos.x, mousePos.y)) {
                botonVolverJugar.setFillColor(sf::Color(200, 200, 200, 150));
                ventana.setMouseCursor(cursorMano);
            } else {
                botonVolverJugar.setFillColor(sf::Color(255, 255, 255, 80));
            }
            if (boundsSalir.contains(mousePos.x, mousePos.y)) {
                botonSalir.setFillColor(sf::Color(200, 200, 200, 150));
                ventana.setMouseCursor(cursorMano);
            } else {
                botonSalir.setFillColor(sf::Color(255, 255, 255, 80));
            }
            if (!boundsVolverJugar.contains(mousePos.x, mousePos.y) && !boundsSalir.contains(mousePos.x, mousePos.y)) {
                ventana.setMouseCursor(cursorNormal);
            }
        } else if (estado != BIENVENIDA || !boundsIniciar.contains(mousePos.x, mousePos.y)) {
            if (estado != GAMEOVER || !boundsReiniciar.contains(mousePos.x, mousePos.y)) {
                ventana.setMouseCursor(cursorNormal);
            }
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
                limpiarEsporas();
                std::cout << "Estado cambiado a TUTORIAL" << std::endl;
                inicializarTutorial();
            }
        }
        if (estado == TUTORIAL || estado == JUGANDO || estado == JEFEFINAL) {
            std::cout << "Estado actual: " << estado << ", NPCs en escena: " << npc.size() << std::endl;
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
                                marcadorPuntaje.incrementarPuntaje(10);
                                std::cout << "Personaje eliminado, puntaje +10: " << puntaje << std::endl;
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
                            estado = WIN;
                            textoNivel.setString("¡Juego Completado!");
                            marcadorPuntaje.incrementarPuntaje(20);
                            std::cout << "Rey Hongo eliminado, cambiando a GAMEOVER" << std::endl;
                        }
                    }
                }
                if (evento.key.code == sf::Keyboard::Q) {
                    std::cout << "Tecla Q presionada, estado: " << estado << std::endl;
                    Ray* ray = dynamic_cast<Ray*>(&jugador);
                    if (ray) {
                        std::cout << "Cast a Ray exitoso, energía: " << ray->getBastonEnergia() << ", máxima: " << ray->getMaxEnergia() << std::endl;
                        if (ray->getBastonEnergia() >= 50) {
                            sf::Vector2f posRay = ray->getPosition();
                            sf::FloatRect boundsRay = ray->getBounds();
                            float inicioX = posRay.x + boundsRay.width;
                            float centroY = posRay.y + boundsRay.height / 2.0f;
                            std::cout << "Lado derecho de Ray: (" << inicioX << ", " << centroY << ")" << std::endl;

                            Personaje* enemigo = nullptr;
                            float distanciaMinima = 500.0f;
                            sf::Vector2f posEnemigoMasCercano;

                            // Buscar el NPC infectado más cercano
                            for (auto& personaje : npc) {
                                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                                    if (hormiga->getInfectadas() && hormiga->getVida() > 0) {
                                        sf::FloatRect boundsHormiga = hormiga->getBounds();
                                        // Calcular el centro de la hormiga
                                        float centroHormigaX = boundsHormiga.left + boundsHormiga.width / 2.0f;
                                        float centroHormigaY = boundsHormiga.top + boundsHormiga.height / 2.0f;
                                        float dx = inicioX - centroHormigaX;
                                        float dy = centroY - centroHormigaY;
                                        float distancia = std::sqrt(dx * dx + dy * dy);
                                        std::cout << "Centro de Hormiga: (" << centroHormigaX << ", " << centroHormigaY << "), Distancia: " << distancia << std::endl;
                                        if (distancia < distanciaMinima) {
                                            distanciaMinima = distancia;
                                            enemigo = hormiga;
                                            posEnemigoMasCercano = sf::Vector2f(centroHormigaX, centroHormigaY);
                                        }
                                    }
                                }
                            }

                            // Considerar al Rey Hongo si estamos en JEFEFINAL
                            if (estado == JEFEFINAL && jefefinal.getVida() > 0) {
                                sf::FloatRect boundsJefe = jefefinal.getBounds();
                                // Calcular el centro del jefe
                                float centroJefeX = boundsJefe.left + boundsJefe.width / 2.0f;
                                float centroJefeY = boundsJefe.top + boundsJefe.height / 2.0f;
                                float dx = inicioX - centroJefeX;
                                float dy = centroY - centroJefeY;
                                float distancia = std::sqrt(dx * dx + dy * dy);
                                std::cout << "Centro de Rey Hongo: (" << centroJefeX << ", " << centroJefeY << "), Distancia: " << distancia << std::endl;
                                // Priorizar al jefe si las distancias son iguales
                                if (distancia <= distanciaMinima) {
                                    distanciaMinima = distancia;
                                    enemigo = &jefefinal;
                                    posEnemigoMasCercano = sf::Vector2f(centroJefeX, centroJefeY);
                                }
                            }

                            if (enemigo) {
                                std::cout << "Enemigo seleccionado en: (" << posEnemigoMasCercano.x << ", " << posEnemigoMasCercano.y << ")" << std::endl;
                                // Crear un sprite para el proyectil en lugar de una bola
                                sf::Sprite ataqueLuz;
                                float dañoAtaque = (ray->getBastonEnergia() == ray->getMaxEnergia()) ? 50.0f : 20.0f;
                                ray->setEstaAtacando(true);
                                ray->setTiempoAtaqueRestante(0.5f);

                                if (dañoAtaque == 50.0f) {
                                    if (!ray->getTextureSuperGolpe().loadFromFile("raysupergolpedeluz.png")) {
                                        std::cout << "Error al cargar raysupergolpedeluz.png" << std::endl;
                                    }
                                    ray->getSpriteAtaque().setTexture(ray->getTextureSuperGolpe());
                                    std::cout << "Configurando animación para supergolpedeLuz" << std::endl;
                                    ataqueLuz.setTexture(ray->getProjectileSuperGolpeTexture());
                                } else {
                                    if (!ray->getTextureGolpe().loadFromFile("raygolpedeluz.png")) {
                                        std::cout << "Error al cargar raygolpedeluz.png" << std::endl;
                                    }
                                    ray->getSpriteAtaque().setTexture(ray->getTextureGolpe());
                                    std::cout << "Configurando animación para golpedeLuz" << std::endl;
                                    ataqueLuz.setTexture(ray->getProjectileGolpeTexture());
                                }
                                ataqueLuz.setOrigin(ataqueLuz.getLocalBounds().width / 2.0f, ataqueLuz.getLocalBounds().height / 2.0f);
                                ataqueLuz.setPosition(inicioX, centroY);
                                ataqueLuz.setScale(0.5f, 0.5f); // Ajustar escala si es necesario

                                ataquesLuz.push_back(ataqueLuz);
                                sf::Vector2f direccion = posEnemigoMasCercano - sf::Vector2f(inicioX, centroY);
                                float longitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
                                if (longitud != 0) {
                                    direccion /= longitud;
                                }
                                direccionesAtaques.push_back(direccion);
                                dañosAtaques.push_back(dañoAtaque);
                                int consumoEnergia = (dañoAtaque == 50.0f) ? 100 : 50;
                                ray->consumirEnergia(consumoEnergia);

                                std::cout << "Ataque de luz lanzado desde el lado derecho con daño: " << dañoAtaque << ", energía consumida: " << consumoEnergia
                                          << ", energía restante: " << ray->getBastonEnergia() << std::endl;
                            } else {
                                std::cout << "No se encontró enemigo dentro de 500.0f" << std::endl;
                            }
                        } else {
                            std::cout << "Energía insuficiente para lanzar ataque (necesita 50, tiene " << ray->getBastonEnergia() << ")" << std::endl;
                        }
                    } else {
                        std::cout << "Fallo en el cast a Ray, tipo de jugador: " << typeid(jugador).name() << std::endl;
                    }
                }
                if (evento.key.code == sf::Keyboard::Space) {
                    jugador.saltar();
                    std::cout << "Saltando" << std::endl;
                }
            }
        }
        // Manejo del botón de reinicio en GAMEOVER
        if (estado == GAMEOVER && evento.type == sf::Event::MouseButtonPressed &&
            evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
            if (botonReiniciar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                // Reiniciar el juego
                estado = TUTORIAL;
                nivel = 1;
                marcadorPuntaje.setPuntaje(0);
                oleadasJefeFinal = 0;
                laberintoCompletado = false;
                hongosGeneradosDespuesSegundaOleada = false;
                limpiarEsporas();
                inicializarTutorial();
                std::cout << "Juego reiniciado, estado cambiado a TUTORIAL" << std::endl;
                // Reiniciar la vida y energía de Ray
                Ray* ray = dynamic_cast<Ray*>(&jugador);
                if (ray) {
                    ray->restablecerVida();
                    ray->setBastonEnergia(ray->getMaxEnergia());
                }
                // Reiniciar la vida del jefe
                jefefinal.restablecerVida();
                // Limpiar NPCs, proyectiles y otras listas
                npc.clear();
                ataquesLuz.clear();
                direccionesAtaques.clear();
                dañosAtaques.clear();
                hongos.clear();
                semillas.clear();
            }
            }
    }// Manejo de los botones en WIN
    if (estado == WIN && evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
        // Botón "Volver a Jugar"
        if (botonVolverJugar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            estado = TUTORIAL;
            nivel = 1;
            marcadorPuntaje.setPuntaje(0);
            oleadasJefeFinal = 0;
            laberintoCompletado = false;
            hongosGeneradosDespuesSegundaOleada = false;
            limpiarEsporas();
            inicializarTutorial();
            std::cout << "Juego reiniciado desde WIN, estado cambiado a TUTORIAL" << std::endl;
            Ray* ray = dynamic_cast<Ray*>(&jugador);
            if (ray) {
                ray->restablecerVida();
                ray->setBastonEnergia(ray->getMaxEnergia());
            }
            jefefinal.restablecerVida();
            npc.clear();
            ataquesLuz.clear();
            direccionesAtaques.clear();
            dañosAtaques.clear();
            hongos.clear();
            semillas.clear();
        }
        // Botón "Salir"
        if (botonSalir.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            ventana.close();
            std::cout << "Saliendo del juego desde WIN" << std::endl;
        }
    }
}


void Juego::actualizar(float deltaTime) {
    jugador.actualizar(deltaTime);
    Ray& ray = dynamic_cast<Ray&>(jugador);

    if (estado == TUTORIAL || estado == JUGANDO || estado == JEFEFINAL) {
        float velocidad = 150.0f;
        float dx = 0.0f, dy = 0.0f;
        bool moviendo = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            dx = -velocidad * deltaTime;
            moviendo = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            dx = velocidad * deltaTime;
            moviendo = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            dy = -velocidad * deltaTime;
            moviendo = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            dy = velocidad * deltaTime;
            moviendo = true;
        }

        if (dx != 0.0f && dy != 0.0f) {
            float factor = 1.0f / std::sqrt(2.0f);
            dx *= factor;
            dy *= factor;
        }

        if (moviendo) {
            sf::Vector2f pos = jugador.getPosition();
            float newX = pos.x + dx;
            float newY = pos.y + dy;

            sf::FloatRect bounds = jugador.getBounds();
            bounds.left = newX;
            bounds.top = newY;

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

        if (estado == JUGANDO && nivel == 1 && !laberintoCompletado) {
            sf::Vector2f posJugador = jugador.getPosition();
            if (finalLaberinto.contains(posJugador)) {
                laberintoCompletado = true;
                std::cout << "Ray alcanzó el final del laberinto en (" << posJugador.x << ", " << posJugador.y
                          << "), rectángulo: [" << finalLaberinto.left << ", " << finalLaberinto.top << ", "
                          << finalLaberinto.width << ", " << finalLaberinto.height << "]" << std::endl;
            }
        }

        // Actualizar hormigas
        std::cout << "Actualizando hormigas, NPCs: " << npc.size() << ", Estado: " << estado << std::endl;
        sf::Vector2f posicionJugador = jugador.getPosition();
        for (auto it = npc.begin(); it != npc.end();) {
            Personaje* personaje = it->get();
            std::cout << "Procesando NPC en (" << personaje->getBounds().left << ", " << personaje->getBounds().top << ")" << std::endl;
            if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje)) {
                std::cout << "Hormiga detectada, infectada: " << hormiga->getInfectadas() << std::endl;
                if (hormiga->getInfectadas()) {
                    if (estado != JUGANDO || nivel != 1 || laberintoCompletado) {
                        hormiga->moverHaciaJugador(posicionJugador, deltaTime, [this, &personaje](float x, float y) {
                            return PuedeMoverse(x, y, personaje);
                        });
                        if (hormiga->getVida() > 0 && hormiga->estaEnPosicionDisparo()) {
                            hormiga->dispararEspora(esporas, direccionesEsporas, rotacionesEsporas, &jugador, esporaHormigaTexture);
                        }
                    } else {
                        std::cout << "Hormiga en nivel 1 esperando, laberinto no completado" << std::endl;
                    }
                }
            } else {
                std::cout << "dynamic_cast a Hormigas falló" << std::endl;
            }
            ++it;
        }

        // Activar ReyHongo solo después de las oleadas
        if (estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 2 && jefefinal.getVida() > 0) {
            jefefinal.moverHaciaJugador(posicionJugador, deltaTime, [this](float x, float y) {
                return PuedeMoverse(x, y, &jefefinal);
            });
            if (jefefinal.estaEnPosicionDisparo()) {
                jefefinal.dispararEspora(esporas, direccionesEsporas, rotacionesEsporas, &jugador, esporaReyHongoTexture);
            }
        }

        // Recolectar hongos para restaurar energía
        sf::FloatRect rayBounds = jugador.getBounds();
        for (size_t i = 0; i < hongos.size();) {
            sf::FloatRect hongoBounds = hongos[i].getGlobalBounds();
            if (rayBounds.intersects(hongoBounds)) {
                if (ray.getBastonEnergia() >= ray.getMaxEnergia()) {
                    std::cout << "Energía llena (" << ray.getBastonEnergia() << "), no se recolecta hongo en ("
                              << hongos[i].getPosition().x << ", " << hongos[i].getPosition().y << ")" << std::endl;
                    ++i;
                    continue;
                }
                float energiaMax = ray.getMaxEnergia();
                float energiaRecuperada = energiaMax * 0.5f; // 50% de la energía máxima
                float nuevaEnergia = std::min(ray.getBastonEnergia() + energiaRecuperada, energiaMax);
                ray.setBastonEnergia(nuevaEnergia);
                std::cout << "Hongo recolectado en (" << hongos[i].getPosition().x << ", " << hongos[i].getPosition().y
                          << "), energía restaurada: " << energiaRecuperada << ", nueva energía: " << nuevaEnergia << std::endl;
                // Marcar el hongo como recolectado en posicionesOriginalesHongos
                for (auto& posEstado : posicionesOriginalesHongos) {
                    if (posEstado.first == hongos[i].getPosition()) {
                        posEstado.second = true;
                        break;
                    }
                }
                marcadorPuntaje.incrementarPuntaje(5);
                hongos.erase(hongos.begin() + i);
                continue;
            }
            ++i;
        }

        // Recolectar semillas
        for (size_t i = 0; i < semillas.size();) {
            sf::FloatRect semillaBounds = semillas[i].getGlobalBounds();
            if (rayBounds.intersects(semillaBounds)) {
                if (jugador.getVida() >= 100) {
                    std::cout << "Vida llena (" << jugador.getVida() << "), no se recolecta semilla en ("
                              << semillas[i].getPosition().x << ", " << semillas[i].getPosition().y << ")" << std::endl;
                    ++i;
                    continue;
                }
                float vidaRecuperada = 15.0f;
                jugador.aumentarVida(static_cast<int>(vidaRecuperada));
                std::cout << "Semilla recolectada en (" << semillas[i].getPosition().x << ", " << semillas[i].getPosition().y
                          << "), vida restaurada: " << vidaRecuperada << ", nueva vida: " << jugador.getVida() << std::endl;
                for (auto& posEstado : posicionesOriginalesSemillas) {
                    if (posEstado.first == semillas[i].getPosition()) {
                        posEstado.second = true;
                        break;
                    }
                }
                marcadorPuntaje.incrementarPuntaje(5);
                semillas.erase(semillas.begin() + i);
                continue;
            }
            ++i;
        }

        // Regenerar hongos después de la primera oleada en JEFEFINAL
if (estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 1) {
    for (size_t i = 0; i < posicionesOriginalesHongos.size(); ++i) {
        if (posicionesOriginalesHongos[i].second) { // Solo regenerar si fue recolectado
            sf::Sprite hongo(hongoTexture);
            if (hongoTexture.getSize().x == 0) {
                cout << "no hay textura" << endl;
            } else {
                hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);
                hongo.setPosition(posicionesOriginalesHongos[i].first);
                hongo.setScale(0.5f, 0.5f);
            }
            hongos.push_back(hongo);
            posicionesOriginalesHongos[i].second = false; // Resetear el estado
            std::cout << "Hongo regenerado en (" << posicionesOriginalesHongos[i].first.x << ", "
                      << posicionesOriginalesHongos[i].first.y << ")" << std::endl;
        }
    }
    // Regenerar semillas después de la primera oleada
    for (size_t i = 0; i < posicionesOriginalesSemillas.size(); ++i) {
        if (posicionesOriginalesSemillas[i].second) {
            sf::Sprite semilla(semillaTexture);
            if (semillaTexture.getSize().x == 0) {
                cout << "no hay textura" << endl;
            } else {
                semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);
                semilla.setPosition(posicionesOriginalesSemillas[i].first);
                semilla.setScale(0.5f, 0.5f);
            }
            semillas.push_back(semilla);
            posicionesOriginalesSemillas[i].second = false;
            std::cout << "Semilla regenerada en (" << posicionesOriginalesSemillas[i].first.x << ", "
                      << posicionesOriginalesSemillas[i].first.y << ")" << std::endl;
        }
    }
}

// Generar nuevos hongos y nuevas semillas después de la segunda oleada en JEFEFINAL
if (estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 2 && !hongosGeneradosDespuesSegundaOleada) {
    hongosGeneradosDespuesSegundaOleada = true;
    std::vector<sf::Vector2f> nuevasPosicionesHongos = {
        {1050, 300}, {900, 400}, {1000, 500}, {400, 500}, {300, 220}
    };
    for (const auto& pos : nuevasPosicionesHongos) {
        sf::Sprite hongo(hongoTexture);
        if (hongoTexture.getSize().x == 0) {
            cout << "No hay Textura" << endl;
        } else {
            hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);
            hongo.setPosition(pos);
            hongo.setScale(0.5f, 0.5f);
        }
        hongos.push_back(hongo);
        posicionesOriginalesHongos.push_back(std::make_pair(pos, false));
        std::cout << "Nuevo hongo generado después de la segunda oleada en (" << pos.x << ", " << pos.y << ")" << std::endl;
    }
    std::vector<sf::Vector2f> nuevasPosicionesSemillas = {
        {100, 300}, {250, 800}, {550, 500}, {350, 50}, {650, 200}, {450, 230}
    };
    for (const auto& pos : nuevasPosicionesSemillas) {
        sf::Sprite semilla(semillaTexture);
        if (semillaTexture.getSize().x == 0) {
            cout << "No hay textura" <<endl;
        } else {
            semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);
            semilla.setPosition(pos);
            semilla.setScale(0.5f, 0.5f);
        }
        semillas.push_back(semilla);
        posicionesOriginalesSemillas.push_back(std::make_pair(pos, false));
        std::cout << "Nueva semilla generada después de la segunda oleada en (" << pos.x << ", " << pos.y << ")" << std::endl;
    }
}

        // Actualizar esporas
        for (size_t i = 0; i < esporas.size();) {
            float velocidadEspora = 150.0f;
            sf::Vector2f movimiento = direccionesEsporas[i] * velocidadEspora * deltaTime;
            esporas[i].move(movimiento);

            // Animación de rotación
            rotacionesEsporas[i] += 5.0f; // Incremento de 5 grados por frame
            if (rotacionesEsporas[i] >= 360.0f) {
                rotacionesEsporas[i] -= 360.0f; // Reiniciar a 0 después de un giro completo
            }
            esporas[i].setRotation(rotacionesEsporas[i]);

    sf::FloatRect esporaBounds(esporas[i].getPosition().x - esporas[i].getGlobalBounds().width / 2,
                               esporas[i].getPosition().y - esporas[i].getGlobalBounds().height / 2,
                               esporas[i].getGlobalBounds().width, esporas[i].getGlobalBounds().height);
    if (esporaBounds.intersects(jugador.getBounds()) && !jugador.isSaltando()) {
        bool dañoAplicado = false;
        for (auto& personaje : npc) {
            if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                if (hormiga->getInfectadas()) {
                    hormiga->atacar(&jugador);
                    dañoAplicado = true;
                    break;
                }
            }
        }
        if (!dañoAplicado && estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 2) {
            jugador.recibirdano(25);
        }
        esporas.erase(esporas.begin() + i);
        direccionesEsporas.erase(direccionesEsporas.begin() + i);
        rotacionesEsporas.erase(rotacionesEsporas.begin() + i);
        std::cout << "Espora impactó a Ray, vida: " << jugador.getVida() << std::endl;
        if (jugador.getVida() <= 0) {
            estado = GAMEOVER;
            textoNivel.setString("¡Game Over!");
            limpiarEsporas();
            std::cout << "Ray eliminado, cambiando a GAMEOVER" << std::endl;
        }
        continue;
    } else if (esporaBounds.intersects(jugador.getBounds()) && jugador.isSaltando()) {
        std::cout << "Espora ignorada durante salto en (" << esporas[i].getPosition().x << ", " << esporas[i].getPosition().y << ")" << std::endl;
    }

    if (esporas[i].getPosition().x < 0 || esporas[i].getPosition().x > 1920 ||
        esporas[i].getPosition().y < 0 || esporas[i].getPosition().y > 1080) {
        esporas.erase(esporas.begin() + i);
        direccionesEsporas.erase(direccionesEsporas.begin() + i);
        rotacionesEsporas.erase(rotacionesEsporas.begin() + i);
        std::cout << "Espora salió de la pantalla" << std::endl;
        continue;
    }

    ++i;
}

        // Actualizar ataques de luz
        float velocidadAtaque = 300.0f;
        for (size_t i = 0; i < ataquesLuz.size();) {
            sf::Vector2f movimiento = direccionesAtaques[i] * velocidadAtaque * deltaTime;
            ataquesLuz[i].move(movimiento);

            sf::FloatRect ataqueBounds(ataquesLuz[i].getPosition().x - ataquesLuz[i].getGlobalBounds().width / 2,
                                       ataquesLuz[i].getPosition().y - ataquesLuz[i].getGlobalBounds().height / 2,
                                       ataquesLuz[i].getGlobalBounds().width, ataquesLuz[i].getGlobalBounds().height);
            bool colisiono = false;

            // Colisión con hormigas
            for (auto& personaje : npc) {
                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                    if (hormiga->getInfectadas() && hormiga->getVida() > 0 && ataqueBounds.intersects(hormiga->getBounds())) {
                        hormiga->recibirdano(dañosAtaques[i]);
                        std::cout << "Ataque de luz impactó a Hormiga en (" << hormiga->getPosition().x << ", " << hormiga->getPosition().y
                                  << "), daño causado: " << dañosAtaques[i] << ", vida restante: " << hormiga->getVida() << std::endl;
                        if (hormiga->getVida() <= 0) {
                            for (auto it = npc.begin(); it != npc.end();) {
                                if (it->get() == hormiga) {
                                    it = npc.erase(it);
                                    marcadorPuntaje.incrementarPuntaje(10);
                                    std::cout << "Hormiga eliminada" << std::endl;
                                    if (estado == JEFEFINAL && jefefinal.getVida() > 0) {
                                        jefefinal.recibirdano(5); // Quitar 5 de vida al Reyhongo
                                        std::cout << "Reyhongo perdió 5 de vida por eliminación de hormiga, vida restante: " << jefefinal.getVida() << std::endl;
                                    }
                                } else {
                                    ++it;
                                }
                            }
                        }
                        colisiono = true;
                        break;
                    }
                }
            }

            // Colisión con Rey Hongo
            if (estado == JEFEFINAL && jefefinal.getVida() > 0 && ataqueBounds.intersects(jefefinal.getBounds())) {
                jefefinal.recibirdano(dañosAtaques[i]);
                std::cout << "Ataque de luz impactó a Rey Hongo en (" << jefefinal.getPosition().x << ", " << jefefinal.getPosition().y
                          << "), daño causado: " << dañosAtaques[i] << ", vida restante: " << jefefinal.getVida() << std::endl;
                if (jefefinal.getVida() <= 0) {
                    estado = WIN;
                    textoNivel.setString("¡Juego Completado!");
                    std::cout << "Rey Hongo eliminado, cambiando a GAMEOVER" << std::endl;
                }
                colisiono = true;
            }

            if (colisiono || ataquesLuz[i].getPosition().x < 0 || ataquesLuz[i].getPosition().x > 1920 ||
                ataquesLuz[i].getPosition().y < 0 || ataquesLuz[i].getPosition().y > 1080) {
                ataquesLuz.erase(ataquesLuz.begin() + i);
                direccionesAtaques.erase(direccionesAtaques.begin() + i);
                dañosAtaques.erase(dañosAtaques.begin() + i);
                std::cout << "Ataque de luz eliminado" << std::endl;
                continue;
            }

            ++i;
        }
    }

    if (estado == TUTORIAL) {
        if (npc.empty()) {
            limpiarEsporas();
            estado = JUGANDO;
            textoNivel.setString("¡Tutorial completado!");
            std::cout << "Tutorial completado, cambiando a JUGANDO" << std::endl;
            cargarnivel(nivel);
        }
    } else if (estado == JUGANDO && nivel == 1) {
        if (npc.empty()) {
            limpiarEsporas();
            estado = JEFEFINAL;
            textoNivel.setString("¡Desafío Final: Rey Hongo!");
            std::cout << "Todas las hormigas eliminadas en nivel 1, cambiando a JEFEFINAL" << std::endl;
            inicializarJefeFinal();
        } else {
            // Verificar si quedan NPCs infectados vivos
            bool quedanInfectados = false;
            for (const auto& personaje : npc) {
                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                    if (hormiga->getInfectadas() && hormiga->getVida() > 0) {
                        quedanInfectados = true;
                        break;
                    }
                }
            }
            if (!quedanInfectados) {
                limpiarEsporas();
                estado = JEFEFINAL;
                textoNivel.setString("¡Desafío Final: Rey Hongo!");
                std::cout << "Todos los NPCs infectados eliminados en nivel 1, cambiando a JEFEFINAL" << std::endl;
                inicializarJefeFinal();
            }
        }
    } else if (estado == JEFEFINAL) {
        if (npc.empty() && oleadasJefeFinal < 2) {
            limpiarEsporas();
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


// Función para dibujar la barra de energía (siguiendo a Ray, más arriba)
void Juego::dibujarBarraEnergia(sf::RenderWindow& ventana, Ray& ray, float energiaMax) {
    // Obtener posición y tamaño del personaje
    sf::FloatRect bounds = ray.getBounds();

    // Configurar dimensiones de la barra (ajustadas según escala del personaje)
    float escala = (estado == JUGANDO && nivel == 1) ? 0.5f : 1.0f; // Escala de Ray
    float anchoBarra = 80.0f * escala;
    float altoBarra = 10.0f * escala;
    float offsetY = 30.0f * escala; // Más arriba que la barra de salud (15.0f + 15.0f)

    // Posición centrada encima del personaje, más arriba que la barra de salud
    float x = bounds.left + (bounds.width - anchoBarra) / 2;
    float y = bounds.top - offsetY - altoBarra;

    // Barra de fondo (energía máxima, gris)
    sf::RectangleShape fondoBarra(sf::Vector2f(anchoBarra, altoBarra));
    fondoBarra.setPosition(x, y);
    fondoBarra.setFillColor(sf::Color(50, 50, 50, 200)); // Fondo gris
    fondoBarra.setOutlineColor(sf::Color::Black);
    fondoBarra.setOutlineThickness(1.0f);

    // Barra de energía (energía actual, azul)
    float energia = ray.getBastonEnergia();
    float proporcion = std::max(0.0f, std::min(1.0f, static_cast<float>(energia) / energiaMax));
    sf::RectangleShape barraEnergia(sf::Vector2f(anchoBarra * proporcion, altoBarra));
    barraEnergia.setPosition(x, y);
    barraEnergia.setFillColor(sf::Color(0, 0, 255, 200)); // Azul semitransparente
    barraEnergia.setOutlineColor(sf::Color::Black);
    barraEnergia.setOutlineThickness(1.0f);

    // Dibujar barras
    ventana.draw(fondoBarra);
    ventana.draw(barraEnergia);
}

// Dibuja elementos según estado
void Juego::renderizar() {
    ventana.clear(sf::Color::Black);
    Ray* ray = dynamic_cast<Ray*>(&jugador);
    if (!ray) {
        std::cout << "Fallo en el cast a Ray en renderizar(), tipo de jugador: " << typeid(jugador).name() << std::endl;
        ventana.display();
        return;
    }
    if (estado == BIENVENIDA) {
        ventana.draw(spriteFondoBienvenida);
        ventana.draw(textoBienvenida);
        ventana.draw(botonIniciar);
        ventana.draw(textoBoton);
    } else if (estado == TUTORIAL) {
        ventana.draw(spriteFondoTutorial);
        for (const auto& hongo : hongos) {
            ventana.draw(hongo);
        }
        for (const auto& semilla : semillas) {
            ventana.draw(semilla);
        }
        for (auto& personaje : npc) {
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);
                dibujarBarraSalud(ventana, *personaje, 50.0f);
            }
        }
        jugador.dibujar(ventana);
        dibujarBarraEnergia(ventana, *ray, 100.0f);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        for (const auto& ataque : ataquesLuz) {
            ventana.draw(ataque);
        }
    } else if (estado == JUGANDO) {
        if (nivel == 1) {
            ventana.draw(spriteLaberinto);
        } else {
            ventana.draw(spriteFondoTutorial);
        }
        for (const auto& hongo : hongos) {
            ventana.draw(hongo);
        }
        for (const auto& semilla : semillas) {
            ventana.draw(semilla);
        }
        // Dibujar NPCs, diferenciando entre infectados y no infectados
        for (auto& personaje : npc) {
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);
                // Solo dibujar barra de salud para NPCs infectados
                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                    if (hormiga->getInfectadas()) {
                        dibujarBarraSalud(ventana, *personaje, 50.0f);
                    }
                }
            }
        }
        marcadorPuntaje.dibujar(ventana, nivel);
        jugador.dibujar(ventana);
        dibujarBarraEnergia(ventana, *ray, 100.0f);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        for (const auto& ataque : ataquesLuz) {
            ventana.draw(ataque);
        }
    } else if (estado == JEFEFINAL) {
        ventana.draw(spritefondojefefinal);
        for (const auto& hongo : hongos) {
            ventana.draw(hongo);
        }
        for (const auto& semilla : semillas) {
            ventana.draw(semilla);
        }
        for (auto& personaje : npc) {
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);
                dibujarBarraSalud(ventana, *personaje, 50.0f);
            }
        }
        marcadorPuntaje.dibujar(ventana, nivel);
        jefefinal.dibujar(ventana);
        dibujarBarraSalud(ventana, jefefinal, 200.0f);
        jugador.dibujar(ventana);
        dibujarBarraEnergia(ventana, *ray, 100.0f);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        for (const auto& ataque : ataquesLuz) {
            ventana.draw(ataque);
        }
    } else if (estado == GAMEOVER) {
        ventana.draw(spriteFondoGameover);
        // Configurar el texto "Game Over" de manera explícita
        textoNivel.setFont(fuente); // Asegurar que use la fuente correcta
        textoNivel.setString("Game Over"); // Establecer el texto
        textoNivel.setCharacterSize(300); // Tamaño grande, consistente con el texto de bienvenida
        textoNivel.setFillColor(sf::Color::White); // Color rojo para mayor contraste
        textoNivel.setPosition(550 , 150); // Centrado en 1920x1080
        ventana.draw(textoNivel);
        // Dibujar botón de reinicio
        ventana.draw(botonReiniciar);
        ventana.draw(textoReiniciar);
    } else if (estado == WIN) {
        ventana.draw(spriteFondoWinner);
        textoNivel.setFont(fuente);
        textoNivel.setString("WINNER");
        textoNivel.setCharacterSize(300);
        textoNivel.setFillColor(sf::Color::Green); // Verde para indicar victoria
        textoNivel.setPosition(600, 100 );
        ventana.draw(textoNivel);
        ventana.draw(botonVolverJugar);
        ventana.draw(textoVolverJugar);
        ventana.draw(botonSalir);
        ventana.draw(textoSalir);
    }
ventana.display();
}

// Inicializa NPCs y posición de jugador para tutorial
void Juego::inicializarTutorial() {
    hongos.clear();
    semillas.clear();
    npc.clear();
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
    // Generar hongos en posiciones fijas
    hongos.clear();
    std::vector<sf::Vector2f> posicionesHongos = {{1200, 300}, {1000, 600}}; // Posiciones de ejemplo
    for (const auto& pos : posicionesHongos) {
        sf::Sprite hongo(hongoTexture);
        if (hongoTexture.getSize().x == 0) { // Si no hay textura, usar círculo rojo
            cout << "hongoTexture.getSize().x" << endl;
        } else {
            hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);
            hongo.setPosition(pos);
            hongo.setScale(0.5f, 0.5f); // Ajustar tamaño si es necesario
        }
        hongos.push_back(hongo);
    }
    // Generar semillas en el tutorial
    std::vector<sf::Vector2f> posicionesSemillas = {{1300, 400}};
    for (const auto& pos : posicionesSemillas) {
        sf::Sprite semilla(semillaTexture);
        if (semillaTexture.getSize().x == 0) {
            cout << "No hay textura" << endl;
        } else {
            semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);
            semilla.setPosition(pos);
            semilla.setScale(0.5f, 0.5f);
        }
        semillas.push_back(semilla);
        posicionesOriginalesSemillas.push_back(std::make_pair(pos, false));
    }

    std::cout << "Tutorial inicializado con " << hongos.size() << " hongos y " << semillas.size() << " semillas" << std::endl;
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

void Juego::limpiarEsporas() {
    esporas.clear();
    direccionesEsporas.clear();
    rotacionesEsporas.clear();
    fuentesEsporas.clear();
    std::cout << "Esporas limpiadas al cambiar de nivel. Esporas restantes: " << esporas.size() << std::endl;
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
    jefefinal.setPosition(1500, 500);
    jefefinal.setScale(1.5f, 1.5f); // Escala normal para el jefe
    std::cout << "Rey Hongo posicionado en (1000, 800) con escala (1.0, 1.0), vida: "
              << jefefinal.getVida() << std::endl;
    oleadasJefeFinal++;
    jefefinal.controlarHormigas(npc, posicionesHormigasJefeFinal); // Generar primera oleada
    std::cout << "Primera oleada generada, NPCs: " << npc.size() << std::endl;

    // Generar hongos y guardar posiciones originales
    hongos.clear();
    posicionesOriginalesHongos.clear();
    std::vector<sf::Vector2f> posicionesHongos = {{{300, 750}, {480, 600}, {300, 200}, {100, 450}}};
    for (const auto& pos : posicionesHongos) {
        sf::Sprite hongo(hongoTexture);
        if (hongoTexture.getSize().x == 0) {
            sf::CircleShape circulo(20.f);
            circulo.setFillColor(sf::Color::Red);
            circulo.setPosition(pos);
            hongo.setPosition(pos);
        } else {
            hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);
            hongo.setPosition(pos);
            hongo.setScale(0.5f, 0.5f);
        }
        hongos.push_back(hongo);
        posicionesOriginalesHongos.push_back(std::make_pair(pos, false)); // Inicialmente no recolectados
    }


    std::cout << "Jefe Final inicializado con " << npc.size() << " hormigas y " << hongos.size() << " hongos" << std::endl;

    // Generar semillas
    std::vector<sf::Vector2f> posicionesSemillas = {{550, 600}, {720, 300}, {300, 700}};
    for (const auto& pos : posicionesSemillas) {
        sf::Sprite semilla(semillaTexture);
        if (semillaTexture.getSize().x == 0) {
            cout << "no hay textura" << endl;
        } else {
            semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);
            semilla.setPosition(pos);
            semilla.setScale(0.5f, 0.5f);
        }
        semillas.push_back(semilla);
        posicionesOriginalesSemillas.push_back(std::make_pair(pos, false));
    }


    jefefinal.setPosition(1500, 200);
    jefefinal.controlarHormigas(npc, posicionesHormigasJefeFinal);
    std::cout << "Jefe Final inicializado con " << npc.size() << " hormigas, " << hongos.size() << " hongos y " << semillas.size() << " semillas" << std::endl;
}


// Resto de métodos (vacios o mínimos)
void Juego::cargartop10() {}

void Juego::cargarnivel(int nivel) {
    npc.clear();
    std::cout << "Cargando nivel " << nivel << ", NPCs limpiados, tamaño: " << npc.size() << std::endl;

    if (nivel == 1) {
        jugador.setPosition(posicionInicialRayNivel1.x, posicionInicialRayNivel1.y);
        jugador.setScale(0.5f, 0.5f);
        jugador.restablecerVida();
        jugador.setBastonEnergia(100);
        laberintoCompletado = false;
        marcadorPuntaje.setPuntaje(0);
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
        // Crear hormigas no infectadas en el laberinto
        for (const auto& pos : posicionesHormigasNoInfectadas) {
            npc.push_back(std::make_unique<Hormigas>(false)); // false = no infectada
            npc.back()->setPosition(pos.x, pos.y);
            npc.back()->setScale(0.5f, 0.5f);
        }


        // Generar hongos en el nivel del laberinto
        hongos.clear();
        std::vector<sf::Vector2f> posicionesHongos = {{1400, 300}, {1450, 450}, {1420, 650}}; // Posiciones de ejemplo
        for (const auto& pos : posicionesHongos) {
            sf::Sprite hongo(hongoTexture);
            if (hongoTexture.getSize().x == 0) {
                cout << "no hay textura" << endl;
            } else {
                hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);
                hongo.setPosition(pos);
                hongo.setScale(0.5f, 0.5f);
            }
            // Verificar si la posición del hongo no colisiona con los muros
            sf::FloatRect hongoBounds = hongo.getGlobalBounds();
            bool colisiona = false;
            for (const auto& muro : laberintoMuros) {
                if (muro.intersects(hongoBounds)) {
                    colisiona = true;
                    break;
                }
            }
            if (!colisiona) {
                hongos.push_back(hongo);
            } else {
                std::cout << "Hongo en (" << pos.x << ", " << pos.y << ") colisiona con un muro, no se agrega" << std::endl;
            }
        }
        // Generar semillas en el nivel del laberinto
        semillas.clear();
        posicionesOriginalesSemillas.clear();
        std::vector<sf::Vector2f> posicionesSemillas = {{1550, 650}, {1450, 300}};
        for (const auto& pos : posicionesSemillas) {
            sf::Sprite semilla(semillaTexture);
            if (semillaTexture.getSize().x == 0) {
                cout  <<  "No hay textura"  << endl;
            } else {
                semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);
                semilla.setPosition(pos);
                semilla.setScale(0.5f, 0.5f);
            }
            sf::FloatRect semillaBounds = semilla.getGlobalBounds();
            bool colisiona = false;
            for (const auto& muro : laberintoMuros) {
                if (muro.intersects(semillaBounds)) {
                    colisiona = true;
                    break;
                }
            }
            if (!colisiona) {
                semillas.push_back(semilla);
                posicionesOriginalesSemillas.push_back(std::make_pair(pos, false));
            } else {
                std::cout << "Semilla en (" << pos.x << ", " << pos.y << ") colisiona con un muro, no se agrega" << std::endl;
            }
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
    std::cout << "Nivel " << nivel << " cargado, NPCs: " << npc.size() << ", Hongos: " << hongos.size() << std::endl;
}
