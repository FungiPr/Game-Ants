#include "reyhongo.h" // Incluye la definición de la clase Reyhongo.
#include "hormigas.h" // Incluye la definición de la clase Hormigas, ya que Reyhongo las gestiona.
#include <cmath>      // Necesario para funciones matemáticas como std::sqrt.

// Constructor de la clase Reyhongo.
// Inicializa al Reyhongo con su textura por defecto y vida máxima, y configura sus estados y animaciones.
Reyhongo::Reyhongo()
    : Personaje("rey_hongovistasuperior.png", 200), // Llama al constructor de la clase base 'Personaje'.
      energiaHormigas(0),         // Inicializa la energía usada para controlar hormigas.
      enPosicionDisparo(false),   // Indica si el Reyhongo está en una posición para disparar.
      frameActual(0),             // El índice del frame de animación actual (0 o 1).
      tiempoFrame(0.0f),          // Controla el tiempo transcurrido para el cambio de frame.
      enMovimiento(false),        // Bandera que indica si el Reyhongo se está moviendo.
      estaAtacando(false),        // Bandera que indica si el Reyhongo está realizando un ataque.
      tiempoAtaqueRestante(0.0f)  // Contador para la duración de la animación de ataque.
{
    // Centra el origen del sprite para facilitar transformaciones (rotación, escalado).
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    // Carga las texturas para la animación de movimiento (pie derecho/izquierdo).
    texturasAnimacion.resize(2); // Ajusta el tamaño para dos frames de animación.
    if (!texturasAnimacion[0].loadFromFile("reyhongooficialcaminandoderecha.png")) {
        std::cerr << "Error: No se pudo cargar la textura reyhongooficialcaminandoderecha.png" << std::endl;
        throw std::runtime_error("Fallo al cargar reyhongooficialcaminandoderecha.png"); // Lanza error si falla la carga.
    }
    if (!texturasAnimacion[1].loadFromFile("rey_hongocaminandodizquierdas.png")) {
        std::cerr << "Error: No se pudo cargar la textura rey_hongocaminandodizquierdas.png" << std::endl;
        throw std::runtime_error("Fallo al cargar rey_hongocaminandodizquierdas.png"); // Lanza error si falla la carga.
    }
    sprite.setTexture(texturasAnimacion[0]); // Establece la textura inicial (pie derecho).

    // Carga la textura específica para la animación de ataque.
    if (!texturaAtaque.loadFromFile("rey_hongoatacando.png")) {
        std::cerr << "Error: No se pudo cargar la textura rey_hongoatacando.png" << std::endl;
        throw std::runtime_error("Fallo al cargar rey_hongoatacando.png"); // Lanza error si falla la carga.
    }

    // Mensaje de depuración al crear el Reyhongo.
    std::cout << "Reyhongo creado en posición (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << "), vida inicial: " << vida_actual << std::endl;
}

// Controla la generación y gestión de las hormigas aliadas del Reyhongo.
// Limpia las hormigas existentes y crea nuevas en posiciones predefinidas.
void Reyhongo::controlarHormigas(std::vector<std::unique_ptr<Personaje>>& npc, std::vector<sf::Vector2f>& posiciones) {
    std::cout << "Reyhongo: Iniciando controlarHormigas, posiciones recibidas: " << posiciones.size() << std::endl;
    npc.clear(); // Limpia la lista de NPCs (hormigas) existentes.
    std::cout << "Reyhongo: NPCs limpiados, tamaño actual: " << npc.size() << std::endl;
    if (posiciones.empty()) { // Si no hay posiciones para las hormigas, muestra un error.
        std::cerr << "Error: El vector de posiciones está vacío, no se pueden generar hormigas" << std::endl;
        return;
    }
    // Itera sobre las posiciones y crea nuevas hormigas infectadas.
    for (size_t i = 0; i < posiciones.size() && i < 4; ++i) { // Limita la creación a un máximo de 4 hormigas.
        std::cout << "Reyhongo: Creando hormiga " << i + 1 << " en posición ("
                  << posiciones[i].x << ", " << posiciones[i].y << ")" << std::endl;
        npc.push_back(std::make_unique<Hormigas>(true)); // Añade una nueva Hormiga infectada.
        if (npc.back()) { // Si la hormiga se creó correctamente.
            npc.back()->setPosition(posiciones[i].x, posiciones[i].y); // Posiciona la hormiga.
            npc.back()->setScale(1.0f, 1.0f); // Ajusta la escala de la hormiga.
            std::cout << "Reyhongo: Hormiga infectada creada en (" << posiciones[i].x << ", "
                      << posiciones[i].y << ") con escala (1.0, 1.0), vida: "
                      << npc.back()->getVida() << std::endl;
        } else {
            std::cerr << "Error: No se pudo crear hormiga en posición (" << posiciones[i].x << ", "
                      << posiciones[i].y << ")" << std::endl; // Muestra error si falla la creación.
        }
    }
    std::cout << "Reyhongo: Hormigas generadas, NPCs: " << npc.size() << std::endl;
}

// Obtiene los límites de colisión del Reyhongo.
// Reduce el tamaño de los límites globales para una colisión más precisa.
sf::FloatRect Reyhongo::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds(); // Obtiene los límites globales del sprite.
    float scaleReduction = 0.70f; // Factor de reducción para los límites de colisión.
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f; // Desplazamiento para centrar los nuevos límites.
    float offsetY = (bounds.height - newHeight) / 2.0f;
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

// Establece la posición del Reyhongo en el juego.
// También reinicia la animación de movimiento al primer frame.
void Reyhongo::setPosition(float x, float y) {
    sprite.setPosition(x, y); // Establece la nueva posición del sprite.
    frameActual = 0; // Reinicia el frame de animación.
    sprite.setTexture(texturasAnimacion[0]); // Establece la textura inicial de animación.
    std::cout << "Reyhongo posicionado en (" << x << ", " << y << ")" << std::endl;
}

// Establece la escala del sprite del Reyhongo.
void Reyhongo::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY); // Aplica la nueva escala al sprite.
    std::cout << "Escala de Reyhongo ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

// Actualiza el estado del Reyhongo, incluyendo el manejo de la animación de ataque.
void Reyhongo::actualizar(float deltaTime) {
    if (estaAtacando) { // Si el Reyhongo está en estado de ataque.
        tiempoAtaqueRestante -= deltaTime; // Reduce el tiempo restante del ataque.
        if (tiempoAtaqueRestante <= 0.0f) { // Si el tiempo de ataque ha terminado.
            estaAtacando = false; // Desactiva el estado de ataque.
            if (enMovimiento) {
                sprite.setTexture(texturasAnimacion[frameActual]); // Vuelve a la animación de movimiento.
            } else {
                sprite.setTexture(texturasAnimacion[0]); // Si no se mueve, vuelve al frame inicial (pie derecho).
            }
            std::cout << "Reyhongo terminó animación de ataque, volviendo a frame " << frameActual << std::endl;
        }
    }
    // Si no se está moviendo y no está atacando, asegura que la animación esté en el frame inicial.
    if (!enMovimiento && !estaAtacando) {
        frameActual = 0; // Reinicia el frame a pie derecho.
        sprite.setTexture(texturasAnimacion[0]);
        tiempoFrame = 0.0f; // Reinicia el contador de tiempo del frame.
    }
}

// Realiza una acción de ataque contra un objetivo.
// Aplica daño al objetivo y activa la animación de ataque del Reyhongo.
void Reyhongo::atacar(Personaje* objetivo) {
    if (!estaAtacando) { // Solo permite un ataque si no está ya atacando.
        objetivo->recibirdano(20); // El objetivo recibe un daño fijo de 20.
        estaAtacando = true; // Activa el estado de ataque.
        tiempoAtaqueRestante = duracionAtaque; // Establece la duración de la animación de ataque.
        sprite.setTexture(texturaAtaque); // Cambia la textura a la de ataque.
        std::cout << "Reyhongo ataca, causando 20 de daño. Estado: estaAtacando = " << estaAtacando << ", tiempoAtaqueRestante = " << tiempoAtaqueRestante << std::endl;
    }
}

// Mueve al Reyhongo hacia la posición del jugador, evitando colisiones.
// También gestiona la animación de movimiento.
void Reyhongo::moverHaciaJugador( sf::Vector2f& posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse) {
    sf::Vector2f posicionActual = sprite.getPosition(); // Posición actual del Reyhongo.
    sf::Vector2f direccion = posicionJugador - posicionActual; // Vector de dirección hacia el jugador.
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y); // Distancia al jugador.

    std::cout << "Reyhongo en (" << posicionActual.x << ", " << posicionActual.y
              << "), Jugador en (" << posicionJugador.x << ", " << posicionJugador.y
              << "), Distancia: " << distancia << ", deltaTime: " << deltaTime << std::endl;

    if (distancia > 0) { // Normaliza el vector de dirección si la distancia no es cero.
        direccion.x /= distancia;
        direccion.y /= distancia;
    } else {
        std::cout << "Distancia cero, no se mueve" << std::endl;
        return; // No se mueve si ya está en la misma posición.
    }

    float movimiento = 0.0f;
    // Si la distancia al jugador es mayor que la distancia objetivo, el Reyhongo se mueve.
    if (distancia > distanciaObjetivo + 10.0f) {
        movimiento = velocidad * deltaTime; // Calcula el desplazamiento basado en la velocidad y el tiempo.
        enPosicionDisparo = false; // No está en posición de disparo si se está moviendo.
        enMovimiento = true; // Activa la bandera de movimiento para la animación.
        std::cout << "Moviendo hacia jugador, movimiento: " << movimiento << std::endl;
    } else { // Si está cerca del jugador, se detiene y entra en posición de disparo.
        enPosicionDisparo = true;
        enMovimiento = false; // Desactiva el movimiento.
        std::cout << "En posición de disparo, no se mueve" << std::endl;
        return;
    }

    float dx = direccion.x * movimiento; // Desplazamiento en X.
    float dy = direccion.y * movimiento; // Desplazamiento en Y.

    float newX = posicionActual.x + dx; // Nueva posición X tentativa.
    float newY = posicionActual.y + dy; // Nueva posición Y tentativa.
    // Verifica si el movimiento es válido (no hay colisiones con muros).
    bool movimientoValido = puedeMoverse(newX, newY);
    std::cout << "Intentando mover a (" << newX << ", " << newY << "), PuedeMoverse: " << (movimientoValido ? "true" : "false") << std::endl;

    if (movimientoValido) {
        sprite.move(dx, dy); // Aplica el movimiento si es válido.
        std::cout << "Reyhongo movida a (" << newX << ", " << newY << ")" << std::endl;
    } else {
        std::cout << "Movimiento bloqueado por colisión" << std::endl;
    }

    // Actualiza la animación de movimiento.
    tiempoFrame += deltaTime; // Acumula el tiempo para el cambio de frame.
    if (enMovimiento && tiempoFrame >= duracionFrame) { // Si está moviéndose y es hora de cambiar de frame.
        tiempoFrame = 0.0f; // Reinicia el contador de tiempo del frame.
        frameActual = (frameActual + 1) % 2; // Alterna entre el frame 0 y el 1.
        sprite.setTexture(texturasAnimacion[frameActual]); // Cambia la textura del sprite.
        std::cout << "Cambiando a frame " << frameActual << " ("
                  << (frameActual == 0 ? "pie derecho" : "pie izquierdo")
                  << ")" << std::endl;
    }
}

// Verifica si el Reyhongo está en posición para disparar y si su temporizador lo permite.
bool Reyhongo::estaEnPosicionDisparo() {
    float tiempoTranscurrido = relojDisparo.getElapsedTime().asSeconds(); // Tiempo desde el último disparo.
    std::cout << "Reyhongo::estaEnPosicionDisparo - Tiempo transcurrido: " << tiempoTranscurrido << " segundos" << std::endl;
    // Retorna true si está en posición de disparo y ha pasado suficiente tiempo desde el último disparo.
    return enPosicionDisparo && tiempoTranscurrido >= 1.3f;
}

// Dispara una espora hacia la posición del jugador.
// Crea y configura un nuevo sprite de espora y calcula su dirección.
void Reyhongo::dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador, sf::Texture& esporaTexture) {
    if (!estaEnPosicionDisparo()) { // Si no está listo para disparar, sale de la función.
        std::cout << "Reyhongo no dispara: no está en posición o temporizador no listo" << std::endl;
        return;
    }
    sf::Sprite espora; // Crea un nuevo sprite para la espora.
    if (esporaTexture.getSize().x == 0) { // Verifica si la textura de la espora se cargó correctamente.
        cout << "No hay textura" << endl;
    } else {
        espora.setTexture(esporaTexture); // Asigna la textura a la espora.
        // Centra el origen del sprite de la espora.
        espora.setOrigin(esporaTexture.getSize().x / 2.0f, esporaTexture.getSize().y / 2.0f);

        // Calcula la posición inicial de la espora (lado izquierdo del Reyhongo).
        sf::Vector2f posicionJefefinal = sprite.getPosition();
        sf::Vector2f escalaJefefinal = sprite.getScale();
        float anchoJefefinal = sprite.getLocalBounds().width;

        float offsetX = 1.0f; // Ajuste adicional en X.
        float offsetY = 1.0f; // Ajuste adicional en Y.

        // Calcula el desplazamiento para posicionar la espora al lado izquierdo del Reyhongo.
        float desplazamientoIzquierdo = -(anchoJefefinal / 2.0f) * escalaJefefinal.x + offsetX;
        float desplazamientoVertical = offsetY;

        sf::Vector2f posicionEspora(posicionJefefinal.x + desplazamientoIzquierdo, posicionJefefinal.y + desplazamientoVertical);
        espora.setPosition(posicionEspora); // Establece la posición de la espora.
        espora.setScale(0.5f, 0.5f); // Ajusta el tamaño de la espora.
    }

    sf::Vector2f posicionEspora = espora.getPosition(); // Obtiene la posición final de la espora.
    sf::FloatRect jugadorBounds = jugador->getBounds(); // Obtiene los límites del jugador.
    sf::Vector2f centroJugador(jugadorBounds.left + jugadorBounds.width / 2.0f,
                               jugadorBounds.top + jugadorBounds.height / 2.0f); // Calcula el centro del jugador.

    // Calcula la dirección de la espora hacia el centro del jugador.
    sf::Vector2f direccion(centroJugador.x - posicionEspora.x, centroJugador.y - posicionEspora.y);
    float magnitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    if (magnitud > 0) { // Normaliza la dirección.
        direccion.x /= magnitud;
        direccion.y /= magnitud;
    }

    esporas.push_back(espora); // Añade la espora a la lista de esporas activas.
    direccionesEsporas.push_back(direccion); // Añade la dirección de la espora.
    rotacionesEsporas.push_back(0.0f); // Inicializa la rotación de la espora.
    estaAtacando = true; // Activa el estado de ataque del Reyhongo.
    tiempoAtaqueRestante = duracionAtaque; // Reinicia el temporizador de ataque.
    sprite.setTexture(texturaAtaque); // Cambia la textura del Reyhongo a la de ataque.
    relojDisparo.restart(); // Reinicia el temporizador de disparo.
    std::cout << "Espora disparada desde (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ") hacia (" << jugador->getPosition().x << ", " << jugador->getPosition().y << "). Estado: estaAtacando = " << estaAtacando << std::endl;
}

// Dibuja el sprite del Reyhongo en la ventana del juego.
void Reyhongo::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite); // Dibuja el sprite del Reyhongo.
    std::cout << "Dibujando Reyhongo en (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << ") con estado " << (estaAtacando ? "atacando" : "normal") << std::endl;
}