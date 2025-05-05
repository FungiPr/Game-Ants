#include "juego.h"
#include <locale>
int main(){
    setlocale(LC_ALL,"Spanish");
    Juego ants;
    ants.iniciar();
    return 0;
}