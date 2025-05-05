#ifndef PUNTAJE_H
#define PUNTAJE_H
#include <iostream>
#include <vector>
#include <map>
using namespace std;
class Puntaje{
    public:
    map<string, int> puntajes;
	private:
    void cargararchivo(string& archivo);
	void guardararchivo(string& archivo);
	void agregarpuntaje(string& nombre, int puntaje);
	vector <pair<string,int >> obtenertop10();
};
#endif