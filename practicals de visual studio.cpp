#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

int main() {
    srand(time(0));  // Semilla para generar aleatorios

    vector<string> mazo;        // El mazo de cartas
    vector<string> jugador;     // Mano del jugador
    vector<string> dealer;      // Mano del dealer
    char jugarOtra;             // Para preguntar si quiere jugar otra vez

    // Valores posibles de cartas
    vector<string> valores = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    // Llenar el mazo con 4 copias de cada valor
    for (int i = 0; i < 4; i++) {           // 4 palos
        for (int j = 0; j < valores.size(); j++) {  // Cada carta
            mazo.push_back(valores[j]);    // Añade la carta al mazo
        }
    }

    return 0;
}
