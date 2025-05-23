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

    if (!semillaTexture.loadFromFile("semillas.png")) {
        std::cout << "Error al cargar la textura de la semilla" << std::endl;
    }

    if (!hongoTexture.loadFromFile("hongosbiolumicentes.png")) {
        std::cout << "No se pudo cargar hongo.png, usando círculo rojo" << std::endl;
    }

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
    laberintoMuros.push_back(sf::FloatRect(0, 0, 900, 20));      // Pared superior izquierda
    laberintoMuros.push_back(sf::FloatRect(0, 500, 50, 850));    // Pared vertical izquierda
    laberintoMuros.push_back(sf::FloatRect(200, 400, 600, 50));   // Pared central
    laberintoMuros.push_back(sf::FloatRect(500, 800, 900, 50));   // Pared inferior derecha
    laberintoMuros.push_back(sf::FloatRect(1200, 900, 50, 600));  // Pared vertical derecha
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
                                sf::CircleShape bola(10.f);
                                bola.setFillColor(sf::Color::Yellow);
                                bola.setPosition(inicioX - bola.getRadius(), centroY - bola.getRadius());
                                bolas.push_back(bola);

                                sf::Vector2f direccion = posEnemigoMasCercano - sf::Vector2f(inicioX, centroY);
                                float longitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
                                if (longitud != 0) {
                                    direccion /= longitud;
                                }
                                direccionesBolas.push_back(direccion);

                                float dañoBola = (ray->getBastonEnergia() == ray->getMaxEnergia()) ? 50.0f : 20.0f;
                                dañosBolas.push_back(dañoBola);

                                int consumoEnergia = (dañoBola == 50.0f) ? 100 : 50;
                                ray->consumirEnergia(consumoEnergia);
                                std::cout << "Bola lanzada desde el lado derecho con daño: " << dañoBola << ", energía consumida: " << consumoEnergia
                                          << ", energía restante: " << ray->getBastonEnergia() << std::endl;
                            } else {
                                std::cout << "No se encontró enemigo dentro de 300.0f" << std::endl;
                            }
                        } else {
                            std::cout << "Energía insuficiente para lanzar bola (necesita 30, tiene " << ray->getBastonEnergia() << ")" << std::endl;
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
    }
}

void Juego::actualizar(float deltaTime) {
    jugador.actualizarSalto(deltaTime);
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

        std::cout << "Actualizando hormigas, NPCs: " << npc.size() << ", Estado: " << estado << std::endl;
        sf::Vector2f posicionJugador = jugador.getPosition();
        for (auto& personaje : npc) {
            std::cout << "Procesando NPC en (" << personaje->getBounds().left << ", " << personaje->getBounds().top << ")" << std::endl;
            if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                std::cout << "Hormiga detectada, infectada: " << hormiga->getInfectadas() << std::endl;
                if (hormiga->getInfectadas()) {
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

        // Recolectar hongos para restaurar energía
        sf::FloatRect rayBounds = jugador.getBounds();
        for (size_t i = 0; i < hongos.size();) {
            sf::FloatRect hongoBounds = hongos[i].getGlobalBounds();

            if (rayBounds.intersects(hongoBounds)) {
                Ray& ray = dynamic_cast<Ray&>(jugador);
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
                float vidaRecuperada = 10.0f;
                jugador.aumentarVida(static_cast<int>(vidaRecuperada));
                std::cout << "Semilla recolectada en (" << semillas[i].getPosition().x << ", " << semillas[i].getPosition().y
                          << "), vida restaurada: " << vidaRecuperada << ", nueva vida: " << jugador.getVida() << std::endl;
                for (auto& posEstado : posicionesOriginalesSemillas) {
                    if (posEstado.first == semillas[i].getPosition()) {
                        posEstado.second = true;
                        break;
                    }
                }
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
                        sf::CircleShape circulo(20.f);
                        circulo.setFillColor(sf::Color::Red);
                        circulo.setPosition(posicionesOriginalesHongos[i].first);

                        hongo.setPosition(posicionesOriginalesHongos[i].first);
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
                        cout << "Semilla llena (" << semillaTexture.getSize().x << endl;
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

        // Generar 4 nuevos hongos después de la segunda oleada en JEFEFINAL (independiente de posiciones originales)
        static bool hongosGeneradosDespuesSegundaOleada = false;
        if (estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 2 && !hongosGeneradosDespuesSegundaOleada) {
            hongosGeneradosDespuesSegundaOleada = true;
            // Definir nuevas posiciones predefinidas más adelante en el eje X
            std::vector<sf::Vector2f> nuevasPosiciones = {
                {1050, 300}, {900, 400}, {1000, 500}
            };

            for (const auto& pos : nuevasPosiciones) {
                sf::Sprite hongo(hongoTexture);
                if (hongoTexture.getSize().x == 0) {
                    cout << "no hay textura"<< endl;
                } else {
                    hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);
                    hongo.setPosition(pos);
                    hongo.setScale(0.5f, 0.5f);
                }
                hongos.push_back(hongo);
                posicionesOriginalesHongos.push_back(std::make_pair(pos, false));
                std::cout << "Nuevo hongo generado después de la segunda oleada en (" << pos.x << ", " << pos.y << ")" << std::endl;
            }
            // Nuevas semillas
            std::vector<sf::Vector2f> nuevasPosicionesSemillas = {
                {1000, 300}, {850, 400}
            };
            for (const auto& pos : nuevasPosicionesSemillas) {
                sf::Sprite semilla(semillaTexture);
                if (semillaTexture.getSize().x == 0) {
                    cout << "Semilla llena (" << semillaTexture.getSize().x << endl;
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

        // Actualizar esporas y bolas (código existente)
        for (size_t i = 0; i < esporas.size();) {
            float velocidadEspora = 100.0f;
            sf::Vector2f movimiento(direccionesEsporas[i].x * velocidadEspora * deltaTime, direccionesEsporas[i].y * velocidadEspora * deltaTime);
            esporas[i].move(movimiento);

            sf::FloatRect esporaBounds(esporas[i].getPosition().x - esporas[i].getRadius(), esporas[i].getPosition().y - esporas[i].getRadius(),
                                      esporas[i].getRadius() * 2, esporas[i].getRadius() * 2);
            if (esporaBounds.intersects(jugador.getBounds()) && !jugador.isSaltando()) {
                for (auto& personaje : npc) {
                    if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                        if (hormiga->getInfectadas()) {
                            hormiga->atacar(&jugador);
                            break;
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

            if (esporas[i].getPosition().x < 0 || esporas[i].getPosition().x > 1920 || esporas[i].getPosition().y < 0 || esporas[i].getPosition().y > 1080) {
                esporas.erase(esporas.begin() + i);
                direccionesEsporas.erase(direccionesEsporas.begin() + i);
                std::cout << "Espora salió de la pantalla" << std::endl;
                continue;
            }

            ++i;
        }

        float velocidadBola = 300.0f;
        for (size_t i = 0; i < bolas.size();) {
            sf::Vector2f movimiento = direccionesBolas[i] * velocidadBola * deltaTime;
            bolas[i].move(movimiento);

            sf::FloatRect bolaBounds(bolas[i].getPosition().x - bolas[i].getRadius(), bolas[i].getPosition().y - bolas[i].getRadius(),
                                    bolas[i].getRadius() * 2, bolas[i].getRadius() * 2);
            bool colisiono = false;

            // Colisión con hormigas
            for (auto& personaje : npc) {
                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                    if (hormiga->getInfectadas() && hormiga->getVida() > 0 && bolaBounds.intersects(hormiga->getBounds())) {
                        hormiga->recibirdano(dañosBolas[i]);
                        std::cout << "Bola impactó a Hormiga en (" << hormiga->getPosition().x << ", " << hormiga->getPosition().y
                                  << "), daño causado: " << dañosBolas[i] << ", vida restante: " << hormiga->getVida() << std::endl;
                        if (hormiga->getVida() <= 0) {
                            for (auto it = npc.begin(); it != npc.end();) {
                                if (it->get() == hormiga) {
                                    it = npc.erase(it);
                                    std::cout << "Hormiga eliminada" << std::endl;
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
            if (estado == JEFEFINAL && jefefinal.getVida() > 0 && bolaBounds.intersects(jefefinal.getBounds())) {
                jefefinal.recibirdano(dañosBolas[i]);
                std::cout << "Bola impactó a Rey Hongo en (" << jefefinal.getPosition().x << ", " << jefefinal.getPosition().y
                          << "), daño causado: " << dañosBolas[i] << ", vida restante: " << jefefinal.getVida() << std::endl;
                if (jefefinal.getVida() <= 0) {
                    estado = GAMEOVER;
                    textoNivel.setString("¡Juego Completado!");
                    std::cout << "Rey Hongo eliminado, cambiando a GAMEOVER" << std::endl;
                }
                colisiono = true;
            }

            if (colisiono || bolas[i].getPosition().x < 0 || bolas[i].getPosition().x > 1920 ||
                bolas[i].getPosition().y < 0 || bolas[i].getPosition().y > 1080) {
                bolas.erase(bolas.begin() + i);
                direccionesBolas.erase(direccionesBolas.begin() + i);
                dañosBolas.erase(dañosBolas.begin() + i);
                std::cout << "Bola eliminada" << std::endl;
                continue;
            }

            ++i;
        }
    }

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
        for (const auto& semilla : semillas) { // Dibujar semillas
            ventana.draw(semilla);
        }
        for (auto& personaje : npc) {
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);
                dibujarBarraSalud(ventana, *personaje, 50.0f);
            }
        }
        jugador.dibujar(ventana);
        dibujarBarraEnergia(ventana, dynamic_cast<Ray&>(jugador), 100.0f);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        for (const auto& bola : bolas) {
            ventana.draw(bola);
        }
        ventana.draw(textoNivel);
    } else if (estado == JUGANDO) {
        if (nivel == 1) {
            ventana.draw(spriteLaberinto);
            for (const auto& muro : laberintoMurosVisuales) {
                ventana.draw(muro);
            }
        } else {
            ventana.draw(spriteFondoTutorial);
        }
        for (const auto& hongo : hongos) {
            ventana.draw(hongo);
        }
        for (const auto& semilla : semillas) { // Dibujar semillas
            ventana.draw(semilla);
        }
        for (auto& personaje : npc) {
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);
                dibujarBarraSalud(ventana, *personaje, 50.0f);
            }
        }
        jugador.dibujar(ventana);
        dibujarBarraEnergia(ventana, dynamic_cast<Ray&>(jugador), 100.0f);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        for (const auto& bola : bolas) {
            ventana.draw(bola);
        }
        ventana.draw(textoNivel);
    } else if (estado == JEFEFINAL) {
        ventana.draw(spritefondojefefinal);
        for (const auto& hongo : hongos) {
            ventana.draw(hongo);
        }
        for (const auto& semilla : semillas) { // Dibujar semillas
            ventana.draw(semilla);
        }
        for (auto& personaje : npc) {
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);
                dibujarBarraSalud(ventana, *personaje, 50.0f);
            }
        }
        jefefinal.dibujar(ventana);
        dibujarBarraSalud(ventana, jefefinal, 200.0f);
        jugador.dibujar(ventana);
        dibujarBarraEnergia(ventana, dynamic_cast<Ray&>(jugador), 100.0f);
        dibujarBarraSalud(ventana, jugador, 100.0f);
        for (const auto& espora : esporas) {
            ventana.draw(espora);
        }
        for (const auto& bola : bolas) {
            ventana.draw(bola);
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
    // Generar hongos en posiciones fijas
    hongos.clear();
    std::vector<sf::Vector2f> posicionesHongos = {{1200, 300}, {1000, 600}}; // Posiciones de ejemplo
    for (const auto& pos : posicionesHongos) {
        sf::Sprite hongo(hongoTexture);
        if (hongoTexture.getSize().x == 0) { // Si no hay textura, usar círculo rojo
            hongo = sf::Sprite();
            sf::CircleShape circulo(20.f);
            circulo.setFillColor(sf::Color::Red);
            circulo.setPosition(pos);

            hongo.setPosition(pos);
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

    // Generar hongos y guardar posiciones originales
    hongos.clear();
    posicionesOriginalesHongos.clear();
    std::vector<sf::Vector2f> posicionesHongos = {{{300, 750}, {400, 600}, {300, 200}}};
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
    std::vector<sf::Vector2f> posicionesSemillas = {{550, 600}, {720, 300}};
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

        // Generar hongos en el nivel del laberinto
        hongos.clear();
        std::vector<sf::Vector2f> posicionesHongos = {{1400, 300}, {1450, 450}, {1420, 650}}; // Posiciones de ejemplo
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
    textoNivel.setString("Nivel " + std::to_string(nivel));
    std::cout << "Nivel " << nivel << " cargado, NPCs: " << npc.size() << ", Hongos: " << hongos.size() << std::endl;
}
