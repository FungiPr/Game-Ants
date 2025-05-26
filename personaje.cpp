#include "personaje.h" // Incluye la definición de la clase Personaje.

// Inicialización de la variable estática 'texturasCompartidas'.
// Este mapa almacena las texturas para evitar cargarlas repetidamente, optimizando la memoria.
map<string, sf::Texture> Personaje::texturasCompartidas;

// Constructor de la clase Personaje.
// Inicializa la vida máxima y actual, la posición inicial y el estado de vida.
Personaje::Personaje(const string& archivo_textura, int vidaMax)
: vidamaxima(vidaMax), vida_actual(vidaMax), posicion_x(0), posicion_y(0), vivo(true)
{
    // Verifica si la textura ya ha sido cargada y está en el mapa de texturas compartidas.
    if (texturasCompartidas.find(archivo_textura) == texturasCompartidas.end()) {
        sf::Texture texturaTemp; // Crea una textura temporal.
        // Intenta cargar la textura desde el archivo especificado.
        if (!texturaTemp.loadFromFile(archivo_textura)) {
            cout << "Error: no se pudo cargar la textura " << archivo_textura << endl; // Muestra un error si falla la carga.
        } else {
            // Si la carga es exitosa, guarda la textura en el mapa compartido.
            texturasCompartidas[archivo_textura] = texturaTemp;
        }
    }
    // Asigna la textura del sprite del personaje desde el mapa compartido, ya sea recién cargada o ya existente.
    sprite.setTexture(texturasCompartidas[archivo_textura]);
    sprite.setPosition(posicion_x, posicion_y); // Establece la posición inicial del sprite.
}

// Dibuja el sprite del personaje en la ventana de renderizado.
// El personaje solo se dibuja si está vivo.
void Personaje::dibujar(sf::RenderWindow& ventana) {
    if (vivo) { // Comprueba si el personaje está vivo.
        ventana.draw(sprite); // Dibuja el sprite del personaje.
    }
}

// Aplica una cantidad de daño al personaje, reduciendo su vida actual.
void Personaje::recibirdano(int cantidad) {
    vida_actual -= cantidad; // Reduce la vida actual por la cantidad de daño.
    if (vida_actual <= 0) { // Si la vida cae a 0 o menos...
        vida_actual = 0; // ...asegura que la vida no sea negativa.
        vivo = false; // ...el personaje se marca como muerto.
        cout << "El personaje ha muerto." << endl; // Muestra un mensaje de muerte.
    }
}

// Restablece la vida del personaje a su valor máximo.
void Personaje::restablecerVida() {
    vida_actual = vidamaxima; // Establece la vida actual a la vida máxima.
    cout << "El personaje tiene toda su vida." << endl; // Muestra un mensaje de recuperación.
}

// Obtiene la vida actual del personaje.
int Personaje::getVida() {
    return vida_actual; // Retorna el valor de la vida actual.
}

// Destructor de la clase Personaje.
// Se llama cuando un objeto Personaje es destruido, liberando recursos.
Personaje::~Personaje() {
    cout << "Destructor de Personaje llamado." << endl; // Muestra un mensaje de depuración.
}