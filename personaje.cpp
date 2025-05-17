#include "personaje.h"

// inicialización de la variable estática
map<string, sf::Texture> Personaje::texturasCompartidas;

Personaje::Personaje(const string& archivo_textura, int vidaMax)
: vidamaxima(vidaMax), vida_actual(vidaMax), posicion_x(0), posicion_y(0), vivo(true)
{
    // Verificar si la textura ya está cargada
    if (texturasCompartidas.find(archivo_textura) == texturasCompartidas.end()) {
        sf::Texture texturaTemp;
        if (!texturaTemp.loadFromFile(archivo_textura)) {
            cout << "Error: no se pudo cargar la textura " << archivo_textura << endl;
        } else {
            texturasCompartidas[archivo_textura] = texturaTemp;
        }
    }
    // Asignar la textura desde el mapa compartido
    sprite.setTexture(texturasCompartidas[archivo_textura]);
    sprite.setPosition(posicion_x, posicion_y);
}

// Método para dibujar
void Personaje::dibujar(sf::RenderWindow& ventana) {
    if (vivo) {
        ventana.draw(sprite);
    }
}

void Personaje::mover(float dx, float dy) {
    posicion_x += dx;
    posicion_y += dy;
    sprite.move(dx, dy);
}

void Personaje::recibirdano(int cantidad) {
    vida_actual -= cantidad;
    if (vida_actual <= 0) {
        vida_actual = 0;
        vivo = false;
        cout << "El personaje ha muerto." << endl;
    }
}


int Personaje::getVida() {
    return vida_actual;
}

Personaje::~Personaje() {
    cout << "Destructor de Personaje llamado." << endl;
}
