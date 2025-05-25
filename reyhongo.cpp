#include "reyhongo.h"
#include "hormigas.h"
#include <cmath>

Reyhongo::Reyhongo()
    : Personaje("rey_hongovistasuperior.png", 200),
      energiaHormigas(0),
      enPosicionDisparo(false)
{
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    std::cout << "Reyhongo creado en posición (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << "), vida inicial: " << vida_actual << std::endl;
}

void Reyhongo::atacar(Personaje* objetivo) {
    objetivo->recibirdano(20); // Daño fijo del Reyhongo
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
}

void Reyhongo::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de Reyhongo ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

void Reyhongo::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
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
        std::cout << "Moviendo hacia jugador, movimiento: " << movimiento << std::endl;
    } else {
        enPosicionDisparo = true;
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
            espora.setPosition(sprite.getPosition());
            espora.setScale(0.5f, 0.5f); // Ajustar tamaño de la espora
        }

    // Ajustar la posición inicial al centro del sprite del Reyhongo
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f centroReyhongo(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    espora.setPosition(centroReyhongo);

    // Obtener el centro del sprite del jugador (Ray)
    sf::FloatRect jugadorBounds = jugador->getBounds();
    sf::Vector2f centroJugador(jugadorBounds.left + jugadorBounds.width / 2.0f,
                               jugadorBounds.top + jugadorBounds.height / 2.0f);

    // Calcular dirección hacia el centro del jugador
    sf::Vector2f direccion(centroJugador.x - centroReyhongo.x, centroJugador.y - centroReyhongo.y);
    float magnitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    if (magnitud > 0) {
        direccion.x /= magnitud;
        direccion.y /= magnitud;
    }

    esporas.push_back(espora);
    direccionesEsporas.push_back(direccion);
    rotacionesEsporas.push_back(0.0f); // Inicializar rotación en 0 grados
    relojDisparo.restart();
    std::cout << "Espora disparada desde (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ") hacia (" << jugador->getPosition().x << ", " << jugador->getPosition().y << ")" << std::endl;
}
