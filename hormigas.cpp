#include "hormigas.h" // Incluye la definición de la clase Hormigas.
#include <SFML/Graphics.hpp> // Incluye la biblioteca gráfica SFML para el manejo de sprites y ventanas.
#include <cmath> // Necesario para funciones matemáticas como std::sqrt para cálculos de distancia.


// Constructor de la clase Hormigas.
// Inicializa una nueva instancia de Hormiga, determinando su tipo (infectada/no infectada),
// su posición inicial y atributos de vida.
Hormigas::Hormigas(bool infectadas, sf::Vector2f posicion)
    // Llama al constructor de la clase base 'Personaje'. La textura y la vida inicial
    // varían si la hormiga está infectada o no.
    : Personaje(infectadas ? "hormigainfectadavistasuperior.png" : "Npcparado.png", 50),
      infectadas(infectadas), // Establece el estado de infección de la hormiga.
      enPosicionDisparo(false), // Bandera para controlar si la hormiga está en posición de disparar esporas.
      frameActual(0), // Índice del frame actual para la animación de movimiento.
      tiempoFrame(0.0f), // Temporizador para controlar la velocidad de la animación de los frames.
      enMovimiento(false) // Bandera que indica si la hormiga se está moviendo actualmente.
{
    sprite.setPosition(posicion); // Establece la posición inicial del sprite de la hormiga.
    sprite.setScale(1.0f, 1.0f); // Define la escala inicial del sprite (tamaño normal).
    posicion_x = posicion.x; // Almacena la coordenada X de la posición.
    posicion_y = posicion.y; // Almacena la coordenada Y de la posición.
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2); // Centra el origen del sprite.

    // Carga texturas de animación y ataque solo para hormigas infectadas,
    // ya que las no infectadas suelen ser estáticas o tener un comportamiento diferente.
    if (infectadas) {
        texturasAnimacion.resize(2); // Prepara espacio para dos texturas de animación (pie derecho y pie izquierdo).
        // Carga la textura para el frame de movimiento del pie derecho.
        if (!texturasAnimacion[0].loadFromFile("npcinfectadocaminandoderecha.png")) {
            std::cerr << "Error: No se pudo cargar la textura npcinfectadocaminandoderecha.png" << std::endl;
            throw std::runtime_error("Fallo al cargar npcinfectadocaminandoderecha.png"); // Lanza un error si la carga falla.
        }
        // Carga la textura para el frame de movimiento del pie izquierdo.
        if (!texturasAnimacion[1].loadFromFile("npcinfectadocaminandoizquierda.png")) {
            std::cerr << "Error: No se pudo cargar la textura npcinfectadocaminandoizquierda.png" << std::endl;
            throw std::runtime_error("Fallo al cargar npcinfectadocaminandoizquierda.png"); // Lanza un error si la carga falla.
        }
        sprite.setTexture(texturasAnimacion[0]); // Establece la textura inicial del sprite (pie derecho).

        // Carga la textura específica para el estado de ataque de la hormiga infectada.
        if (!texturaAtaque.loadFromFile("npcinfectadoatacando.png")) {
            std::cerr << "Error: No se pudo cargar la textura npcinfectadoatacando.png" << std::endl;
            throw std::runtime_error("Fallo al cargar npcinfectadoatacando.png"); // Lanza un error si la carga falla.
        }
    }

    // Mensaje de depuración que informa sobre la creación de la hormiga y su estado inicial.
    std::cout << "Hormiga creada (" << (infectadas ? "infectada" : "no infectada")
              << ") en posición (" << posicion_x << ", " << posicion_y << "), vida inicial: "
              << vida_actual << std::endl;
}

// Mueve la hormiga hacia la posición del jugador, si la hormiga está infectada.
// La hormiga se detiene a una distancia predefinida del jugador y gestiona su animación de movimiento.
void Hormigas::moverHaciaJugador(sf::Vector2f posicionJugador, float deltaTime, std::function<bool(float, float)> puedeMoverse) {
    if (!infectadas) { // Las hormigas no infectadas no tienen comportamiento de movimiento hacia el jugador.
        std::cout << "Hormiga no infectada, no se mueve" << std::endl;
        return;
    }

    // Calcula el vector de dirección y la distancia hacia el jugador desde la posición actual de la hormiga.
    sf::Vector2f posicionActual = sprite.getPosition();
    sf::Vector2f direccion = posicionJugador - posicionActual;
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    std::cout << "Hormiga en (" << posicionActual.x << ", " << posicionActual.y
              << "), Jugador en (" << posicionJugador.x << ", " << posicionJugador.y
              << "), Distancia: " << distancia << ", deltaTime: " << deltaTime << std::endl;

    // Normaliza el vector de dirección (lo convierte en un vector unitario).
    if (distancia > 0) {
        direccion.x /= distancia;
        direccion.y /= distancia;
    } else {
        std::cout << "Distancia cero, no se mueve" << std::endl;
        return; // No hay movimiento si la distancia es cero.
    }

    float movimiento = 0.0f;
    // Determina si la hormiga debe moverse o detenerse.
    if (distancia > distanciaObjetivo + 10.0f) { // Si la hormiga está lejos de la distancia objetivo.
        movimiento = velocidad * deltaTime; // Calcula la cantidad de movimiento para este frame.
        enPosicionDisparo = false; // La hormiga no está en posición de disparo si se está moviendo.
        enMovimiento = true; // Activa la bandera de movimiento para la animación.
        std::cout << "Moviendo hacia jugador, movimiento: " << movimiento << std::endl;
    } else { // Si la hormiga está cerca de la distancia objetivo.
        enPosicionDisparo = true; // La hormiga está en posición para disparar o atacar cuerpo a cuerpo.
        enMovimiento = false; // Desactiva el movimiento.
        std::cout << "En posición de disparo, no se mueve" << std::endl;
        return;
    }

    // Calcula el desplazamiento real en X e Y.
    float dx = direccion.x * movimiento;
    float dy = direccion.y * movimiento;

    // Verifica si el movimiento propuesto es válido, utilizando la función 'puedeMoverse' proporcionada.
    float newX = posicionActual.x + dx;
    float newY = posicionActual.y + dy;
    bool movimientoValido = puedeMoverse(newX, newY); // Esto permite verificar colisiones con el laberinto.
    std::cout << "Intentando mover a (" << newX << ", " << newY << "), PuedeMoverse: " << (movimientoValido ? "true" : "false") << std::endl;

    if (movimientoValido) { // Si el movimiento no resulta en una colisión.
        sprite.move(dx, dy); // Aplica el desplazamiento al sprite.
        posicion_x = newX; // Actualiza las variables de posición de la hormiga.
        posicion_y = newY;
        std::cout << "Hormiga movida a (" << newX << ", " << newY << ")" << std::endl;
    } else {
        std::cout << "Movimiento bloqueado por colisión" << std::endl; // Mensaje si el movimiento es bloqueado.
    }
    // Actualiza el temporizador de los frames de animación.
    tiempoFrame += deltaTime;
    // Si la hormiga se está moviendo y es hora de cambiar de frame.
    if (enMovimiento && tiempoFrame >= duracionFrame) {
        tiempoFrame = 0.0f; // Reinicia el temporizador del frame.
        frameActual = (frameActual + 1) % 2; // Alterna entre el frame 0 (pie derecho) y el 1 (pie izquierdo).
        sprite.setTexture(texturasAnimacion[frameActual]); // Cambia la textura del sprite para animar.
        std::cout << "Cambiando a frame " << frameActual << " ("
                  << (frameActual == 0 ? "pie derecho" : "pie izquierdo")
                  << ")" << std::endl;
    }
}

// Realiza una acción de ataque contra un objetivo.
// Solo las hormigas infectadas pueden atacar, causando daño y activando una animación de ataque.
void Hormigas::atacar(Personaje* objetivo) {
    if (infectadas) { // El ataque solo está disponible para hormigas infectadas.
        objetivo->recibirdano(10); // El objetivo recibe 10 puntos de daño.
        estaAtacando = true; // Activa el estado de ataque.
        tiempoAtaqueRestante = duracionAtaque; // Establece la duración de la animación de ataque.
        sprite.setTexture(texturaAtaque); // Cambia la textura de la hormiga a la de ataque.
    }
}

// Actualiza el estado de la hormiga, principalmente para gestionar el fin de la animación de ataque.
void Hormigas::actualizar(float deltaTime) {
    if (estaAtacando) { // Si la hormiga está actualmente en un estado de ataque.
        tiempoAtaqueRestante -= deltaTime; // Decrementa el tiempo restante de la animación de ataque.
        if (tiempoAtaqueRestante <= 0.0f) { // Si el tiempo de ataque ha expirado.
            estaAtacando = false; // Desactiva el estado de ataque.
            if (enMovimiento) {
                sprite.setTexture(texturasAnimacion[frameActual]); // Si la hormiga se estaba moviendo, vuelve a su animación de movimiento.
            } else {
                sprite.setTexture(texturasAnimacion[0]); // Si no se movía, vuelve al frame inicial (pie derecho).
            }
            std::cout << "Hormiga terminó animación de ataque, volviendo a frame " << frameActual << std::endl;
        }
    }
}

// Retorna el estado de infección de la hormiga (true si está infectada, false si no).
bool Hormigas::getInfectadas(){
    return infectadas;
}

// Establece la posición del sprite de la hormiga.
// También reinicia el frame de animación al primero si la hormiga está infectada.
void Hormigas::setPosition(float x, float y) {
    sprite.setPosition(x,y); // Aplica la nueva posición al sprite.
    frameActual = 0; // Reinicia el frame de animación al inicio.
    if (infectadas) { // Si la hormiga está infectada, asegura que la textura sea la del primer frame de movimiento.
        sprite.setTexture(texturasAnimacion[0]);
    }
    std::cout << "Hormiga posicionada en (" << x << ", " << y << ")" << std::endl;
}

// Establece la escala del sprite de la hormiga.
void Hormigas::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY); // Aplica la nueva escala al sprite.
    std::cout << "Escala de hormiga ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

// Obtiene los límites de colisión ajustados de la hormiga para una detección más precisa.
sf::FloatRect Hormigas::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds(); // Obtiene los límites globales (rectángulo delimitador) del sprite.
    float scaleReduction = 0.6f; // Factor para reducir el tamaño de los límites de colisión, haciéndolos más ajustados.
    float newWidth = bounds.width * scaleReduction; // Nuevo ancho de los límites.
    float newHeight = bounds.height * scaleReduction; // Nueva altura de los límites.
    float offsetX = (bounds.width - newWidth) / 2.0f; // Desplazamiento para centrar los nuevos límites horizontalmente.
    float offsetY = (bounds.height - newHeight) / 2.0f; // Desplazamiento para centrar los nuevos límites verticalmente.
    // Crea y retorna un nuevo sf::FloatRect con los límites ajustados.
    sf::FloatRect adjustedBounds(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
    std::cout << "Hormiga bounds: [" << adjustedBounds.left << ", " << adjustedBounds.top << ", "
              << adjustedBounds.width << ", " << adjustedBounds.height << "]" << std::endl;
    return adjustedBounds;
}

// Dispara una espora desde la hormiga hacia la posición del jugador.
// Esta función tiene un cooldown de 3 segundos entre disparos.
void Hormigas::dispararEspora(std::vector<sf::Sprite>& esporas, std::vector<sf::Vector2f>& direccionesEsporas, std::vector<float>& rotacionesEsporas, Personaje* jugador,  sf::Texture& esporaTexture) {
    if (relojDisparo.getElapsedTime().asSeconds() < 3.0f) return; // Si no han pasado 3 segundos desde el último disparo, no hace nada.

    sf::Sprite espora; // Crea un nuevo sprite para la espora.
    if (esporaTexture.getSize().x == 0) { // Verifica si la textura de la espora se cargó correctamente.
        cout << "no hay textura" <<endl;
    } else {
        espora.setTexture(esporaTexture); // Asigna la textura a la espora.
        espora.setOrigin(esporaTexture.getSize().x / 2.0f, esporaTexture.getSize().y / 2.0f); // Centra el origen.

        // Calcula la posición inicial de la espora para que aparezca al lado de la hormiga.
        sf::Vector2f posicionHormiga = sprite.getPosition();
        sf::Vector2f escalaHormiga = sprite.getScale();
        float anchoHormiga = sprite.getLocalBounds().width;

        float offsetX = 0.0f; // Ajuste adicional en X.
        float offsetY = 0.0f; // Ajuste adicional en Y.

        float desplazamientoIzquierdo = -(anchoHormiga / 2.0f) * escalaHormiga.x + offsetX;
        float desplazamientoVertical = offsetY;

        sf::Vector2f posicionEspora(posicionHormiga.x + desplazamientoIzquierdo, posicionHormiga.y + desplazamientoVertical);
        espora.setPosition(posicionEspora); // Establece la posición inicial de la espora.
        espora.setScale(0.5f, 0.5f); // Ajusta el tamaño de la espora.
    }

    // Obtiene el centro del sprite del jugador para apuntar la espora.
    sf::FloatRect jugadorBounds = jugador->getBounds();
    sf::Vector2f centroJugador(jugadorBounds.left + jugadorBounds.width / 2.0f,
                               jugadorBounds.top + jugadorBounds.height / 2.0f);

    // Calcula el vector de dirección desde la hormiga hacia el centro del jugador.
    sf::Vector2f direccion(centroJugador.x - sprite.getPosition().x, centroJugador.y - sprite.getPosition().y);
    float magnitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    if (magnitud > 0) {
        direccion.x /= magnitud; // Normaliza la dirección.
        direccion.y /= magnitud;
    }

    esporas.push_back(espora); // Añade la espora recién creada a la lista de esporas activas.
    direccionesEsporas.push_back(direccion); // Añade la dirección calculada para la espora.
    rotacionesEsporas.push_back(0.0f); // Inicializa la rotación de la espora a 0 grados.
    estaAtacando = true; // Activa el estado de ataque de la hormiga.
    tiempoAtaqueRestante = duracionAtaque; // Reinicia el temporizador de la animación de ataque.
    sprite.setTexture(texturaAtaque); // Cambia la textura de la hormiga a su pose de ataque.
    relojDisparo.restart(); // Reinicia el temporizador del cooldown de disparo.
    std::cout << "Espora disparada desde (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ") hacia (" << jugador->getPosition().x << ", " << jugador->getPosition().y << ")" << std::endl;
}

// Dibuja el sprite de la hormiga en la ventana del juego.
void Hormigas::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite); // Dibuja el sprite de la hormiga en la ventana.
    std::cout << "Dibujando hormiga en (" << sprite.getPosition().x << ", " << sprite.getPosition().y
              << ") con estado " << (estaAtacando ? "atacando" : "normal") << std::endl;
}