#include "reyhongo.h"
#include "hormigas.h"
#include <cmath>

#include "reyhongo.h"
#include "hormigas.h"
#include <cmath>

Reyhongo::Reyhongo()
    : Personaje("rey_hongovistasuperior.png", 200),
      energiaHormigas(0),
      enPosicionDisparo(false),
      frameActual(0),
      tiempoFrame(0.0f),
      enMovimiento(false),
      estaAtacando(false),
      tiempoAtaqueRestante(0.0f)
{
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    // Cargar texturas de animación
    texturasAnimacion.resize(2); // 0: pie derecho, 1: pie izquierdo
    if (!texturasAnimacion[0].loadFromFile("reyhongocaminandoderecha.png")) {
        std::cerr << "Error: No se pudo cargar la textura reyhongocaminandoderecha.png" << std::endl;
        throw std::runtime_error("Fallo al cargar reyhongocaminandoderecha.png");
    }
    if (!texturasAnimacion[1].loadFromFile("Reyhongocaminandoizquierda.png")) {
        std::cerr << "Error: No se pudo cargar la textura Reyhongocaminandoizquierda.png" << std::endl;
        throw std::runtime_error("Fallo al cargar Reyhongocaminandoizquierda.png");
    }
    sprite.setTexture(texturasAnimacion[0]); // Iniciar con pie derecho

    // Cargar textura de ataque
    if (!texturaAtaque.loadFromFile("reyhongoatacando.png")) {
        std::cerr << "Error: No se pudo cargar la textura reyhongoatacando.png" << std::endl;
        throw std::runtime_error("Fallo al cargar reyhongoatacando.png");
    }

    std::cout << "Reyhongo creado en posición (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << "), vida inicial: " << vida_actual << std::endl;
}

void Reyhongo::controlarHormigas(std::vector<std::unique_ptr<Personaje>>& npc, std::vector<sf::Vector2f>& posiciones) {
    std::cout << "Reyhongo: Iniciando controlarHormigas, posiciones recibidas: " << posiciones.size() << std::endl;
    npc.clear();
    std::cout << "Reyhongo: NPCs limpiados, tamaño actual: " << npc.size() << std::endl;
    if (posiciones.empty()) {
        std::cerr << "Error: El vector de posiciones está vacío, no se pueden generar hormigas" << std::endl;
        return;
    }
    for (size_t i = 0; i < posiciones.size() && i < 4; ++i) {
        std::cout << "Reyhongo: Creando hormiga " << i + 1 << " en posición ("
                  << posiciones[i].x << ", " << posiciones[i].y << ")" << std::endl;
        npc.push_back(std::make_unique<Hormigas>(true));
        if (npc.back()) {
            npc.back()->setPosition(posiciones[i].x, posiciones[i].y);
            npc.back()->setScale(1.0f, 1.0f);
            std::cout << "Reyhongo: Hormiga infectada creada en (" << posiciones[i].x << ", "
                      << posiciones[i].y << ") con escala (1.0, 1.0), vida: "
                      << npc.back()->getVida() << std::endl;
        } else {
            std::cerr << "Error: No se pudo crear hormiga en posición (" << posiciones[i].x << ", "
                      << posiciones[i].y << ")" << std::endl;
        }
    }
    std::cout << "Reyhongo: Hormigas generadas, NPCs: " << npc.size() << std::endl;
}

sf::FloatRect Reyhongo::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float scaleReduction = 0.70f; // Igual que Hormigas para consistencia
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

void Reyhongo::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    frameActual = 0;
    sprite.setTexture(texturasAnimacion[0]);
    std::cout << "Reyhongo posicionado en (" << x << ", " << y << ")" << std::endl;
}

void Reyhongo::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de Reyhongo ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

void Reyhongo::actualizar(float deltaTime) {
    if (estaAtacando) {
        tiempoAtaqueRestante -= deltaTime;
        if (tiempoAtaqueRestante <= 0.0f) {
            estaAtacando = false;
            if (enMovimiento) {
                sprite.setTexture(texturasAnimacion[frameActual]); // Volver a la animación de movimiento
            } else {
                sprite.setTexture(texturasAnimacion[0]); // Volver a pie derecho si no se mueve
            }
            std::cout << "Reyhongo terminó animación de ataque, volviendo a frame " << frameActual << std::endl;
        }
    }
    if (!enMovimiento && !estaAtacando) {
        frameActual = 0; // Volver a pie derecho si no se mueve
        sprite.setTexture(texturasAnimacion[0]);
        tiempoFrame = 0.0f;
    }
}

void Reyhongo::atacar(Personaje* objetivo) {
    if (!estaAtacando) { // Evitar ataques múltiples simultáneos
        objetivo->recibirdano(20); // Daño fijo del Reyhongo
        estaAtacando = true;
        tiempoAtaqueRestante = duracionAtaque;
        sprite.setTexture(texturaAtaque); // Cambiar a la textura de ataque
        std::cout << "Reyhongo ataca, causando 20 de daño. Estado: estaAtacando = " << estaAtacando << ", tiempoAtaqueRestante = " << tiempoAtaqueRestante << std::endl;
    }
}

void Reyhongo::moverHaciaJugador( sf::Vector2f& posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse) {
    // Calcular vector de dirección hacia el jugador
    sf::Vector2f posicionActual = sprite.getPosition();
    sf::Vector2f direccion = posicionJugador - posicionActual;
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    std::cout << "Reyhongo en (" << posicionActual.x << ", " << posicionActual.y
              << "), Jugador en (" << posicionJugador.x << ", " << posicionJugador.y
              << "), Distancia: " << distancia << ", deltaTime: " << deltaTime << std::endl;

    // Normalizar dirección
    if (distancia > 0) {
        direccion.x /= distancia;
        direccion.y /= distancia;
    } else {
        std::cout << "Distancia cero, no se mueve" << std::endl;
        return;
    }

    // Determinar si moverse hacia el jugador o detenerse
    float movimiento = 0.0f;
    if (distancia > distanciaObjetivo + 10.0f) {
        movimiento = velocidad * deltaTime; // Mover hacia el jugador
        enPosicionDisparo = false;
        enMovimiento = true; // Activar animación
        std::cout << "Moviendo hacia jugador, movimiento: " << movimiento << std::endl;
    } else {
        enPosicionDisparo = true;
        enMovimiento = false; // Desactivar animación
        std::cout << "En posición de disparo, no se mueve" << std::endl;
        return;
    }

    // Calcular desplazamiento
    float dx = direccion.x * movimiento;
    float dy = direccion.y * movimiento;

    // Verificar si el movimiento es válido
    float newX = posicionActual.x + dx;
    float newY = posicionActual.y + dy;
    bool movimientoValido = puedeMoverse(newX, newY);
    std::cout << "Intentando mover a (" << newX << ", " << newY << "), PuedeMoverse: " << (movimientoValido ? "true" : "false") << std::endl;

    if (movimientoValido) {
        sprite.move(dx, dy);
        std::cout << "Reyhongo movida a (" << newX << ", " << newY << ")" << std::endl;
    } else {
        std::cout << "Movimiento bloqueado por colisión" << std::endl;
    }
    // Actualizar animación
    tiempoFrame += deltaTime;
    if (enMovimiento && tiempoFrame >= duracionFrame) {
        tiempoFrame = 0.0f;
        frameActual = (frameActual + 1) % 2; // Alternar entre 0 (derecha) y 1 (izquierda)
        sprite.setTexture(texturasAnimacion[frameActual]);
        std::cout << "Cambiando a frame " << frameActual << " ("
                  << (frameActual == 0 ? "pie derecho" : "pie izquierdo")
                  << ")" << std::endl;
    }
}


bool Reyhongo::estaEnPosicionDisparo()  {
    float tiempoTranscurrido = relojDisparo.getElapsedTime().asSeconds();
    std::cout << "Reyhongo::estaEnPosicionDisparo - Tiempo transcurrido: " << tiempoTranscurrido << " segundos" << std::endl;
    return enPosicionDisparo && tiempoTranscurrido >= 1.3f;
}

void Reyhongo::dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador, sf::Texture& esporaTexture) {
    if (!estaEnPosicionDisparo()) {
        std::cout << "Reyhongo no dispara: no está en posición o temporizador no listo" << std::endl;
        return;
    }
    sf::Sprite espora;
        if (esporaTexture.getSize().x == 0) {
            cout << "No hay textura" << endl;
        } else {
            espora.setTexture(esporaTexture);
            espora.setOrigin(esporaTexture.getSize().x / 2.0f, esporaTexture.getSize().y / 2.0f);
            // Calcular posición inicial: lado izquierdo de la hormiga
            sf::Vector2f posicionJefefinal = sprite.getPosition();
            sf::Vector2f escalaJefefinal = sprite.getScale();
            float anchoJefefinal = sprite.getLocalBounds().width;

            // Parámetros configurables
            float offsetX = 1.0f; // Ajuste adicional en X (negativo para más a la izquierda, positivo para más a la derecha)
            float offsetY = 1.0f; // Ajuste adicional en Y (negativo para arriba, positivo para abajo)

            // Calcular el lado izquierdo teniendo en cuenta la escala
            float desplazamientoIzquierdo = -(anchoJefefinal / 2.0f) * escalaJefefinal.x + offsetX;
            float desplazamientoVertical = offsetY;

            // Nueva posición de la espora (lado izquierdo de la hormiga)
            sf::Vector2f posicionEspora(posicionJefefinal.x + desplazamientoIzquierdo, posicionJefefinal.y + desplazamientoVertical);
            espora.setPosition(posicionEspora);
            espora.setScale(0.5f, 0.5f); // Ajustar tamaño de la espora

        }

    // Ajustar la dirección usando la posición actual de la espora
    sf::Vector2f posicionEspora = espora.getPosition();
    sf::FloatRect jugadorBounds = jugador->getBounds();
    sf::Vector2f centroJugador(jugadorBounds.left + jugadorBounds.width / 2.0f,
                               jugadorBounds.top + jugadorBounds.height / 2.0f);

    // Calcular dirección hacia el centro del jugador desde la posición de la espora
    sf::Vector2f direccion(centroJugador.x - posicionEspora.x, centroJugador.y - posicionEspora.y);
    float magnitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    if (magnitud > 0) {
        direccion.x /= magnitud;
        direccion.y /= magnitud;
    }

    esporas.push_back(espora);
    direccionesEsporas.push_back(direccion);
    rotacionesEsporas.push_back(0.0f); // Inicializar rotación en 0 grados
    estaAtacando = true;
    tiempoAtaqueRestante = duracionAtaque;
    sprite.setTexture(texturaAtaque); // Cambiar a la textura de ataque
    relojDisparo.restart();
    std::cout << "Espora disparada desde (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ") hacia (" << jugador->getPosition().x << ", " << jugador->getPosition().y << "). Estado: estaAtacando = " << estaAtacando << std::endl;
}

void Reyhongo::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
    std::cout << "Dibujando Reyhongo en (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << ") con estado " << (estaAtacando ? "atacando" : "normal") << std::endl;
}