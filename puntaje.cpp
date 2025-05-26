#include "Puntaje.h" // Incluye la definición de la clase Puntaje.

// Constructor de la clase Puntaje.
// Inicializa el puntaje a 0 y configura el aspecto visual del marcador.
Puntaje::Puntaje(sf::Font& fuente) : puntaje(0) {
    // Configura el rectángulo que sirve de fondo para el puntaje.
    fondo.setSize(sf::Vector2f(140, 60));
    fondo.setFillColor(sf::Color(21, 67, 96)); // Establece un color azul oscuro.
    fondo.setPosition(1750, 20); // Posiciona el fondo en la esquina superior derecha de la pantalla.

    // Configura el texto que mostrará el valor del puntaje.
    textoPuntaje.setFont(fuente); // Asigna la fuente proporcionada.
    textoPuntaje.setCharacterSize(30); // Define el tamaño del texto.
    textoPuntaje.setFillColor(sf::Color::White); // Establece el color del texto a blanco.
    textoPuntaje.setPosition(1769, 32); // Posiciona el texto centrado dentro del rectángulo.
    textoPuntaje.setString("Puntaje: 0"); // Establece el texto inicial.
}

// Actualiza el puntaje a un nuevo valor específico.
void Puntaje::setPuntaje(int nuevoPuntaje) {
    puntaje = nuevoPuntaje; // Asigna el nuevo valor al puntaje interno.
    textoPuntaje.setString("Puntaje: " + std::to_string(puntaje)); // Refresca el texto en pantalla.
}

// Aumenta el puntaje actual en una cantidad dada.
void Puntaje::incrementarPuntaje(int incremento) {
    puntaje += incremento; // Suma el incremento al puntaje actual.
    textoPuntaje.setString("Puntaje: " + std::to_string(puntaje)); // Actualiza el texto mostrado.
}

// Dibuja el marcador de puntaje en la ventana del juego.
// El puntaje solo se dibuja si el nivel actual es 1 o superior.
void Puntaje::dibujar(sf::RenderWindow& ventana, int nivel, sf::RenderStates estados) {
    // Solo dibuja el puntaje si el juego está en el nivel 1 o superior.
    if (nivel >= 1) {
        ventana.draw(fondo, estados); // Dibuja el fondo del puntaje.
        ventana.draw(textoPuntaje, estados); // Dibuja el texto del puntaje.
    }
}