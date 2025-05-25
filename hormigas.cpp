#include "hormigas.h"
#include <SFML/Graphics.hpp>
using namespace std;

Hormigas::Hormigas(bool infectadas, sf::Vector2f posicion)
    : Personaje(infectadas ? "hormigainfectadavistasuperior.png" : "Npcparado.png", 50),
      infectadas(infectadas),
      enPosicionDisparo(false),
      frameActual(0),
      tiempoFrame(0.0f),
      enMovimiento(false)
{
    sprite.setPosition(posicion);
    sprite.setScale(1.0f, 1.0f);
    posicion_x = posicion.x;
    posicion_y = posicion.y;
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    // Cargar texturas de animación solo para hormigas infectadas
    if (infectadas) {
        texturasAnimacion.resize(2); // 0: pie derecho, 1: pie izquierdo
        if (!texturasAnimacion[0].loadFromFile("npcinfectadocaminandoderecha.png")) {
            std::cerr << "Error: No se pudo cargar la textura npcinfectadocaminandoderecha.png" << std::endl;
            throw std::runtime_error("Fallo al cargar npcinfectadocaminandoderecha.png");
        }
        if (!texturasAnimacion[1].loadFromFile("npcinfectadocaminandoizquierda.png")) {
            std::cerr << "Error: No se pudo cargar la textura npcinfectadocaminandoizquierda.png" << std::endl;
            throw std::runtime_error("Fallo al cargar npcinfectadocaminandoizquierda.png");
        }
        sprite.setTexture(texturasAnimacion[0]); // Iniciar con pie derecho
        // Cargar textura de ataque
        if (!texturaAtaque.loadFromFile("npcinfectadoatacando.png")) {
            std::cerr << "Error: No se pudo cargar la textura npcinfectadoatacando.png" << std::endl;
            throw std::runtime_error("Fallo al cargar npcinfectadoatacando.png");
        }
    }

    std::cout << "Hormiga creada (" << (infectadas ? "infectada" : "no infectada")
              << ") en posición (" << posicion_x << ", " << posicion_y << "), vida inicial: "
              << vida_actual << std::endl;
}

void Hormigas::moverHaciaJugador(sf::Vector2f posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse) {
    if (!infectadas) {
        std::cout << "Hormiga no infectada, no se mueve" << std::endl;
        return;
    }

    // Calcular vector de dirección hacia el jugador
    sf::Vector2f posicionActual = sprite.getPosition();
    sf::Vector2f direccion = posicionJugador - posicionActual;
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    std::cout << "Hormiga en (" << posicionActual.x << ", " << posicionActual.y
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
        enMovimiento = false;
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
        posicion_x = newX;
        posicion_y = newY;
        std::cout << "Hormiga movida a (" << newX << ", " << newY << ")" << std::endl;
    } else {
        std::cout << "Movimiento bloqueado por colisión" << std::endl;
    }
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


void Hormigas::atacar(Personaje* objetivo) {
    if (infectadas) {
        objetivo->recibirdano(10);
        estaAtacando = true;
        tiempoAtaqueRestante = duracionAtaque;
        sprite.setTexture(texturaAtaque); // Cambiar a la textura de ataque
}
}

void Hormigas::actualizar(float deltaTime) {
    if (estaAtacando) {
        tiempoAtaqueRestante -= deltaTime;
        if (tiempoAtaqueRestante <= 0.0f) {
            estaAtacando = false;
            if (enMovimiento) {
                sprite.setTexture(texturasAnimacion[frameActual]); // Volver a la animación de movimiento
            } else {
                sprite.setTexture(texturasAnimacion[0]); // Volver a pie derecho si no se mueve
            }
            std::cout << "Hormiga terminó animación de ataque, volviendo a frame " << frameActual << std::endl;
        }
    }
}

bool Hormigas::getInfectadas(){
    return infectadas;
}
void Hormigas::setPosition(float x, float y) {
    sprite.setPosition(x,y);
    frameActual = 0;
    if (infectadas) {
        sprite.setTexture(texturasAnimacion[0]);
    }
    std::cout << "Hormiga posicionada en (" << x << ", " << y << ")" << std::endl;
}


void Hormigas::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de hormiga ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

sf::FloatRect Hormigas::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float scaleReduction = 0.5f;// Reducir el tamaño para colisiones más precisas
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    sf::FloatRect adjustedBounds(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
    std::cout << "Hormiga bounds: [" << adjustedBounds.left << ", " << adjustedBounds.top << ", "
              << adjustedBounds.width << ", " << adjustedBounds.height << "]" << std::endl;
    return adjustedBounds;
}

void Hormigas::dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador,  sf::Texture& esporaTexture) {
    if (relojDisparo.getElapsedTime().asSeconds() < 3.0f) return;
    sf::Sprite espora;
    if (esporaTexture.getSize().x == 0) {
        cout << "no hay textura" <<endl;
    } else {
        espora.setTexture(esporaTexture);
        espora.setOrigin(esporaTexture.getSize().x / 2.0f, esporaTexture.getSize().y / 2.0f);

        // Calcular posición inicial: lado izquierdo de la hormiga
        sf::Vector2f posicionHormiga = sprite.getPosition();
        sf::Vector2f escalaHormiga = sprite.getScale();
        float anchoHormiga = sprite.getLocalBounds().width;

        // Parámetros configurables
        float offsetX = 0.0f; // Ajuste adicional en X (negativo para más a la izquierda, positivo para más a la derecha)
        float offsetY = 0.0f; // Ajuste adicional en Y (negativo para arriba, positivo para abajo)

        // Calcular el lado izquierdo teniendo en cuenta la escala
        float desplazamientoIzquierdo = -(anchoHormiga / 2.0f) * escalaHormiga.x + offsetX;
        float desplazamientoVertical = offsetY;

        // Nueva posición de la espora (lado izquierdo de la hormiga)
        sf::Vector2f posicionEspora(posicionHormiga.x + desplazamientoIzquierdo, posicionHormiga.y + desplazamientoVertical);
        espora.setPosition(posicionEspora);
        espora.setScale(0.5f, 0.5f); // Ajustar tamaño de la espora
    }

    // Obtener el centro del sprite del jugador (Ray)
    sf::FloatRect jugadorBounds = jugador->getBounds();
    sf::Vector2f centroJugador(jugadorBounds.left + jugadorBounds.width / 2.0f,
                               jugadorBounds.top + jugadorBounds.height / 2.0f);

    // Calcular dirección hacia el centro del jugador
    sf::Vector2f direccion(centroJugador.x - sprite.getPosition().x, centroJugador.y - sprite.getPosition().y);
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
    sprite.setTexture(texturaAtaque); // Cambiar a la textura de ataque inmediatamente
    relojDisparo.restart();
    std::cout << "Espora disparada desde (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ") hacia (" << jugador->getPosition().x << ", " << jugador->getPosition().y << ")" << std::endl;
}


void Hormigas::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
    std::cout << "Dibujando hormiga en (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << ") con estado " << (estaAtacando ? "atacando" : "normal") << std::endl;
}


