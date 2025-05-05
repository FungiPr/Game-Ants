cmake_minimum_required(VERSION 3.28)
project(PROYECTO_VIDEOJUEGO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_ROOT "C:/SFML-2.5.1")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

# Encuentra el paquete SFML
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)

# Crear el ejecutable con todos los archivos fuente
add_executable(PROYECTO_VIDEOJUEGO
        main.cpp
        hormigas.cpp
        juego.cpp
        personaje.cpp
        #puntaje.cpp
        ray.cpp
        reyhongo.cpp
)

# Vincula las bibliotecas de SFML
target_link_libraries(PROYECTO_VIDEOJUEGO sfml-system sfml-window sfml-graphics sfml-network sfml-audio)