#include "juego.h"
#include <iostream>
#include <memory>


// Constructor: inicializa ventana, estado y carga nivel inicial
Juego::Juego()
    : ventana(sf::VideoMode(1920, 1080), "The Last Of Ants"),   // Crear ventana de 1920x1080 con título
      jugador(),                                               // Inicializa objeto jugador con constructor por defecto
      jefefinal(),                                             // Inicializa objeto jefe final por defecto
      nivel(1),                                                // Inicializa nivel en 1
      estado(BIENVENIDA),                                      // Estado inicial en pantalla de bienvenida
      marcadorPuntaje(fuente),                                 // Inicializa marcador de puntaje con la fuente
      oleadasJefeFinal(0),                                     // Contador de oleadas del jefe final en 0
      posicionInicialRayNivel1(5, 35),                         // Posición inicial para rayos en nivel 1
      laberintoCompletado(false),                              // Indica que laberinto no está completado aún
      finalLaberinto(1180, 400, 200, 200),                     // Rectángulo que define zona final del laberinto (200x200)
      laberintoCargado(false),                                 // Indica si laberinto ya fue cargado
      hongosGeneradosDespuesSegundaOleada(false)              // Flag para hongos generados tras segunda oleada
{
    ventana.setFramerateLimit(60);                             // Limita los FPS de la ventana a 60

    // Carga fuente una sola vez
    if (!fuente.loadFromFile("pressgothic.ttf")) {            // Intenta cargar la fuente desde archivo
        cout << "Error: no se pudo cargar pressgothic.ttf" << endl;  // Mensaje si falla la carga
    }

    // Carga fondo de bienvenida
    if (!fondoBienvenida.loadFromFile("FondoInicios.png")) {  // Intenta cargar imagen de fondo para bienvenida
        cout << "Error cargando fondo de bienvenida" << endl;  // Mensaje si falla la carga
    }
    spriteFondoBienvenida.setTexture(fondoBienvenida);        // Asigna textura cargada al sprite de fondo bienvenida

    if (!fondoGameover.loadFromFile("PantallaGAmeover.png")) { // Intenta cargar imagen de fondo para pantalla de game over
        cout << "Error cargando fondo de bienvenida" << endl;  // Mensaje si falla la carga (mensaje repetido)
    }
    spriteFondoGameover.setTexture(fondoGameover);            // Asigna textura cargada al sprite de game over

    if (!fondoWinner.loadFromFile("fondoWinner.png")) {        // Intenta cargar imagen de fondo para pantalla de ganador
        cout << "Error cargando fondo de bienvenida" << endl;  // Mensaje si falla la carga (mensaje repetido)
    }
    spriteFondoWinner.setTexture(fondoWinner);                 // Asigna textura cargada al sprite de ganador

    // Configura texto de bienvenida
    textoBienvenida.setFont(fuente);                           // Define la fuente del texto de bienvenida
    textoBienvenida.setString("The \nLast \nOf \nAnts");       // Texto a mostrar con saltos de línea
    textoBienvenida.setCharacterSize(210);                      // Tamaño de letra grande
    textoBienvenida.setFillColor(sf::Color::White);             // Color blanco para el texto
    textoBienvenida.setPosition(100, 40);                        // Posición en pantalla (x=100, y=40)
    textoBienvenida.setLineSpacing(0.8f);                       // Espaciado entre líneas ligeramente reducido

    	// Configura botón de inicio
	botonIniciar.setSize(sf::Vector2f(400, 200));                // Define tamaño del botón iniciar (ancho 400, alto 200)
	botonIniciar.setFillColor(sf::Color(255, 255, 255, 80));     // Color blanco con transparencia alfa 80
	botonIniciar.setPosition(1450, 780);                         // Posición en pantalla (x=1450, y=780)
	textoBoton.setFont(fuente);                                  // Fuente usada para el texto del botón iniciar
	textoBoton.setString("INICIAR");                             // Texto que se muestra en el botón
	textoBoton.setCharacterSize(120);                            // Tamaño de la letra
	textoBoton.setFillColor(sf::Color::White);                   // Color blanco para el texto
	textoBoton.setPosition(1519, 810);                           // Posición del texto dentro del botón

	// Configura botón de reinicio
	botonReiniciar.setSize(sf::Vector2f(500, 200));              // Tamaño del botón reiniciar (ancho 500, alto 200)
	botonReiniciar.setFillColor(sf::Color(255, 255, 255, 80));   // Color blanco con transparencia alfa 80
	botonReiniciar.setPosition(760, 680);                        // Posición centrada debajo del texto "Game Over"
                                                              // (cálculo aproximado: 1920/2 - 400/2, 1080/2 + 150)
	textoReiniciar.setFont(fuente);                              // Fuente para el texto del botón reiniciar
	textoReiniciar.setString("REINICIAR");                       // Texto visible en el botón reiniciar
	textoReiniciar.setCharacterSize(120);                        // Tamaño del texto
	textoReiniciar.setFillColor(sf::Color::White);               // Color blanco para el texto
	textoReiniciar.setPosition(829, 710);                        // Posición ajustada para centrar texto en el botón

	// Configura botón de "Volver a Jugar"
	botonVolverJugar.setSize(sf::Vector2f(400, 200));            // Tamaño del botón volver a jugar
	botonVolverJugar.setFillColor(sf::Color(255, 255, 255, 80)); // Color blanco con transparencia alfa 80
	botonVolverJugar.setPosition(510, 680);                      // Posición a la izquierda del centro (960 - 450)
	textoVolverJugar.setFont(fuente);                            // Fuente para el texto del botón volver a jugar
	textoVolverJugar.setString("VOLVER A JUGAR");                // Texto visible en el botón
	textoVolverJugar.setCharacterSize(80);                       // Tamaño del texto (más pequeño que otros botones)
	textoVolverJugar.setFillColor(sf::Color::White);             // Color blanco para el texto
	textoVolverJugar.setPosition(532, 735);                      // Posición ajustada para centrar texto en el botón

	// Configura botón de "Salir"
	botonSalir.setSize(sf::Vector2f(400, 200));                  // Tamaño del botón salir
	botonSalir.setFillColor(sf::Color(255, 255, 255, 80));       // Color blanco con transparencia alfa 80
	botonSalir.setPosition(1010, 680);                           // Posición a la derecha del centro (960 + 50)
	textoSalir.setFont(fuente);                                  // Fuente para texto del botón salir
	textoSalir.setString("SALIR");                               // Texto visible en el botón salir
	textoSalir.setCharacterSize(120);                            // Tamaño del texto
	textoSalir.setFillColor(sf::Color::White);                   // Color blanco para el texto
	textoSalir.setPosition(1100, 715);                           // Posición ajustada para centrar texto en el botón

     if (!cursorNormal.loadFromSystem(sf::Cursor::Arrow) ||        // Intenta cargar cursor normal (flecha)
    	!cursorMano.loadFromSystem(sf::Cursor::Hand)) {           // Intenta cargar cursor mano (para interacción)
    	}                                                          // No se especifica acción si falla, bloque vacío

	// Carga fondo del tutorial
	if (!fondoTutorial.loadFromFile("fondotutorialoficial.png")) { // Intenta cargar imagen de fondo tutorial
    	cout << "Error cargando fondo del tutorial" << endl;       // Mensaje en consola si falla la carga
	}
	spriteFondoTutorial.setTexture(fondoTutorial);                 // Asigna textura cargada al sprite del tutorial

	// Cargar nivel 1 (puede usarse luego)
	cargarnivel(nivel);                                             // Función para cargar el nivel actual (nivel 1 inicialmente)

	// Cargar mapa de colisión del laberinto
	if (!laberintoColision.loadFromFile("laberintocompletos.png")) {  // Intenta cargar mapa de colisión laberinto
    	cout << "Error cargando imagen de colisión del laberinto" << endl; // Mensaje si falla la carga
	} else {
    	laberintoCargado = true;                                     // Marca que el laberinto fue cargado correctamente
    	cout << "Mapa de colisión del laberinto cargado exitosamente" << endl; // Mensaje éxito
	}

	if (!fondoLaberinto.loadFromFile("laberintocompletos.png")) {    // Intenta cargar imagen de fondo laberinto
    	cout << "Error cargando fondo de laberinto" << endl;          // Mensaje si falla la carga
	}
	spriteLaberinto.setTexture(fondoLaberinto);                      // Asigna textura cargada al sprite laberinto

	if (!semillaTexture.loadFromFile("semillas.png")) {              // Intenta cargar textura para semillas
    	std::cout << "Error al cargar la textura de la semilla" << std::endl; // Mensaje si falla
	}

	if (!hongoTexture.loadFromFile("hongosbiolumicentes.png")) {     // Intenta cargar textura para hongos bioluminiscentes
    	std::cout << "No se pudo cargar hongo.png, usando círculo rojo" << std::endl; // Mensaje si falla
	}

	if (!esporaHormigaTexture.loadFromFile("Esporasnpc.png")) {      // Intenta cargar textura para espora hormiga
    	std::cout << "Error al cargar espora_hormiga.png, usando círculo rojo" << std::endl; // Mensaje si falla
	}

	if (!esporaReyHongoTexture.loadFromFile("Esporasreyhongo.png")) {// Intenta cargar textura para espora rey hongo
    	std::cout << "Error al cargar espora_reyhongo.png, usando círculo rojo" << std::endl; // Mensaje si falla
	}

	// Posiciones iniciales de hormigas nivel 1
	posicionesHormigasNivel1 = {
    	{1600, 100},  // Hormiga 1
    	{1600, 300},  // Hormiga 2
    	{1600, 600},  // Hormiga 3
    	{1600, 800},  // Hormiga 4
	};

	// Posiciones iniciales de hormigas jefe final
	posicionesHormigasJefeFinal = {
    	{1000, 100},  // Hormiga 1
    	{1000, 400},  // Hormiga 2
    	{1000, 700},  // Hormiga 3
	};

	// Posiciones hormigas no infectadas
	posicionesHormigasNoInfectadas = {
    	{440, 250},   // Hormiga 1
    	{250, 950},	  //Hormiga 2
	};

	// Cargar último nivel (jefe final)
	if (!fondoJefefinal.loadFromFile("fondoJefefinalmorado.jpg")) {  // Intenta cargar fondo para jefe final
   		cout << "Error cargando fondo de laberinto" << endl;          // Mensaje si falla la carga
	}
	spritefondojefefinal.setTexture(fondoJefefinal);                 // Asigna textura al sprite del jefe final
	inicializarMurosLaberinto();                                      // Llama función para inicializar muros del laberinto
}


void Juego::inicializarMurosLaberinto() {
    // Limpiar los muros anteriores
    laberintoMuros.clear();                                     // Vacía el vector que contiene los muros previos

    // Definir los muros ajustados con más espacio para los caminos

    laberintoMuros.push_back(sf::FloatRect(0, 0, 1240, 20));          // Barra superior horizontal grande (arriba)
    laberintoMuros.push_back(sf::FloatRect(0, 920, 1240, 200));       // Barra inferior horizontal grande (abajo)
    laberintoMuros.push_back(sf::FloatRect(800, 800, 280, 10));       // Rectángulo horizontal cerca abajo derecha
    laberintoMuros.push_back(sf::FloatRect(1049, 700, 40, 100));      // Rectángulo vertical pequeño, media derecha
    laberintoMuros.push_back(sf::FloatRect(980, 650, 50, 50));        // Cuadro cuadrado cerca derecha centro
    laberintoMuros.push_back(sf::FloatRect(1040, 200, 30, 330));      // Rectángulo vertical largo derecha central
    laberintoMuros.push_back(sf::FloatRect(10, 190, 500, 430));       // Gran rectángulo vertical izquierda central
    laberintoMuros.push_back(sf::FloatRect(600, 190, 70, 250));       // Rectángulo vertical pequeño derecha del anterior
    laberintoMuros.push_back(sf::FloatRect(480, 550, 100, 50));       // Rectángulo horizontal pequeño centro
    laberintoMuros.push_back(sf::FloatRect(720, 500, 20, 10));        // Rectángulo horizontal pequeño centro
    laberintoMuros.push_back(sf::FloatRect(690, 500, 20, 5));         // Rectángulo horizontal muy pequeño centro
    laberintoMuros.push_back(sf::FloatRect(750, 300, 20, 5));         // Rectángulo horizontal pequeño arriba centro
    laberintoMuros.push_back(sf::FloatRect(750, 550, 70, 250));       // Rectángulo vertical derecha-centro
    laberintoMuros.push_back(sf::FloatRect(300, 150, 180, 350));      // Gran rectángulo vertical izquierda centro
    laberintoMuros.push_back(sf::FloatRect(700, 150, 130, 20));       // Rectángulo horizontal arriba derecha-centro
    laberintoMuros.push_back(sf::FloatRect(850, 200, 200, 20));       // Rectángulo horizontal centro derecha
    laberintoMuros.push_back(sf::FloatRect(950, 170, 100, 30));       // Rectángulo mediano horizontal arriba derecha
    laberintoMuros.push_back(sf::FloatRect(900, 360, 50, 280));       // Rectángulo vertical centro derecha
    laberintoMuros.push_back(sf::FloatRect(750, 350, 100, 40));       // Rectángulo horizontal centro derecha
    laberintoMuros.push_back(sf::FloatRect(1100, 530, 30, 25));       // Rectángulo pequeño vertical derecha abajo
    laberintoMuros.push_back(sf::FloatRect(0, 600, 640, 600));        // Gran rectángulo vertical izquierda abajo
    laberintoMuros.push_back(sf::FloatRect(1160, 0, 70, 400));        // Rectángulo vertical muy derecha arriba
    laberintoMuros.push_back(sf::FloatRect(1160, 550, 70, 600));      // Rectángulo vertical muy derecha abajo


    // Crear visualización de los muros
    cout << "Muros del laberinto inicializados: " << laberintoMuros.size() << " muros" << endl; // Muestra la cantidad de muros inicializados
    for ( auto& muro : laberintoMuros) {
        cout << "Muro: [" << muro.left << ", " << muro.top << ", " << muro.width << ", " << muro.height << "]" << endl; // Imprime las dimensiones y posición de cada muro
    }
}



// Método que inicia el flujo de bienvenida y luego bucle principal
void Juego::iniciar() {
    while (ventana.isOpen()) {                                   // Bucle principal mientras la ventana esté abierta
        float deltaTime = reloj.restart().asSeconds();           // Calcula tiempo entre frames para actualizaciones

        procesareventos();                                        // Procesa eventos de ventana y teclado
        actualizar(deltaTime);                                    // Actualiza estado del juego con deltaTime
        renderizar();                                             // Dibuja todo en la ventana

        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana); // Obtiene posición actual del mouse relativa a la ventana

        sf::FloatRect boundsIniciar = botonIniciar.getGlobalBounds();  // Obtiene límites globales del botón iniciar
        if (estado == BIENVENIDA && boundsIniciar.contains(mousePos.x, mousePos.y)) { // Si está en pantalla bienvenida y mouse está sobre botón iniciar
            botonIniciar.setFillColor(sf::Color(200, 200, 200, 150));  // Cambia color del botón para efecto hover
            ventana.setMouseCursor(cursorMano);                        // Cambia cursor a mano para indicar clickeable
        } else {
            botonIniciar.setFillColor(sf::Color(255, 255, 255, 80));   // Color normal del botón iniciar si no está hover
        }

        sf::FloatRect boundsReiniciar = botonReiniciar.getGlobalBounds();  // Límites del botón reiniciar
        if (estado == GAMEOVER && boundsReiniciar.contains(mousePos.x, mousePos.y)) { // Si está en game over y mouse sobre botón reiniciar
            botonReiniciar.setFillColor(sf::Color(200, 200, 200, 150));   // Cambio de color por hover
            ventana.setMouseCursor(cursorMano);                           // Cursor mano
        } else {
            botonReiniciar.setFillColor(sf::Color(255, 255, 255, 80));    // Color normal
        }

        // Manejo del cursor para los botones en WIN
        sf::FloatRect boundsVolverJugar = botonVolverJugar.getGlobalBounds();  // Límites botón volver a jugar
        sf::FloatRect boundsSalir = botonSalir.getGlobalBounds();              // Límites botón salir
        if (estado == WIN) {                                                    // Si está en pantalla de victoria
            if (boundsVolverJugar.contains(mousePos.x, mousePos.y)) {          // Mouse sobre botón volver a jugar
                botonVolverJugar.setFillColor(sf::Color(200, 200, 200, 150));  // Cambiar color hover
                ventana.setMouseCursor(cursorMano);                            // Cursor mano
            } else {
                botonVolverJugar.setFillColor(sf::Color(255, 255, 255, 80));   // Color normal
            }
            if (boundsSalir.contains(mousePos.x, mousePos.y)) {                // Mouse sobre botón salir
                botonSalir.setFillColor(sf::Color(200, 200, 200, 150));        // Color hover
                ventana.setMouseCursor(cursorMano);                            // Cursor mano
            } else {
                botonSalir.setFillColor(sf::Color(255, 255, 255, 80));         // Color normal
            }
            if (!boundsVolverJugar.contains(mousePos.x, mousePos.y) && !boundsSalir.contains(mousePos.x, mousePos.y)) { // Si mouse no está en ninguno
                ventana.setMouseCursor(cursorNormal);    // Cursor normal
            }
        } else if (estado != BIENVENIDA || !boundsIniciar.contains(mousePos.x, mousePos.y)) { // Si no está en bienvenida o mouse no está en botón iniciar
            if (estado != GAMEOVER || !boundsReiniciar.contains(mousePos.x, mousePos.y)) {   // Y no está en gameover o mouse no en botón reiniciar
                ventana.setMouseCursor(cursorNormal);            // Cursor normal
            }
        }
    }
}


void Juego::procesareventos() {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {                                    // Procesa todos los eventos pendientes
        if (evento.type == sf::Event::Closed) {                           // Si el evento es cerrar ventana
            ventana.close();                                               // Cierra la ventana
        }
        if (estado == BIENVENIDA &&
            evento.type == sf::Event::MouseButtonPressed &&                // Si estamos en pantalla bienvenida y se presiona botón del mouse
            evento.mouseButton.button == sf::Mouse::Left) {                // Y es botón izquierdo
            sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);       // Obtiene posición del mouse
            if (botonIniciar.getGlobalBounds().contains(mousePos.x, mousePos.y)) { // Si clic en botón iniciar
                estado = TUTORIAL;                                          // Cambia estado a tutorial
                limpiarEsporas();                                           // Limpia esporas existentes
                std::cout << "Estado cambiado a TUTORIAL" << std::endl;    // Mensaje en consola
                inicializarTutorial();                                     // Inicializa elementos del tutorial
            }
        }
        if (estado == TUTORIAL || estado == JUGANDO || estado == JEFEFINAL) {
            std::cout << "Estado actual: " << estado << ", NPCs en escena: " << npc.size() << std::endl; // Debug info

            if (evento.type == sf::Event::KeyPressed) {                    // Si se presiona una tecla
                if (evento.key.code == sf::Keyboard::F) {                  // Tecla F para atacar
                    for (auto it = npc.begin(); it != npc.end();) {        // Iterar sobre NPCs
                        Personaje* personaje = it->get();                   // Puntero al personaje actual
                        if (jugador.getBounds().intersects(personaje->getBounds())) { // Si jugador colisiona con personaje
                            std::cout << "Atacando personaje en (" << personaje->getBounds().left << ", "
                                      << personaje->getBounds().top << "), vida antes: " << personaje->getVida() << std::endl;
                            jugador.atacar(personaje);                        // Ataca personaje
                            std::cout << "Vida después: " << personaje->getVida() << std::endl;
                            if (personaje->getVida() <= 0) {                 // Si personaje murió
                                it = npc.erase(it);                           // Elimina personaje de la lista
                                marcadorPuntaje.incrementarPuntaje(10);      // Incrementa puntaje
                                std::cout << "Personaje eliminado, puntaje +10: "   << std::endl;
                            } else {
                                ++it;
                            }
                        } else {
                            ++it;
                        }
                    }
                    if (estado == JEFEFINAL && jugador.getBounds().intersects(jefefinal.getBounds())) { // Atacar jefe final
                        std::cout << "Atacando Rey Hongo en (" << jefefinal.getBounds().left << ", "
                                  << jefefinal.getBounds().top << "), vida antes: " << jefefinal.getVida() << std::endl;
                        jugador.atacar(&jefefinal);
                        std::cout << "Vida después: " << jefefinal.getVida() << std::endl;
                        if (jefefinal.getVida() <= 0) {                       // Si jefe muerto
                            estado = WIN;                                      // Cambia estado a victoria
                            textoNivel.setString("¡Juego Completado!");       // Mensaje de victoria
                            marcadorPuntaje.incrementarPuntaje(20);           // Incrementa puntaje por jefe
                            std::cout << "Rey Hongo eliminado, cambiando a GAMEOVER" << std::endl;
                        }
                    }
                }
                if (evento.key.code == sf::Keyboard::Q) {                   // Tecla Q para ataque especial (luz)
                    std::cout << "Tecla Q presionada, estado: " << estado << std::endl;
                        if (jugador.getBastonEnergia() >= 50) {                 // Verifica energía suficiente
                            sf::Vector2f posRay = jugador.getPosition();
                            sf::FloatRect boundsRay = jugador.getBounds();
                            float inicioX = posRay.x + boundsRay.width;       // Lado derecho de Ray (para iniciar ataque)
                            float centroY = posRay.y + boundsRay.height / 2.0f;
                            std::cout << "Lado derecho de Ray: (" << inicioX << ", " << centroY << ")" << std::endl;

                            Personaje* enemigo = nullptr;
                            float distanciaMinima = 400.0f;
                            sf::Vector2f posEnemigoMasCercano;

                            // Buscar el NPC infectado más cercano
                            for (auto& personaje : npc) {
                                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {
                                    if (hormiga->getInfectadas() && hormiga->getVida() > 0) {
                                        sf::FloatRect boundsHormiga = hormiga->getBounds();
                                        // Centro hormiga
                                        float centroHormigaX = boundsHormiga.left + boundsHormiga.width / 2.0f;
                                        float centroHormigaY = boundsHormiga.top + boundsHormiga.height / 2.0f;
                                        float dx = inicioX - centroHormigaX;
                                        float dy = centroY - centroHormigaY;
                                        float distancia = std::sqrt(dx * dx + dy * dy);
                                        std::cout << "Centro de Hormiga: (" << centroHormigaX << ", " << centroHormigaY << "), Distancia: " << distancia << std::endl;
                                        if (distancia < distanciaMinima) {               // Si más cercano
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
                                // Centro jefe
                                float centroJefeX = boundsJefe.left + boundsJefe.width / 2.0f;
                                float centroJefeY = boundsJefe.top + boundsJefe.height / 2.0f;
                                float dx = inicioX - centroJefeX;
                                float dy = centroY - centroJefeY;
                                float distancia = std::sqrt(dx * dx + dy * dy);
                                std::cout << "Centro de Rey Hongo: (" << centroJefeX << ", " << centroJefeY << "), Distancia: " << distancia << std::endl;
                                if (distancia <= distanciaMinima) {               // Prioriza jefe si está igual o más cerca
                                    distanciaMinima = distancia;
                                    enemigo = &jefefinal;
                                    posEnemigoMasCercano = sf::Vector2f(centroJefeX, centroJefeY);
                                }
                            }

                            if (enemigo) {                                     // Si hay enemigo seleccionado
                                std::cout << "Enemigo seleccionado en: (" << posEnemigoMasCercano.x << ", " << posEnemigoMasCercano.y << ")" << std::endl;
                                sf::Sprite ataqueLuz;                           // Sprite para ataque luz
                                float dañoAtaque = (jugador.getBastonEnergia() == jugador.getMaxEnergia()) ? 50.0f : 20.0f;
                                jugador.setEstaAtacando(true);                      // Marca que Ray está atacando
                                jugador.setTiempoAtaqueRestante(0.5f);             // Duración del ataque
                                if (dañoAtaque == 50.0f) {                       // Si supergolpe
                                    if (!jugador.getTextureSuperGolpe().loadFromFile("raysupergolpedeluz.png")) {
                                        std::cout << "Error al cargar raysupergolpedeluz.png" << std::endl;
                                    }
                                    jugador.getSpriteAtaque().setTexture(jugador.getTextureSuperGolpe());
                                    std::cout << "Configurando animación para supergolpedeLuz" << std::endl;
                                    ataqueLuz.setTexture(jugador.getProjectileSuperGolpeTexture());
                                } else {                                         // Golpe normal
                                    if (!jugador.getTextureGolpe().loadFromFile("raygolpedeluz.png")) {
                                        std::cout << "Error al cargar raygolpedeluz.png" << std::endl;
                                    }
                                    jugador.getSpriteAtaque().setTexture(jugador.getTextureGolpe());
                                    std::cout << "Configurando animación para golpedeLuz" << std::endl;
                                    ataqueLuz.setTexture(jugador.getProjectileGolpeTexture());
                                }
                                ataqueLuz.setOrigin(ataqueLuz.getLocalBounds().width / 2.0f, ataqueLuz.getLocalBounds().height / 2.0f);
                                ataqueLuz.setPosition(inicioX, centroY);
                                ataqueLuz.setScale(0.5f, 0.5f);               // Ajusta escala del sprite del ataque

                                ataquesLuz.push_back(ataqueLuz);               // Agrega ataque luz a la lista de ataques
                                sf::Vector2f direccion = posEnemigoMasCercano - sf::Vector2f(inicioX, centroY);
                                float longitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
                                if (longitud != 0) {
                                    direccion /= longitud;                     // Normaliza vector dirección
                                }
                                direccionesAtaques.push_back(direccion);       // Guarda dirección para animación y movimiento
                                dañosAtaques.push_back(dañoAtaque);            // Guarda daño correspondiente
                                int consumoEnergia = (dañoAtaque == 50.0f) ? 100 : 50;
                                jugador.consumirEnergia(consumoEnergia);          // Consume energía según ataque

                                std::cout << "Ataque de luz lanzado desde el lado derecho con daño: " << dañoAtaque << ", energía consumida: " << consumoEnergia
                                          << ", energía restante: " << jugador.getBastonEnergia() << std::endl;
                            } else {
                                std::cout << "No se encontró enemigo dentro de 500.0f" << std::endl;
                            }
                        } else {
                            std::cout << "Energía insuficiente para lanzar ataque (necesita 50, tiene " << jugador.getBastonEnergia() << ")" << std::endl;
                        }
                    } else {
                        std::cout << "Fallo en el cast a Ray, tipo de jugador: " << typeid(jugador).name() << std::endl;
                    }
                }
                if (evento.key.code == sf::Keyboard::Space) {               // Tecla espacio para salto
                    jugador.saltar();
                    std::cout << "Saltando" << std::endl;
                }
            }
        }
        // Manejo del botón de reinicio en GAMEOVER
        if (estado == GAMEOVER &&
            evento.type == sf::Event::MouseButtonPressed &&
            evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
            if (botonReiniciar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {  // Si clic en botón reiniciar
                // Reiniciar el juego
                estado = TUTORIAL;
                nivel = 1;
                marcadorPuntaje.setPuntaje(0);
                oleadasJefeFinal = 0;
                laberintoCompletado = false;
                hongosGeneradosDespuesSegundaOleada = false;
                limpiarEsporas();
                inicializarTutorial();
                std::cout << "Juego reiniciado, estado cambiado a TUTORIAL" << std::endl;
                // Reiniciar la vida y energía de Ray
                jugador.restablecerVida();
                jugador.setBastonEnergia(jugador.getMaxEnergia());
                // Reiniciar la vida del jefe
                jefefinal.restablecerVida();
                // Limpiar NPCs, proyectiles y otras listas
                npc.clear();
                ataquesLuz.clear();
                direccionesAtaques.clear();
                dañosAtaques.clear();
                hongos.clear();
                semillas.clear();
            }
        }
    // Manejo de los botones en WIN
    if (estado == WIN &&
        evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
        // Botón "Volver a Jugar"
        if (botonVolverJugar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            estado = TUTORIAL;
            nivel = 1;
            marcadorPuntaje.setPuntaje(0);
            oleadasJefeFinal = 0;
            laberintoCompletado = false;
            hongosGeneradosDespuesSegundaOleada = false;
            limpiarEsporas();
            inicializarTutorial();
            std::cout << "Juego reiniciado desde WIN, estado cambiado a TUTORIAL" << std::endl;
            jugador.restablecerVida();
            jugador.setBastonEnergia(jugador.getMaxEnergia());
            jefefinal.restablecerVida();
            npc.clear();
            ataquesLuz.clear();
            direccionesAtaques.clear();
            dañosAtaques.clear();
            hongos.clear();
            semillas.clear();
        }
        // Botón "Salir"
        if (botonSalir.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            ventana.close();
            std::cout << "Saliendo del juego desde WIN" << std::endl;
        }
    }
}


void Juego::actualizar(float deltaTime) {                              // Metodo para actualizar el estado del juego con el tiempo delta
    jugador.actualizar(deltaTime);                                    // Actualiza al jugador con el tiempo delta recibido
    if (estado == TUTORIAL || estado == JUGANDO || estado == JEFEFINAL) {  // Solo actualiza movimiento si está en estos estados
        float velocidad = 150.0f;                                     // Define la velocidad de movimiento
        float dx = 0.0f, dy = 0.0f;                                  // Variables de desplazamiento horizontal y vertical
        bool moviendo = false;                                        // Bandera para indicar si hay movimiento

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {           // Si se presiona tecla 'A' (izquierda)
            dx = -velocidad * deltaTime;                             // Mueve hacia la izquierda proporcional al deltaTime
            moviendo = true;                                          // Marca que se está moviendo
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {           // Si se presiona tecla 'D' (derecha)
            dx = velocidad * deltaTime;                              // Mueve hacia la derecha proporcional al deltaTime
            moviendo = true;                                          // Marca movimiento
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {           // Si se presiona tecla 'W' (arriba)
            dy = -velocidad * deltaTime;                             // Mueve hacia arriba proporcional al deltaTime
            moviendo = true;                                          // Marca movimiento
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {           // Si se presiona tecla 'S' (abajo)
            dy = velocidad * deltaTime;                              // Mueve hacia abajo proporcional al deltaTime
            moviendo = true;                                          // Marca movimiento
        }

        if (dx != 0.0f && dy != 0.0f) {                              // Si hay movimiento diagonal (ambos dx y dy)
            float factor = 1.0f / std::sqrt(2.0f);                   // Factor para normalizar la velocidad diagonal
            dx *= factor;                                             // Aplica factor a dx
            dy *= factor;                                             // Aplica factor a dy
        }

        if (moviendo) {                                              // Si el jugador se está moviendo
            sf::Vector2f pos = jugador.getPosition();                // Obtiene la posición actual del jugador
            float newX = pos.x + dx;                                 // Calcula nueva posición X
            float newY = pos.y + dy;                                 // Calcula nueva posición Y

            sf::FloatRect bounds = jugador.getBounds();             // Obtiene el área (bounds) del jugador
            bounds.left = newX;                                      // Actualiza bounds con la nueva X
            bounds.top = newY;                                       // Actualiza bounds con la nueva Y

            bool dentroLimites = bounds.left >= 0 && bounds.top >= 0 &&  // Verifica que esté dentro de los límites de la pantalla
                                 bounds.left + bounds.width <= 1920 &&
                                 bounds.top + bounds.height <= 1080;

            std::cout << "Intentando mover Ray a (" << newX << ", " << newY << "), Bounds: ["
                      << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height
                      << "]" << std::endl;                           // Imprime intento de movimiento y bounds

            if (PuedeMoverse(newX, newY, nullptr) && dentroLimites) { // Verifica que el movimiento sea posible y dentro de límites
                jugador.mover(dx, dy);                               // Mueve jugador
                std::cout << "Ray movido a (" << newX << ", " << newY << ")" << std::endl; // Indica movimiento exitoso
            } else {
                std::cout << "Movimiento de Ray bloqueado: "
                          << (dentroLimites ? "Colisión" : "Fuera de límites") << std::endl;  // Indica bloqueo del movimiento y causa
            }
        }

        if (estado == JUGANDO && nivel == 1 && !laberintoCompletado) { // Si está jugando, nivel 1 y laberinto no completado
            sf::Vector2f posJugador = jugador.getPosition();            // Obtiene posición jugador
            if (finalLaberinto.contains(posJugador)) {                  // Verifica si jugador está en área final del laberinto
                laberintoCompletado = true;                             // Marca laberinto como completado
                std::cout << "Ray alcanzó el final del laberinto en (" << posJugador.x << ", " << posJugador.y
                          << "), rectángulo: [" << finalLaberinto.left << ", " << finalLaberinto.top << ", "
                          << finalLaberinto.width << ", " << finalLaberinto.height << "]" << std::endl;  // Imprime evento
            }
        }

        // Actualizar hormigas
        std::cout << "Actualizando hormigas, NPCs: " << npc.size() << ", Estado: " << estado << std::endl;  // Indica actualización NPCs
        sf::Vector2f posicionJugador = jugador.getPosition();        // Obtiene posición actual del jugador
        for (auto it = npc.begin(); it != npc.end();) {              // Itera sobre NPCs
            Personaje* personaje = it->get();                         // Obtiene puntero al personaje
            std::cout << "Procesando NPC en (" << personaje->getBounds().left << ", " << personaje->getBounds().top << ")" << std::endl; // Muestra posición NPC
            if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje)) {  // Intenta castear a Hormigas
                std::cout << "Hormiga detectada, infectada: " << hormiga->getInfectadas() << std::endl;  // Indica estado infección
                if (hormiga->getInfectadas()) {                       // Si está infectada
                    if (estado != JUGANDO || nivel != 1 || laberintoCompletado) {  // Condición para mover hormiga
                        hormiga->moverHaciaJugador(posicionJugador, deltaTime, [this, &personaje](float x, float y) {  // Mueve hormiga hacia jugador
                            return PuedeMoverse(x, y, personaje);       // Verifica si puede moverse a la posición
                        });
                        if (hormiga->getVida() > 0 && hormiga->estaEnPosicionDisparo()) {  // Si puede disparar
                            hormiga->dispararEspora(esporas, direccionesEsporas, rotacionesEsporas, &jugador, esporaHormigaTexture);  // Dispara espora
                        }
                    } else {
                        std::cout << "Hormiga en nivel 1 esperando, laberinto no completado" << std::endl; // Mensaje de espera
                    }
                }
            } else {
                std::cout << "dynamic_cast a Hormigas falló" << std::endl;  // Aviso si casteo falla
            }
            ++it;                                                     // Avanza al siguiente NPC
        }

        // Activar ReyHongo solo después de las oleadas
        if (estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 2 && jefefinal.getVida() > 0) {  // Condición para mover Rey Hongo
            jefefinal.moverHaciaJugador(posicionJugador, deltaTime, [this](float x, float y) {   // Mueve Rey Hongo hacia jugador
                return PuedeMoverse(x, y, &jefefinal);                                       // Verifica si puede moverse
            });
            if (jefefinal.estaEnPosicionDisparo()) {                                        // Si puede disparar
                jefefinal.dispararEspora(esporas, direccionesEsporas, rotacionesEsporas, &jugador, esporaReyHongoTexture);  // Dispara espora
            }
        }

        // Recolectar hongos para restaurar energía
        sf::FloatRect rayBounds = jugador.getBounds();                                   // Obtiene bounds de jugador
        for (size_t i = 0; i < hongos.size();) {                                        // Itera hongos disponibles
            sf::FloatRect hongoBounds = hongos[i].getGlobalBounds();                    // Obtiene bounds del hongo
            if (rayBounds.intersects(hongoBounds)) {                                   // Si jugador intersecta con hongo
                if (jugador.getBastonEnergia() >= jugador.getMaxEnergia()) {                    // Si energía está llena
                    std::cout << "Energía llena (" << jugador.getBastonEnergia() << "), no se recolecta hongo en ("
                              << hongos[i].getPosition().x << ", " << hongos[i].getPosition().y << ")" << std::endl; // Mensaje no recolecta
                    ++i;                                                               // Continúa al siguiente hongo
                    continue;
                }
                float energiaMax = jugador.getMaxEnergia();                                // Obtiene energía máxima
                float energiaRecuperada = energiaMax * 0.5f;                           // Calcula energía recuperada (50%)
                float nuevaEnergia = std::min(jugador.getBastonEnergia() + energiaRecuperada, energiaMax);  // Nueva energía limitada al máximo
                jugador.setBastonEnergia(nuevaEnergia);                                    // Actualiza energía del jugador
                std::cout << "Hongo recolectado en (" << hongos[i].getPosition().x << ", " << hongos[i].getPosition().y
                          << "), energía restaurada: " << energiaRecuperada << ", nueva energía: " << nuevaEnergia << std::endl; // Mensaje recolecta

                // Marca el hongo como recolectado en posiciones originales
                for (auto& posEstado : posicionesOriginalesHongos) {                   // Busca en posiciones originales
                    if (posEstado.first == hongos[i].getPosition()) {                 // Si coincide posición
                        posEstado.second = true;                                       // Marca como recolectado
                        break;
                    }
                }
                marcadorPuntaje.incrementarPuntaje(5);                                // Incrementa puntaje por recolectar
                hongos.erase(hongos.begin() + i);                                    // Elimina hongo del vector
                continue;                                                            // Continúa sin incrementar i por erase
            }
            ++i;                                                                   // Incrementa índice si no hubo erase
        }

        // Recolectar semillas
        for (size_t i = 0; i < semillas.size();) {                                // Itera semillas disponibles
            sf::FloatRect semillaBounds = semillas[i].getGlobalBounds();          // Obtiene bounds de la semilla
            if (rayBounds.intersects(semillaBounds)) {                           // Si jugador intersecta con semilla
                if (jugador.getVida() >= 100) {                                  // Si vida está llena
                    std::cout << "Vida llena (" << jugador.getVida() << "), no se recolecta semilla en ("
                              << semillas[i].getPosition().x << ", " << semillas[i].getPosition().y << ")" << std::endl; // Mensaje no recolecta
                    ++i;                                                         // Continúa al siguiente
                    continue;
                }
                float vidaRecuperada = 15.0f;                                    // Define vida recuperada por semilla
                jugador.aumentarVida(static_cast<int>(vidaRecuperada));          // Aumenta vida del jugador
                std::cout << "Semilla recolectada en (" << semillas[i].getPosition().x << ", " << semillas[i].getPosition().y
                          << "), vida restaurada: " << vidaRecuperada << ", nueva vida: " << jugador.getVida() << std::endl; // Mensaje recolecta

                // Marca la semilla como recolectada en posiciones originales
                for (auto& posEstado : posicionesOriginalesSemillas) {           // Busca posición original
                    if (posEstado.first == semillas[i].getPosition()) {          // Si coincide
                        posEstado.second = true;                                  // Marca como recolectada
                        break;
                    }
                }
                marcadorPuntaje.incrementarPuntaje(5);                          // Incrementa puntaje
                semillas.erase(semillas.begin() + i);                           // Elimina semilla recolectada
                continue;                                                      // Continúa sin incrementar i por erase
            }
            ++i;                                                             // Incrementa índice si no hubo erase
        }

        // Regenerar hongos después de la primera oleada en JEFEFINAL
        if (estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 1) {          // Si en JEFEFINAL, sin NPCs y oleada 1
            for (size_t i = 0; i < posicionesOriginalesHongos.size(); ++i) {       // Itera posiciones originales hongos
                if (posicionesOriginalesHongos[i].second) {                        // Solo si fue recolectado
                    sf::Sprite hongo(hongoTexture);                               // Crea sprite de hongo
                    if (hongoTexture.getSize().x == 0) {                          // Verifica textura válida
                        cout << "no hay textura" << endl;                          // Mensaje error textura
                    } else {
                        hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);  // Centra origen
                        hongo.setPosition(posicionesOriginalesHongos[i].first);    // Posiciona hongo
                        hongo.setScale(0.5f, 0.5f);                                // Escala a la mitad
                    }
                    hongos.push_back(hongo);                                       // Agrega hongo al vector
                    posicionesOriginalesHongos[i].second = false;                  // Resetea estado
                    std::cout << "Hongo regenerado en (" << posicionesOriginalesHongos[i].first.x << ", "
                              << posicionesOriginalesHongos[i].first.y << ")" << std::endl;  // Mensaje regeneración
                }
            }
            // Regenerar semillas después de la primera oleada
            for (size_t i = 0; i < posicionesOriginalesSemillas.size(); ++i) {    // Itera semillas originales
                if (posicionesOriginalesSemillas[i].second) {                     // Solo si fue recolectada
                    sf::Sprite semilla(semillaTexture);                          // Crea sprite semilla
                    if (semillaTexture.getSize().x == 0) {                       // Verifica textura válida
                        cout << "no hay textura" << endl;                        // Mensaje error textura
                    } else {
                        semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);  // Centro origen
                        semilla.setPosition(posicionesOriginalesSemillas[i].first);   // Posiciona semilla
                        semilla.setScale(0.5f, 0.5f);                              // Escala a la mitad
                    }
                    semillas.push_back(semilla);                                // Agrega semilla al vector
                    posicionesOriginalesSemillas[i].second = false;             // Resetea estado
                    std::cout << "Semilla regenerada en (" << posicionesOriginalesSemillas[i].first.x << ", "
                              << posicionesOriginalesSemillas[i].first.y << ")" << std::endl;  // Mensaje regeneración
                }
            }
        }

        // Generar nuevos hongos y nuevas semillas después de la segunda oleada en JEFEFINAL
        if (estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 2 && !hongosGeneradosDespuesSegundaOleada) {  // Condición generación nueva oleada
            hongosGeneradosDespuesSegundaOleada = true;                     // Marca que ya generó hongos
            std::vector<sf::Vector2f> nuevasPosicionesHongos = {            // Define nuevas posiciones para hongos
                {1050, 300}, {900, 400}, {1000, 500}, {400, 500}, {300, 220}
            };
            for (auto& pos : nuevasPosicionesHongos) {                // Itera nuevas posiciones
                sf::Sprite hongo(hongoTexture);                             // Crea sprite hongo
                if (hongoTexture.getSize().x == 0) {                        // Verifica textura válida
                    cout << "No hay Textura" << endl;                       // Mensaje error textura
                } else {
                    hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);  // Centro origen
                    hongo.setPosition(pos);                                 // Posiciona hongo
                    hongo.setScale(0.5f, 0.5f);                             // Escala a la mitad
                }
                hongos.push_back(hongo);                                    // Agrega hongo al vector
                posicionesOriginalesHongos.push_back(std::make_pair(pos, false));  // Agrega posición original con estado falso
                std::cout << "Nuevo hongo generado después de la segunda oleada en (" << pos.x << ", " << pos.y << ")" << std::endl;  // Mensaje generación
            }
            std::vector<sf::Vector2f> nuevasPosicionesSemillas = {          // Define nuevas posiciones semillas
                {100, 300}, {250, 800}, {550, 500}, {350, 50}, {650, 200}, {450, 230}
            };
            for ( auto& pos : nuevasPosicionesSemillas) {              // Itera nuevas posiciones semillas
                sf::Sprite semilla(semillaTexture);                         // Crea sprite semilla
                if (semillaTexture.getSize().x == 0) {                      // Verifica textura válida
                    cout << "No hay textura" <<endl;                        // Mensaje error textura
                } else {
                    semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);  // Centro origen
                    semilla.setPosition(pos);                               // Posiciona semilla
                    semilla.setScale(0.5f, 0.5f);                           // Escala a la mitad
                }
                semillas.push_back(semilla);                                // Agrega semilla al vector
                posicionesOriginalesSemillas.push_back(std::make_pair(pos, false));  // Agrega posición original con estado falso
                std::cout << "Nueva semilla generada después de la segunda oleada en (" << pos.x << ", " << pos.y << ")" << std::endl;  // Mensaje generación
            }
        }

        // Actualizar esporas
        for (size_t i = 0; i < esporas.size();) {                            // Itera esporas activas
            float velocidadEspora = 150.0f;                                 // Velocidad de movimiento de espora
            sf::Vector2f movimiento = direccionesEsporas[i] * velocidadEspora * deltaTime;  // Calcula desplazamiento
            esporas[i].move(movimiento);                                   // Mueve espora

            // Animación de rotación
            rotacionesEsporas[i] += 5.0f;                                  // Incrementa rotación 5 grados
            if (rotacionesEsporas[i] >= 360.0f) {                          // Si rotación supera 360
                rotacionesEsporas[i] -= 360.0f;                            // Reinicia rotación a 0
            }
            esporas[i].setRotation(rotacionesEsporas[i]);                 // Aplica rotación al sprite

            sf::FloatRect esporaBounds(esporas[i].getPosition().x - esporas[i].getGlobalBounds().width / 2,  // Crea bounds centrado para espora
                                       esporas[i].getPosition().y - esporas[i].getGlobalBounds().height / 2,
                                       esporas[i].getGlobalBounds().width, esporas[i].getGlobalBounds().height);
            if (esporaBounds.intersects(jugador.getBounds()) && !jugador.isSaltando()) {  // Si espora colisiona con jugador y no está saltando
                bool dañoAplicado = false;                                  // Bandera daño aplicado
                for (auto& personaje : npc) {                              // Itera NPCs
                    if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {  // Castea a hormiga
                        if (hormiga->getInfectadas()) {                    // Si infectada
                            hormiga->atacar(&jugador);                      // Ataca jugador
                            dañoAplicado = true;                            // Marca daño aplicado
                            break;                                         // Sale del loop
                        }
                    }
                }
                if (!dañoAplicado && estado == JEFEFINAL && npc.empty() && oleadasJefeFinal == 2) {  // Si no daño aplicado, y condiciones
                    jugador.recibirdano(25);                              // Jugador recibe daño 25
                }
                esporas.erase(esporas.begin() + i);                      // Borra espora
                direccionesEsporas.erase(direccionesEsporas.begin() + i); // Borra dirección espora
                rotacionesEsporas.erase(rotacionesEsporas.begin() + i);  // Borra rotación espora
                std::cout << "Espora impactó a Ray, vida: " << jugador.getVida() << std::endl;   // Mensaje impacto
                if (jugador.getVida() <= 0) {                           // Si jugador murió
                    estado = GAMEOVER;                                   // Cambia estado a game over
                    textoNivel.setString("¡Game Over!");                // Cambia texto pantalla
                    limpiarEsporas();                                    // Limpia esporas activas
                    std::cout << "Ray eliminado, cambiando a GAMEOVER" << std::endl;  // Mensaje muerte jugador
                }
                continue;                                              // Continúa sin incrementar i por erase
            } else if (esporaBounds.intersects(jugador.getBounds()) && jugador.isSaltando()) {  // Si jugador está saltando y colisiona
                std::cout << "Espora ignorada durante salto en (" << esporas[i].getPosition().x << ", " << esporas[i].getPosition().y << ")" << std::endl;  // Ignora daño
            }

            if (esporas[i].getPosition().x < 0 || esporas[i].getPosition().x > 1920 ||   // Si espora sale de pantalla horizontal
                esporas[i].getPosition().y < 0 || esporas[i].getPosition().y > 1080) {   // O vertical
                esporas.erase(esporas.begin() + i);                                      // Borra espora
                direccionesEsporas.erase(direccionesEsporas.begin() + i);                // Borra dirección
                rotacionesEsporas.erase(rotacionesEsporas.begin() + i);                  // Borra rotación
                std::cout << "Espora salió de la pantalla" << std::endl;                 // Mensaje espora fuera de pantalla
                continue;                                                              // Continúa sin incrementar i
            }

            ++i;                                                                     // Incrementa índice si no borra
        }

        // Actualizar ataques de luz
        float velocidadAtaque = 300.0f;                                             // Velocidad ataques de luz
        for (size_t i = 0; i < ataquesLuz.size();) {                                // Itera ataques de luz activos
            sf::Vector2f movimiento = direccionesAtaques[i] * velocidadAtaque * deltaTime;  // Calcula movimiento
            ataquesLuz[i].move(movimiento);                                         // Mueve ataque

            sf::FloatRect ataqueBounds(ataquesLuz[i].getPosition().x - ataquesLuz[i].getGlobalBounds().width / 2,  // Bounds centrado ataque
                                       ataquesLuz[i].getPosition().y - ataquesLuz[i].getGlobalBounds().height / 2,
                                       ataquesLuz[i].getGlobalBounds().width, ataquesLuz[i].getGlobalBounds().height);
            bool colisiono = false;                                                 // Bandera colisión ataque

            // Colisión con hormigas
            for (auto& personaje : npc) {                                           // Itera NPCs
                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) { // Castea a hormiga
                    if (hormiga->getInfectadas() && hormiga->getVida() > 0 && ataqueBounds.intersects(hormiga->getBounds())) { // Si infectada y colisiona
                        hormiga->recibirdano(dañosAtaques[i]);                      // Hormiga recibe daño
                        std::cout << "Ataque de luz impactó a Hormiga en (" << hormiga->getPosition().x << ", " << hormiga->getPosition().y
                                  << "), daño causado: " << dañosAtaques[i] << ", vida restante: " << hormiga->getVida() << std::endl; // Mensaje impacto
                        if (hormiga->getVida() <= 0) {                             // Si hormiga murió
                            for (auto it = npc.begin(); it != npc.end();) {       // Itera NPCs
                                if (it->get() == hormiga) {                       // Encuentra hormiga
                                    it = npc.erase(it);                            // Elimina hormiga
                                    marcadorPuntaje.incrementarPuntaje(10);        // Incrementa puntaje
                                    std::cout << "Hormiga eliminada" << std::endl;  // Mensaje eliminación
                                    if (estado == JEFEFINAL && jefefinal.getVida() > 0) {  // Si está en jefe final y Rey Hongo vivo
                                        jefefinal.recibirdano(5);                   // Rey Hongo recibe daño 5
                                        std::cout << "Reyhongo perdió 5 de vida por eliminación de hormiga, vida restante: " << jefefinal.getVida() << std::endl;  // Mensaje daño Rey Hongo
                                    }
                                } else {
                                    ++it;                                           // Continúa iteración
                                }
                            }
                        }
                        colisiono = true;                                       // Marca colisión para borrar ataque
                        break;                                                 // Sale loop NPCs
                    }
                }
            }

            // Colisión con Rey Hongo
            if (estado == JEFEFINAL && jefefinal.getVida() > 0 && ataqueBounds.intersects(jefefinal.getBounds())) {  // Si colisiona con Rey Hongo
                jefefinal.recibirdano(dañosAtaques[i]);                        // Rey Hongo recibe daño
                std::cout << "Ataque de luz impactó a Rey Hongo en (" << jefefinal.getPosition().x << ", " << jefefinal.getPosition().y
                          << "), daño causado: " << dañosAtaques[i] << ", vida restante: " << jefefinal.getVida() << std::endl;  // Mensaje impacto
                if (jefefinal.getVida() <= 0) {                              // Si Rey Hongo murió
                    estado = WIN;                                            // Cambia estado a ganador
                    textoNivel.setString("¡Juego Completado!");             // Cambia texto pantalla
                    std::cout << "Rey Hongo eliminado, cambiando a GAMEOVER" << std::endl;  // Mensaje finalización
                }
                colisiono = true;                                           // Marca colisión para borrar ataque
            }

            if (colisiono || ataquesLuz[i].getPosition().x < 0 || ataquesLuz[i].getPosition().x > 1920 ||   // Si colisionó o está fuera de pantalla horizontal
                ataquesLuz[i].getPosition().y < 0 || ataquesLuz[i].getPosition().y > 1080) {               // O vertical
                ataquesLuz.erase(ataquesLuz.begin() + i);                                             // Borra ataque
                direccionesAtaques.erase(direccionesAtaques.begin() + i);                             // Borra dirección ataque
                dañosAtaques.erase(dañosAtaques.begin() + i);                                         // Borra daño asociado
                std::cout << "Ataque de luz eliminado" << std::endl;                                  // Mensaje eliminación ataque
                continue;                                                                            // Continúa sin incrementar i
            }

            ++i;                                                                                     // Incrementa índice si no borra
        }
    }

    if (estado == TUTORIAL) {                                                                       // Si estado es tutorial
        if (npc.empty()) {                                                                         // Y no hay NPCs activos
            limpiarEsporas();                                                                      // Limpia esporas activas
            estado = JUGANDO;                                                                      // Cambia estado a jugando
            textoNivel.setString("¡Tutorial completado!");                                        // Cambia texto pantalla
            std::cout << "Tutorial completado, cambiando a JUGANDO" << std::endl;                  // Mensaje cambio de estado
            cargarnivel(nivel);                                                                    // Carga nivel actual
        }
    } else if (estado == JUGANDO && nivel == 1) {                                                 // Si estado jugando y nivel 1
        if (npc.empty()) {                                                                         // Y no hay NPCs activos
            limpiarEsporas();                                                                      // Limpia esporas
            estado = JEFEFINAL;                                                                    // Cambia a estado jefe final
            textoNivel.setString("¡Desafío Final: Rey Hongo!");                                    // Cambia texto pantalla
            std::cout << "Todas las hormigas eliminadas en nivel 1, cambiando a JEFEFINAL" << std::endl;  // Mensaje cambio estado
            inicializarJefeFinal();                                                                // Inicializa jefe final
        } else {
            // Verificar si quedan NPCs infectados vivos
            bool quedanInfectados = false;                                                        // Bandera NPC infectados vivos
            for ( auto& personaje : npc) {                                                   // Itera NPCs
                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {               // Castea a hormiga
                    if (hormiga->getInfectadas() && hormiga->getVida() > 0) {                    // Si infectada y viva
                        quedanInfectados = true;                                                  // Marca que quedan infectados
                        break;                                                                   // Sale loop
                    }
                }
            }
            if (!quedanInfectados) {                                                              // Si no quedan infectados
                limpiarEsporas();                                                                  // Limpia esporas
                estado = JEFEFINAL;                                                                // Cambia a jefe final
                textoNivel.setString("¡Desafío Final: Rey Hongo!");                                // Cambia texto pantalla
                std::cout << "Todos los NPCs infectados eliminados en nivel 1, cambiando a JEFEFINAL" << std::endl; // Mensaje cambio estado
                inicializarJefeFinal();                                                            // Inicializa jefe final
            }
        }
    } else if (estado == JEFEFINAL) {                                                             // Si estado jefe final
        if (npc.empty() && oleadasJefeFinal < 2) {                                               // Si no hay NPCs y oleadas < 2
            limpiarEsporas();                                                                      // Limpia esporas
            std::cout << "Todas las hormigas eliminadas, generando oleada " << (oleadasJefeFinal + 1) << std::endl;  // Mensaje oleada nueva
            oleadasJefeFinal++;                                                                   // Incrementa oleadas
            jefefinal.controlarHormigas(npc, posicionesHormigasJefeFinal);                       // Genera/controla hormigas para jefe final
            std::cout << "Oleada " << oleadasJefeFinal << " generada, NPCs: " << npc.size() << std::endl;          // Mensaje oleada generada
        }
    }
}


// Función auxiliar para dibujar barras de salud
void Juego::dibujarBarraSalud(sf::RenderWindow& ventana, Personaje& personaje, float vidaMax) {
    // Obtener posición y tamaño del personaje
    sf::FloatRect bounds = personaje.getBounds();

    // Configurar dimensiones de la barra (ajustadas según escala del personaje)
    float escala = (estado == JUGANDO && nivel == 1) ? 0.5f : 1.0f; // Escala para Ray/hormigas en nivel 1 o normal
    if (&personaje == &jefefinal) escala = 1.5f;                     // Escala mayor para el Rey Hongo
    float anchoBarra = 80.0f * escala;                               // Ancho de la barra según escala
    float altoBarra = 10.0f * escala;                                // Alto de la barra según escala
    float offsetY = 15.0f * escala;                                  // Espacio vertical por encima del sprite

    // Posición centrada encima del personaje
    float x = bounds.left + (bounds.width - anchoBarra) / 2;         // Coordenada X centrada
    float y = bounds.top - offsetY - altoBarra;                      // Coordenada Y justo encima del sprite

    // Barra de fondo (vida máxima, color rojo)
    sf::RectangleShape fondoBarra(sf::Vector2f(anchoBarra, altoBarra));  // Rectángulo de fondo
    fondoBarra.setPosition(x, y);                                         // Posición de la barra de fondo
    fondoBarra.setFillColor(sf::Color::Red);                             // Color rojo para fondo
    fondoBarra.setOutlineColor(sf::Color::Black);                        // Contorno negro
    fondoBarra.setOutlineThickness(1.0f);                                // Grosor del contorno

    // Barra de vida (vida actual, color verde)
    float vida = personaje.getVida();                                    // Vida actual del personaje
    float proporcion = std::max(0.0f, std::min(1.0f, vida / vidaMax));   // Proporción vida actual / vida máxima, limitada entre 0 y 1
    sf::RectangleShape barraVida(sf::Vector2f(anchoBarra * proporcion, altoBarra));  // Rectángulo que representa la vida actual
    barraVida.setPosition(x, y);                                         // Posición igual que la barra de fondo
    barraVida.setFillColor(sf::Color::Green);                           // Color verde para la barra de vida
    barraVida.setOutlineColor(sf::Color::Black);                        // Contorno negro
    barraVida.setOutlineThickness(1.0f);                                // Grosor del contorno

    // Dibujar ambas barras en la ventana
    ventana.draw(fondoBarra);
    ventana.draw(barraVida);
}


// Función para dibujar la barra de energía (siguiendo a Ray, más arriba)
void Juego::dibujarBarraEnergia(sf::RenderWindow& ventana, Ray& ray, float energiaMax) {
    // Obtener posición y tamaño del personaje Ray
    sf::FloatRect bounds = ray.getBounds();

    // Configurar dimensiones de la barra (ajustadas según escala del personaje)
    float escala = (estado == JUGANDO && nivel == 1) ? 0.5f : 1.0f; // Escala para Ray en nivel 1 o normal
    float anchoBarra = 80.0f * escala;                               // Ancho de la barra según escala
    float altoBarra = 10.0f * escala;                                // Alto de la barra según escala
    float offsetY = 30.0f * escala;                                  // Offset vertical, más arriba que la barra de salud

    // Posición centrada encima del personaje, más arriba que la barra de salud
    float x = bounds.left + (bounds.width - anchoBarra) / 2;         // Coordenada X centrada
    float y = bounds.top - offsetY - altoBarra;                      // Coordenada Y más arriba del personaje

    // Barra de fondo (energía máxima, color gris semitransparente)
    sf::RectangleShape fondoBarra(sf::Vector2f(anchoBarra, altoBarra));  // Rectángulo fondo
    fondoBarra.setPosition(x, y);                                         // Posición barra fondo
    fondoBarra.setFillColor(sf::Color(50, 50, 50, 200));                 // Color gris semitransparente
    fondoBarra.setOutlineColor(sf::Color::Black);                        // Contorno negro
    fondoBarra.setOutlineThickness(1.0f);                                // Grosor contorno

    // Barra de energía (energía actual, color azul semitransparente)
    float energia = ray.getBastonEnergia();                              // Obtiene energía actual de Ray
    float proporcion = std::max(0.0f, std::min(1.0f, static_cast<float>(energia) / energiaMax));  // Proporción energía / max, limitada entre 0 y 1
    sf::RectangleShape barraEnergia(sf::Vector2f(anchoBarra * proporcion, altoBarra));  // Rectángulo que representa energía actual
    barraEnergia.setPosition(x, y);                                     // Posición igual a fondo
    barraEnergia.setFillColor(sf::Color(0, 0, 255, 200));               // Color azul semitransparente
    barraEnergia.setOutlineColor(sf::Color::Black);                    // Contorno negro
    barraEnergia.setOutlineThickness(1.0f);                            // Grosor contorno

    // Dibujar barras en la ventana
    ventana.draw(fondoBarra);
    ventana.draw(barraEnergia);
}


// Dibuja elementos según estado
void Juego::renderizar() {
    ventana.clear(sf::Color::Black);                                  // Limpia la ventana con color negro// Intenta convertir jugador a Ray
    if (estado == BIENVENIDA) {                                     // Si el estado es bienvenida
        ventana.draw(spriteFondoBienvenida);                        // Dibuja fondo de bienvenida
        ventana.draw(textoBienvenida);                              // Dibuja texto de bienvenida
        ventana.draw(botonIniciar);                                 // Dibuja botón iniciar
        ventana.draw(textoBoton);                                   // Dibuja texto del botón
    } else if (estado == TUTORIAL) {                               // Si estado es tutorial
        ventana.draw(spriteFondoTutorial);                         // Dibuja fondo tutorial
        for (auto& hongo : hongos) {                         // Dibuja todos los hongos
            ventana.draw(hongo);
        }
        for (auto& semilla : semillas) {                     // Dibuja todas las semillas
            ventana.draw(semilla);
        }
        for (auto& personaje : npc) {                              // Dibuja todos los NPCs vivos
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);                        // Dibuja personaje
                dibujarBarraSalud(ventana, *personaje, 50.0f);     // Dibuja barra de salud con vida máxima 50
            }
        }
        jugador.dibujar(ventana);                                  // Dibuja jugador
        dibujarBarraEnergia(ventana, jugador, 100.0f);                // Dibuja barra de energía con max 100
        dibujarBarraSalud(ventana, jugador, 100.0f);               // Dibuja barra de salud jugador con max 100
        for ( auto& espora : esporas) {                        // Dibuja esporas activas
            ventana.draw(espora);
        }
        for (auto& ataque : ataquesLuz) {                    // Dibuja ataques de luz activos
            ventana.draw(ataque);
        }
    } else if (estado == JUGANDO) {                               // Si estado es jugando
        if (nivel == 1) {                                          // Si nivel 1
            ventana.draw(spriteLaberinto);                         // Dibuja sprite del laberinto
        } else {
            ventana.draw(spriteFondoTutorial);                     // Dibuja fondo tutorial para otros niveles
        }
        for ( auto& hongo : hongos) {                         // Dibuja hongos
            ventana.draw(hongo);
        }
        for (auto& semilla : semillas) {                     // Dibuja semillas
            ventana.draw(semilla);
        }
        // Dibujar NPCs, diferenciando entre infectados y no infectados
        for (auto& personaje : npc) {                              // Itera NPCs
            if (personaje && personaje->getVida() > 0) {           // Si NPC está vivo
                personaje->dibujar(ventana);                        // Dibuja NPC
                // Solo dibujar barra de salud para NPCs infectados
                if (Hormigas* hormiga = dynamic_cast<Hormigas*>(personaje.get())) {  // Intenta castear a Hormigas
                    if (hormiga->getInfectadas()) {                  // Si está infectada
                        dibujarBarraSalud(ventana, *personaje, 50.0f);  // Dibuja barra de salud con max 50
                    }
                }
            }
        }
        marcadorPuntaje.dibujar(ventana, nivel);                   // Dibuja marcador de puntaje con nivel actual
        jugador.dibujar(ventana);                                  // Dibuja jugador
        dibujarBarraEnergia(ventana, jugador, 100.0f);                // Dibuja barra de energía jugador
        dibujarBarraSalud(ventana, jugador, 100.0f);               // Dibuja barra de salud jugador
        for ( auto& espora : esporas) {                        // Dibuja esporas activas
            ventana.draw(espora);
        }
        for ( auto& ataque : ataquesLuz) {                    // Dibuja ataques de luz activos
            ventana.draw(ataque);
        }
    } else if (estado == JEFEFINAL) {                            // Si estado es jefe final
        ventana.draw(spritefondojefefinal);                      // Dibuja fondo jefe final
        for ( auto& hongo : hongos) {                       // Dibuja hongos
            ventana.draw(hongo);
        }
        for ( auto& semilla : semillas) {                   // Dibuja semillas
            ventana.draw(semilla);
        }
        for (auto& personaje : npc) {                            // Dibuja NPCs vivos
            if (personaje && personaje->getVida() > 0) {
                personaje->dibujar(ventana);                      // Dibuja NPC
                dibujarBarraSalud(ventana, *personaje, 50.0f);   // Dibuja barra de salud NPC con max 50
            }
        }
        marcadorPuntaje.dibujar(ventana, nivel);                 // Dibuja marcador puntaje
        jefefinal.dibujar(ventana);                              // Dibuja jefe final
        dibujarBarraSalud(ventana, jefefinal, 200.0f);           // Dibuja barra de salud jefe con max 200
        jugador.dibujar(ventana);                                // Dibuja jugador
        dibujarBarraEnergia(ventana, jugador, 100.0f);              // Dibuja barra de energía jugador
        dibujarBarraSalud(ventana, jugador, 100.0f);             // Dibuja barra de salud jugador
        for ( auto& espora : esporas) {                      // Dibuja esporas
            ventana.draw(espora);
        }
        for ( auto& ataque : ataquesLuz) {                  // Dibuja ataques de luz
            ventana.draw(ataque);
        }
    } else if (estado == GAMEOVER) {                            // Si estado es game over
        ventana.draw(spriteFondoGameover);                       // Dibuja fondo game over
        // Configurar el texto "Game Over" de manera explícita
        textoNivel.setFont(fuente);                              // Establece fuente del texto
        textoNivel.setString("Game Over");                       // Establece el texto a mostrar
        textoNivel.setCharacterSize(300);                        // Tamaño grande del texto
        textoNivel.setFillColor(sf::Color::White);               // Color blanco para texto
        textoNivel.setPosition(550 , 150);                       // Posición centrada en ventana 1920x1080
        ventana.draw(textoNivel);                                // Dibuja texto
        // Dibujar botón de reinicio
        ventana.draw(botonReiniciar);                            // Dibuja botón reiniciar
        ventana.draw(textoReiniciar);                            // Dibuja texto del botón reiniciar
    } else if (estado == WIN) {                                 // Si estado es ganador
        ventana.draw(spriteFondoWinner);                         // Dibuja fondo ganador
        textoNivel.setFont(fuente);                              // Establece fuente texto
        textoNivel.setString("WINNER");                          // Texto ganador
        textoNivel.setCharacterSize(300);                        // Tamaño grande texto
        textoNivel.setFillColor(sf::Color::Green);               // Color verde para victoria
        textoNivel.setPosition(600, 100 );                        // Posición texto
        ventana.draw(textoNivel);                                // Dibuja texto
        ventana.draw(botonVolverJugar);                          // Dibuja botón volver a jugar
        ventana.draw(textoVolverJugar);                          // Dibuja texto botón volver a jugar
        ventana.draw(botonSalir);                                // Dibuja botón salir
        ventana.draw(textoSalir);                                // Dibuja texto botón salir
    }
    ventana.display();                                          // Muestra todo lo dibujado en pantalla
}


// Inicializa NPCs y posición de jugador para tutorial
void Juego::inicializarTutorial() {
    hongos.clear();                                            // Limpia vector de hongos
    semillas.clear();                                          // Limpia vector de semillas
    npc.clear();                                               // Limpia vector de NPCs
    cout << "Inicializando TUTORIAL" << endl;                 // Mensaje en consola

    jugador.setScale(1.0f, 1.0f);                             // Escala normal para el jugador
    jugador.setPosition(10, 10);                               // Posiciona jugador en coordenadas (10, 10)
    cout << "Ray dibujado en 10 , 10" << endl;                // Mensaje en consola

    npc.clear();                                               // Limpia nuevamente vector NPCs (redundante)
    npc.push_back(make_unique<Hormigas>(true));               // Añade hormiga infectada a NPCs
    npc.back()->setPosition(1600, 200);                        // Posiciona esa hormiga en (1600, 200)
    cout << "Hormiga dibujado en 1600 , 100" << endl;         // Mensaje en consola (posible error en Y mostrado)

    npc.push_back(make_unique<Hormigas>(true));               // Añade segunda hormiga infectada
    npc.back()->setPosition(1600, 600);                        // Posiciona esta hormiga en (1600, 600)
    cout << "Hormiga dibujado en 1600 , 500" << endl;         // Mensaje en consola (posible error en Y mostrado)

    // Generar hongos en posiciones fijas
    hongos.clear();                                            // Limpia vector hongos (redundante)
    std::vector<sf::Vector2f> posicionesHongos = {{1200, 300}, {1000, 600}};  // Define posiciones de hongos
    for ( auto& pos : posicionesHongos) {                 // Itera posiciones
        sf::Sprite hongo(hongoTexture);                        // Crea sprite con textura de hongo
        if (hongoTexture.getSize().x == 0) {                   // Si no hay textura válida
            cout << "hongoTexture.getSize().x" << endl;       // Mensaje error textura
        } else {
            hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f);  // Centra origen sprite
            hongo.setPosition(pos);                             // Posiciona sprite en la posición actual
            hongo.setScale(0.5f, 0.5f);                         // Escala a la mitad el tamaño
        }
        hongos.push_back(hongo);                               // Añade el sprite al vector de hongos
    }

    // Generar semillas en el tutorial
    std::vector<sf::Vector2f> posicionesSemillas = {{1300, 400}};  // Define posición de semilla
    for ( auto& pos : posicionesSemillas) {                   // Itera posiciones
        sf::Sprite semilla(semillaTexture);                        // Crea sprite con textura de semilla
        if (semillaTexture.getSize().x == 0) {                     // Si no hay textura válida
            cout << "No hay textura" << endl;                      // Mensaje error textura
        } else {
            semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f);  // Centra origen
            semilla.setPosition(pos);                              // Posiciona sprite en la posición actual
            semilla.setScale(0.5f, 0.5f);                          // Escala a la mitad el tamaño
        }
        semillas.push_back(semilla);                               // Añade sprite al vector semillas
        posicionesOriginalesSemillas.push_back(std::make_pair(pos, false));  // Guarda posición original y estado no recolectado
    }

    std::cout << "Tutorial inicializado con " << hongos.size() << " hongos y " << semillas.size() << " semillas" << std::endl;  // Mensaje resumen
}

bool Juego::PuedeMoverse(float Xnew, float Ynew, Personaje* personajeActual) {
    sf::FloatRect bounds;                                             // Rectángulo para calcular límites del personaje

    if (personajeActual) {                                            // Si se pasa un personaje específico
        bounds = personajeActual->getBounds();                        // Obtiene su área actual
        bounds.left = Xnew;                                           // Actualiza posición X al nuevo valor
        bounds.top = Ynew;                                            // Actualiza posición Y al nuevo valor
    } else {
        bounds = jugador.getBounds();                                 // Si no, usa los límites del jugador por defecto
        bounds.left = Xnew;                                           // Actualiza posición X al nuevo valor
        bounds.top = Ynew;                                            // Actualiza posición Y al nuevo valor
    }

    // Imprime en consola la posición nueva y los límites calculados
    std::cout << "Verificando movimiento a (" << Xnew << ", " << Ynew << "), Bounds: ["
              << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height << "]" << std::endl;

    // Verificar colisión con muros del laberinto solo si está en nivel 1 jugando
    if (estado == JUGANDO && nivel == 1) {
        for (auto& muro : laberintoMuros) {                     // Recorre todos los muros del laberinto
            if (bounds.intersects(muro)) {                            // Si el personaje choca con algún muro
                std::cout << "Colisión detectada con muro en [" << muro.left << ", " << muro.top << ", "
                          << muro.width << ", " << muro.height << "]" << std::endl;  // Mensaje de colisión
                return false;                                          // Movimiento no permitido
            }
        }
    }

    // Verificar colisión con otros personajes NPC
    for ( auto& personaje : npc) {
        if (personaje.get() == personajeActual) continue;             // Ignora el mismo personaje que se mueve
        sf::FloatRect personajeBounds = personaje->getBounds();       // Obtiene límites del otro personaje
        if (bounds.intersects(personajeBounds)) {                     // Si hay intersección (colisión) con otro NPC
            std::cout << "Colisión detectada con personaje en (" << personajeBounds.left << ", "
                      << personajeBounds.top << ")" << std::endl;      // Mensaje colisión
            return false;                                              // Movimiento no permitido
        }
    }

    // Si está en estado jefe final, también checa colisión con el Rey Hongo, excepto si el personaje es el mismo Rey Hongo
    if (estado == JEFEFINAL && personajeActual != &jefefinal) {
        sf::FloatRect jefeBounds = jefefinal.getBounds();             // Obtiene límites del Rey Hongo
        if (bounds.intersects(jefeBounds)) {                          // Si hay colisión con Rey Hongo
            std::cout << "Colisión detectada con Rey Hongo en (" << jefeBounds.left << ", "
                      << jefeBounds.top << ")" << std::endl;           // Mensaje colisión
            return false;                                              // Movimiento no permitido
        }
    }

    // Verificar que la nueva posición esté dentro de los límites de la pantalla
    bool dentroLimites = bounds.left >= 0 && bounds.top >= 0 &&
                         bounds.left + bounds.width <= 1920 &&
                         bounds.top + bounds.height <= 1080;
    if (!dentroLimites) {                                             // Si está fuera de pantalla
        std::cout << "Movimiento fuera de límites" << std::endl;      // Mensaje fuera de límites
        return false;                                                 // Movimiento no permitido
    }

    return true;                                                     // Si no hay colisiones ni fuera de límites, movimiento permitido
}


void Juego::limpiarEsporas() {
    esporas.clear();                                                    // Limpia el vector de esporas
    direccionesEsporas.clear();                                         // Limpia las direcciones de las esporas
    rotacionesEsporas.clear();                                          // Limpia las rotaciones de las esporas
    fuentesEsporas.clear();                                             // Limpia las fuentes relacionadas con esporas
    std::cout << "Esporas limpiadas al cambiar de nivel. Esporas restantes: " << esporas.size() << std::endl; // Mensaje estado limpieza
}

void Juego::inicializarJefeFinal() {
    std::cout << "Inicializando JEFEFINAL" << std::endl;                // Mensaje inicio de inicialización jefe final
    npc.clear();                                                       // Limpia la lista de NPCs
    oleadasJefeFinal = 0;                                              // Resetea contador de oleadas de jefe final
    std::cout << "NPCs limpiados, tamaño: " << npc.size() << std::endl; // Mensaje estado NPCs limpiados
    jugador.setPosition(posicionInicialRayNivel1.x, posicionInicialRayNivel1.y); // Posiciona jugador en inicio nivel 1
    jugador.setScale(1.0f, 1.0f);                                      // Escala normal para jugador
    std::cout << "Ray posicionado en (" << posicionInicialRayNivel1.x << ", "
              << posicionInicialRayNivel1.y << ") con escala (1.0, 1.0)" << std::endl; // Mensaje posición jugador

    jefefinal.setPosition(1500, 500);                                 // Posiciona jefe final (Rey Hongo)
    jefefinal.setScale(1.5f, 1.5f);                                   // Escala mayor para jefe final
    std::cout << "Rey Hongo posicionado en (1000, 800) con escala (1.0, 1.0), vida: "
              << jefefinal.getVida() << std::endl;                    // Mensaje posición y vida jefe final (posiblemente erróneo el texto)

    oleadasJefeFinal++;                                                // Incrementa contador de oleadas
    jefefinal.controlarHormigas(npc, posicionesHormigasJefeFinal);    // Genera/controla primera oleada de hormigas
    std::cout << "Primera oleada generada, NPCs: " << npc.size() << std::endl; // Mensaje oleada creada

    // Generar hongos y guardar posiciones originales
    hongos.clear();                                                   // Limpia vector hongos
    posicionesOriginalesHongos.clear();                              // Limpia posiciones originales hongos
    std::vector<sf::Vector2f> posicionesHongos = {{{300, 750}, {480, 600}, {300, 200}, {100, 450}}}; // Define posiciones hongos
    for ( auto& pos : posicionesHongos) {                       // Itera posiciones hongos
        sf::Sprite hongo(hongoTexture);                              // Crea sprite hongo
        if (hongoTexture.getSize().x == 0) {                        // Si no hay textura válida
            sf::CircleShape circulo(20.f);                           // Crea círculo rojo
            circulo.setFillColor(sf::Color::Red);                    // Color rojo
            circulo.setPosition(pos);                                // Posición círculo (no agregado a vector)
            hongo.setPosition(pos);                                  // Posición sprite hongo
        } else {
            hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f); // Centra origen sprite
            hongo.setPosition(pos);                                 // Posiciona sprite hongo
            hongo.setScale(0.5f, 0.5f);                             // Escala sprite a la mitad
        }
        hongos.push_back(hongo);                                    // Agrega sprite hongo a vector
        posicionesOriginalesHongos.push_back(std::make_pair(pos, false)); // Guarda posición y estado (no recolectado)
    }

    std::cout << "Jefe Final inicializado con " << npc.size() << " hormigas y " << hongos.size() << " hongos" << std::endl; // Mensaje resumen

    // Generar semillas
    std::vector<sf::Vector2f> posicionesSemillas = {{550, 600}, {720, 300}, {300, 700}}; // Posiciones semillas
    for ( auto& pos : posicionesSemillas) {                        // Itera posiciones semillas
        sf::Sprite semilla(semillaTexture);                             // Crea sprite semilla
        if (semillaTexture.getSize().x == 0) {                         // Si no hay textura válida
            cout << "no hay textura" << endl;                          // Mensaje error textura
        } else {
            semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f); // Centra origen sprite
            semilla.setPosition(pos);                                  // Posiciona sprite semilla
            semilla.setScale(0.5f, 0.5f);                              // Escala sprite a la mitad
        }
        semillas.push_back(semilla);                                   // Añade sprite a vector semillas
        posicionesOriginalesSemillas.push_back(std::make_pair(pos, false)); // Guarda posición y estado (no recolectado)
    }

    jefefinal.setPosition(1600, 350);                                 // Cambia posición del jefe final
    jefefinal.controlarHormigas(npc, posicionesHormigasJefeFinal);    // Controla hormigas para jefe final
    std::cout << "Jefe Final inicializado con " << npc.size() << " hormigas, " << hongos.size() << " hongos y " << semillas.size() << " semillas" << std::endl; // Mensaje resumen final
}

void Juego::cargarnivel(int nivel) {
    npc.clear();                                                     // Limpia NPCs para nuevo nivel
    std::cout << "Cargando nivel " << nivel << ", NPCs limpiados, tamaño: " << npc.size() << std::endl; // Mensaje inicio nivel

    if (nivel == 1) {                                                // Si es nivel 1
        jugador.setPosition(posicionInicialRayNivel1.x, posicionInicialRayNivel1.y); // Posiciona jugador inicio nivel 1
        jugador.setScale(0.5f, 0.5f);                                // Escala reducida jugador
        jugador.restablecerVida();                                   // Restablece vida jugador
        jugador.setBastonEnergia(100);                              // Pone energía del bastón al máximo
        laberintoCompletado = false;                               // Resetea estado laberinto
        marcadorPuntaje.setPuntaje(0);                             // Resetea puntaje
        std::cout << "Ray reposicionado en (" << posicionInicialRayNivel1.x << ", "
                  << posicionInicialRayNivel1.y << ") con escala (0.5, 0.5) para nivel " << nivel << std::endl; // Mensaje reposición jugador

        for (size_t i = 0; i < posicionesHormigasNivel1.size() && i < 5; ++i) { // Crea hasta 5 hormigas infectadas
            npc.push_back(std::make_unique<Hormigas>(true));         // Añade hormiga infectada
            npc.back()->setPosition(posicionesHormigasNivel1[i].x, posicionesHormigasNivel1[i].y); // Posiciona hormiga
            npc.back()->setScale(0.5f, 0.5f);                        // Escala hormiga
            std::cout << "Hormiga infectada creada en (" << posicionesHormigasNivel1[i].x << ", "
                      << posicionesHormigasNivel1[i].y << ") con escala (0.5, 0.5), vida: "
                      << npc.back()->getVida() << std::endl;          // Mensaje creación hormiga
        }

        // Crear hormigas no infectadas en el laberinto
        for ( auto& pos : posicionesHormigasNoInfectadas) {
            npc.push_back(std::make_unique<Hormigas>(false));        // Añade hormiga no infectada
            npc.back()->setPosition(pos.x, pos.y);                    // Posiciona hormiga
            npc.back()->setScale(0.5f, 0.5f);                         // Escala hormiga
        }

        // Generar hongos en el nivel del laberinto
        hongos.clear();                                             // Limpia vector hongos
        std::vector<sf::Vector2f> posicionesHongos = {{1400, 300}, {1450, 450}, {1420, 650}}; // Posiciones hongos ejemplo
        for ( auto& pos : posicionesHongos) {                  // Itera posiciones hongos
            sf::Sprite hongo(hongoTexture);                         // Crea sprite hongo
            if (hongoTexture.getSize().x == 0) {                   // Si textura inválida
                cout << "no hay textura" << std::endl;             // Mensaje error textura
            } else {
                hongo.setOrigin(hongoTexture.getSize().x / 2.0f, hongoTexture.getSize().y / 2.0f); // Centra origen sprite
                hongo.setPosition(pos);                             // Posiciona sprite hongo
                hongo.setScale(0.5f, 0.5f);                         // Escala sprite
            }
            // Verificar si la posición del hongo no colisiona con los muros
            sf::FloatRect hongoBounds = hongo.getGlobalBounds();   // Obtiene bounds hongo
            bool colisiona = false;                                 // Variable para colisión
            for ( auto& muro : laberintoMuros) {              // Recorre muros del laberinto
                if (muro.intersects(hongoBounds)) {                // Si colisiona con muro
                    colisiona = true;                               // Marca colisión
                    break;                                          // Sale del loop
                }
            }
            if (!colisiona) {                                       // Si no colisiona con muro
                hongos.push_back(hongo);                            // Agrega hongo al vector
            } else {
                std::cout << "Hongo en (" << pos.x << ", " << pos.y << ") colisiona con un muro, no se agrega" << std::endl; // Mensaje no agregado
            }
        }

        // Generar semillas en el nivel del laberinto
        semillas.clear();                                           // Limpia vector semillas
        posicionesOriginalesSemillas.clear();                       // Limpia posiciones originales semillas
        std::vector<sf::Vector2f> posicionesSemillas = {{1550, 650}, {1450, 300}}; // Posiciones semillas
        for ( auto& pos : posicionesSemillas) {                // Itera posiciones semillas
            sf::Sprite semilla(semillaTexture);                     // Crea sprite semilla
            if (semillaTexture.getSize().x == 0) {                 // Si textura inválida
                cout  <<  "No hay textura"  << std::endl;          // Mensaje error
            } else {
                semilla.setOrigin(semillaTexture.getSize().x / 2.0f, semillaTexture.getSize().y / 2.0f); // Centra origen
                semilla.setPosition(pos);                           // Posiciona semilla
                semilla.setScale(0.5f, 0.5f);                       // Escala semilla
            }
            sf::FloatRect semillaBounds = semilla.getGlobalBounds(); // Obtiene bounds semilla
            bool colisiona = false;                                  // Variable colisión
            for ( auto& muro : laberintoMuros) {               // Recorre muros
                if (muro.intersects(semillaBounds)) {               // Si colisiona con muro
                    colisiona = true;                                // Marca colisión
                    break;                                          // Sale loop
                }
            }
            if (!colisiona) {                                        // Si no colisiona
                semillas.push_back(semilla);                         // Agrega semilla
                posicionesOriginalesSemillas.push_back(std::make_pair(pos, false)); // Guarda posición y estado
            } else {
                std::cout << "Semilla en (" << pos.x << ", " << pos.y << ") colisiona con un muro, no se agrega" << std::endl; // Mensaje no agregado
            }
        }
    } else {                                                      // Para niveles distintos de 1
        jugador.setPosition(100, 800);                            // Posiciona jugador en (100, 800)
        jugador.setScale(1.0f, 1.0f);                             // Escala normal jugador
        std::cout << "Ray reposicionado en (100, 800) con escala (1.0, 1.0) para nivel " << nivel << std::endl; // Mensaje posicionamiento
        for (int i = 0; i < nivel * 5; ++i) {                    // Crea NPCs infectados proporcional al nivel
            npc.push_back(std::make_unique<Hormigas>(true));     // Añade hormiga infectada
            npc.back()->setPosition(300 + i * 100, 800);          // Posiciona hormiga en línea
            npc.back()->setScale(1.0f, 1.0f);                     // Escala normal
            std::cout << "Hormiga infectada creada en (" << 300 + i * 100 << ", 800) con escala (1.0, 1.0), vida: "
                      << npc.back()->getVida() << std::endl;       // Mensaje creación hormiga
        }
    }
    std::cout << "Nivel " << nivel << " cargado, NPCs: " << npc.size() << ", Hongos: " << hongos.size() << std::endl; // Mensaje resumen final
}
