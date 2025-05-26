#include "ray.h"
#include <iostream>

Ray::Ray()
    : Personaje("Rayvistasuperiors.png", 100),
      bastonEnergia(MAX_ENERGIA),
      numeroVidas(3),
      SemillasRecolectadas(0),
      hongosbiolumicentesrecolectados(0),
      frameActual(0),
      tiempoFrame(0.0f),
    enMovimiento(false),
    estaSaltando(false),
    alturaSalto(100.0f),
    tiempoEsperaAtaque(2.0f) // Tiempo de espera de 2 segundos entre ataques
{
    // Cargar las texturas de animación
    texturasAnimacion.resize(3); // Base, pie derecho, pie izquierdo
    if (!texturasAnimacion[0].loadFromFile("Rayvistasuperiors.png")) {
        std::cerr << "Error: No se pudo cargar la textura Rayvistasuperiors.png" << std::endl;
        throw std::runtime_error("Fallo al cargar Rayvistasuperiors.png");
    }
    if (!texturasAnimacion[1].loadFromFile("Raycaminandoderechas.png")) {
        std::cerr << "Error: No se pudo cargar la textura Raycaminandoderechas.png" << std::endl;
        throw std::runtime_error("Fallo al cargar Raycaminandoderechas.png");
    }
    if (!texturasAnimacion[2].loadFromFile("Raycaminandoizquierdos.png")) {
        std::cerr << "Error: No se pudo cargar la textura Raycaminandoizquierdos.png" << std::endl;
        throw std::runtime_error("Fallo al cargar Raycaminandoizquierdos.png");
    }
    if (!golpeNormalTexture.loadFromFile("raygolpeando.png")) {
        std::cout << "Error al cargar golpe_normal.png, usando rectángulo rojo" << std::endl;
    }

    if (!golpeLuzTexture.loadFromFile("raygolpedeluz.png")) {
        std::cout << "Error al cargar golpe_luz.png, usando rectángulo amarillo" << std::endl;
    }

    if (!superGolpeLuzTexture.loadFromFile("raysupergolpedeluz.png")) {
        std::cout << "Error al cargar super_golpe_luz.png, usando rectángulo blanco" << std::endl;
    }
    if (!projectileGolpeTexture.loadFromFile("bolagolpedeluz.png")) {
        std::cout << "Error al cargar raygolpedeluz.png" << std::endl;
    }
    if (!projectileSuperGolpeTexture.loadFromFile("bolasupergolpedeluz.png")) {
        std::cout << "Error al cargar bolasupergolpedeluz.png" << std::endl;
    }

    sprite.setTexture(texturasAnimacion[0]); // Iniciar con la textura base
    sprite.setPosition(100, 400);
    sprite.setScale(1.0f, 1.0f);

    std::cout << "Ray creado con vida inicial: " << vida_actual
              << ", posición: (100, 400), escala: (1.0, 1.0)" << std::endl;
}

void Ray::mover(float dx, float dy) {
    sprite.move(dx, dy);
    enMovimiento = true;
    std::cout << "Ray moviéndose, nueva posición: (" << sprite.getPosition().x << ", "
              << sprite.getPosition().y << ")" << std::endl;
    tiempoFrame += 1.0f / 60.0f; // Asumimos 60 FPS
    if (tiempoFrame >= duracionFrame) {
        tiempoFrame = 0.0f;
        frameActual = (frameActual + 1) % 3; // Ciclo: 0 -> 1 -> 2 -> 0
        sprite.setTexture(texturasAnimacion[frameActual]);
        std::cout << "Cambiando a frame " << frameActual << " ("
                  << (frameActual == 0 ? "base" : frameActual == 1 ? "pie derecho" : "pie izquierdo")
                  << ")" << std::endl;
    }
}

bool Ray::isSaltando() {
    return estaSaltando;
}

void Ray::actualizarSalto(float deltaTime) {
    if (!estaSaltando) return;

    float tiempoSalto = relojSalto.getElapsedTime().asSeconds();
    float duracionSalto = 1.0f; // Duración total del salto (segundos)

    if (tiempoSalto >= duracionSalto) {
        // Terminar salto
        estaSaltando = false;
        sprite.setPosition(posicionOriginal); // Restaurar posición original
        std::cout << "Ray terminó salto, posición restaurada: (" << posicionOriginal.x << ", " << posicionOriginal.y << ")" << std::endl;
        return;
    }

    // Animación: desplazamiento vertical (parábola suave)
    float t = tiempoSalto / duracionSalto; // Progreso normalizado (0 a 1)
    float altura = -4 * alturaSalto * t * (t - 1); // Fórmula parabólica: -4h*t*(t-1)
    sprite.setPosition(posicionOriginal.x, posicionOriginal.y - altura);
}

void Ray::saltar() {
        if (!estaSaltando) {
            estaSaltando = true;
            relojSalto.restart();
            posicionOriginal = sprite.getPosition();
            std::cout << "Ray inició salto en (" << posicionOriginal.x << ", " << posicionOriginal.y << ")" << std::endl;
        }
    }

void Ray::setAtaqueSprite(sf::Texture& texture, float duracion) {
    spriteAtaque.setTexture(texture);
    // Usar la misma escala que sprite
    sf::Vector2f spriteScale = sprite.getScale();
    spriteAtaque.setScale(spriteScale.x, spriteScale.y);
    tiempoAtaqueRestante = duracion;

    // Configurar el origen dependiendo de la textura
    if (&texture == &golpeNormalTexture) {
        float offsetX = 130.0f; // Valor ajustado
        float offsetY = 150.0f; // Valor ajustado
        spriteAtaque.setOrigin(texture.getSize().x / 2.0f - offsetX, texture.getSize().y / 2.0f - offsetY);
        std::cout << "Origen ajustado para raygolpeando.png: ("
                  << (texture.getSize().x / 2.0f - offsetX) << ", "
                  << (texture.getSize().y / 2.0f - offsetY) << "), Tamaño: ("
                  << texture.getSize().x << ", " << texture.getSize().y << "), Escala: ("
                  << spriteScale.x << ", " << spriteScale.y << ")" << std::endl;
    } else {
        spriteAtaque.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        std::cout << "Origen centrado para textura de ataque: ("
                  << (texture.getSize().x / 2.0f) << ", "
                  << (texture.getSize().y / 2.0f) << "), Tamaño: ("
                  << texture.getSize().x << ", " << texture.getSize().y << "), Escala: ("
                  << spriteScale.x << ", " << spriteScale.y << ")" << std::endl;
    }
}

sf::Sprite& Ray::getSpriteAtaque() {
    return spriteAtaque;
}

sf::Texture& Ray::getTextureGolpe() {
    return textureGolpe;
}

sf::Texture& Ray::getTextureSuperGolpe() {
    return textureSuperGolpe;
}

void Ray::setEstaAtacando(bool atacando) {
    estaAtacando = atacando;
}

void Ray::setTiempoAtaqueRestante(float tiempo) {
    tiempoAtaqueRestante = tiempo;
}

void Ray::atacar(Personaje* enemigo) {
    enemigo->recibirdano(15);
    std::cout << "Ray ataca, causando 15 de daño" << std::endl;
    estaAtacando = true;
    setAtaqueSprite(golpeNormalTexture, 0.15f); // Golpe normal por 0.5 segundos
    relojAtaque.restart();
}

void Ray::golpedeLuz(Personaje* enemigo) {
    if (enemigo && bastonEnergia >= 50) {
        enemigo->recibirdano(25);
        consumirEnergia(50); // Usar el método consumirEnergia
        std::cout << "Ray usa golpedeLuz, causando 25 de daño, energía restante: "
                  << bastonEnergia << std::endl;
        setAtaqueSprite(golpeLuzTexture, 0.3f);
        relojAtaque.restart();
    } else {
        std::cout << "Ray no tiene suficiente energía para golpedeLuz (necesita 50, tiene "
                  << bastonEnergia << ")" << std::endl;
    }
}

void Ray::supergolpedeLuz(Personaje* enemigo) {
    if (enemigo && bastonEnergia == 100) {
        enemigo->recibirdano(50);
        consumirEnergia(100); // Usar el método consumirEnergia
        std::cout << "Ray usa supergolpedeLuz, causando 50 de daño, energía restante: "
                  << bastonEnergia << std::endl;
        setAtaqueSprite(superGolpeLuzTexture, 0.3f);
        relojAtaque.restart();
    } else {
        std::cout << "Ray no tiene suficiente energía para supergolpedeLuz (necesita 100, tiene "
                  << bastonEnergia << ")" << std::endl;
    }
}

sf::Texture& Ray::getProjectileGolpeTexture() {
    return projectileGolpeTexture;
}

sf::Texture& Ray::getProjectileSuperGolpeTexture() {
    return projectileSuperGolpeTexture;
}

void Ray::procesarAtaque(sf::Keyboard::Key key, Personaje* enemigo) {
    bool fIsPressed = (key == sf::Keyboard::F) && sf::Keyboard::isKeyPressed(sf::Keyboard::F);
    bool qIsPressed = (key == sf::Keyboard::Q) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q);

    // Solo procesar si no está atacando actualmente
    if (!estaAtacando) {
        // Ataque básico con F
        if (fIsPressed && !fWasPressed) {
            atacar(enemigo);
            std::cout << "Tecla F presionada (nueva pulsación)" << std::endl;
        }

        // Ataque especial con Q
        if (qIsPressed && !qWasPressed) {
            if (bastonEnergia == 100) {
                supergolpedeLuz(enemigo);
                std::cout << "Tecla Q presionada (nueva pulsación), súper golpe de luz" << std::endl;
            } else if (bastonEnergia >= 50) {
                golpedeLuz(enemigo);
                std::cout << "Tecla Q presionada (nueva pulsación), golpe de luz" << std::endl;
            }
        }
    }

    // Actualizar el estado anterior de las teclas
    fWasPressed = fIsPressed;
    qWasPressed = qIsPressed;
}


void Ray::actualizar(float deltaTime) {
    actualizarSalto(deltaTime);
    if (estaAtacando) {
        tiempoAtaqueRestante -= deltaTime;
        if (tiempoAtaqueRestante <= 0.0f) {
            estaAtacando = false;
            tiempoAtaqueRestante = 0.0f;
            sprite.setTexture(texturasAnimacion[frameActual]); // Restaurar la textura de caminado
            std::cout << "Ray terminó animación de ataque, restaurando frame " << frameActual << std::endl;
        }
    }
}

void Ray::consumirEnergia(int cantidad) {
    if (bastonEnergia >= cantidad) {
        bastonEnergia -= cantidad;
    } else {
        bastonEnergia = 0; // Asegurarse de no ir por debajo de 0
    }
    cout << "Energía consumida: " << cantidad << ", Energía restante: " << bastonEnergia << endl;
}

void Ray::aumentarVida(int vidaRecuperada) {
    vida_actual += vidaRecuperada;
    if (vida_actual > vidamaxima) vida_actual = vidamaxima;
    if (vida_actual < 0) vida_actual = 0;
}

void Ray::setBastonEnergia(int energia) {
    bastonEnergia = std::max(0, std::min(energia, MAX_ENERGIA)); // Asegurar que esté entre 0 y MAX_ENERGIA
}


void Ray::recolectarsemilla() {
    SemillasRecolectadas++;
    std::cout << "Ray recolecta una semilla, total: " << SemillasRecolectadas << std::endl;
}

void Ray::recolectarHongo() {
    hongosbiolumicentesrecolectados++;
    std::cout << "Ray recolecta un hongo bioluminiscente, total: "
              << hongosbiolumicentesrecolectados << std::endl;
}

void Ray::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    frameActual = 0; // Volver a la imagen base cuando se reposiciona
    sprite.setTexture(texturasAnimacion[0]);
    std::cout << "Ray posicionado en (" << x << ", " << y << "), frame reiniciado a base" << std::endl;
}

void Ray::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
    std::cout << "Escala de Ray ajustada a (" << scaleX << ", " << scaleY << ")" << std::endl;
}

sf::FloatRect Ray::getBounds() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float scaleReduction = 0.8f;
    float newWidth = bounds.width * scaleReduction;
    float newHeight = bounds.height * scaleReduction;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;
    return sf::FloatRect(bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

void Ray::dibujar(sf::RenderWindow& ventana) {
    if (estaAtacando && tiempoAtaqueRestante > 0.0f) {
        // Sincronizar la posición y escala de spriteAtaque con sprite
        spriteAtaque.setPosition(sprite.getPosition());
        sf::Vector2f spriteScale = sprite.getScale();
        spriteAtaque.setScale(spriteScale.x, spriteScale.y);
        ventana.draw(spriteAtaque);
        std::cout << "Dibujando Ray en modo ataque en (" << spriteAtaque.getPosition().x << ", "
                  << spriteAtaque.getPosition().y << "), Escala: ("
                  << spriteScale.x << ", " << spriteScale.y << ")" << std::endl;
    } else {
        sf::Vector2f spriteScale = sprite.getScale();
        ventana.draw(sprite);
        std::cout << "Dibujando Ray en (" << sprite.getPosition().x << ", " << sprite.getPosition().y
                  << ") con frame " << frameActual << " ("
                  << (frameActual == 0 ? "base" : frameActual == 1 ? "pie derecho" : "pie izquierdo")
                  << "), Escala: (" << spriteScale.x << ", " << spriteScale.y << ")" << std::endl;
    }
}