#include "reyhongo.h"
#include "hormigas.h"


Reyhongo::Reyhongo()
    : Personaje("rey hongo.png", 200) {
    energiaHormigas = 0;
}


void Reyhongo::dispararesporas(Personaje* objetivo) {
    objetivo->recibirdano(20);
}


void Reyhongo::controlarhormigas(vector<Personaje*>& npc) {
    Hormigas* nuevaInfectada = new Hormigas(true);
    npc.push_back(nuevaInfectada);
    cout << "El Rey Hongo ha controlado una nueva hormiga infectada!" << endl;


    energiaHormigas = 0;
    for (auto& hormiga : npc) {
        Hormigas* h = dynamic_cast<Hormigas*>(hormiga);
        if (h && h->getInfectadas()) {
            energiaHormigas += 10;
        }
    }
    cout << "Energía del Rey Hongo ahora: " << energiaHormigas << endl;
}

// Dibujar
void Reyhongo::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}
