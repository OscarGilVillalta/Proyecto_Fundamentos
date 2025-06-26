#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Calcula el puntaje de una mano
int calcularPuntaje(const vector<string>& mano) {
    int puntaje = 0;
    int ases = 0;

    for (int i = 0; i < mano.size(); i++) {
        string carta = mano[i];
        if (carta == "J" || carta == "Q" || carta == "K") {
            puntaje += 10;
        } else if (carta == "A") {
            puntaje += 11;
            ases++;
        } else {
            puntaje += stoi(carta);
        }
    }

    while (puntaje > 21 && ases > 0) {
        puntaje -= 10;
        ases--;
    }

    return puntaje;
}

// Reparte una carta del mazo a la mano
void repartirCarta(vector<string>& mazo, vector<string>& mano) {
    if (!mazo.empty()) {
        mano.push_back(mazo.back());
        mazo.pop_back();
    } else {
        cout << "El mazo está vacío, no se puede repartir más cartas." << endl;
    }
}

int main() {
    srand(time(0));

    vector<string> mazo;
    vector<string> jugador;
    vector<string> dealer;

    vector<string> valores = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < valores.size(); j++) {
            mazo.push_back(valores[j]);
        }
    }

    random_shuffle(mazo.begin(), mazo.end());

    repartirCarta(mazo, jugador);
    repartirCarta(mazo, jugador);
    repartirCarta(mazo, dealer);
    repartirCarta(mazo, dealer);

    char decision;

    // TURNO DEL JUGADOR
    do {
        cout << "\nTus cartas: ";
        for (string carta : jugador) cout << carta << " ";
        cout << "\nPuntaje: " << calcularPuntaje(jugador) << endl;

        if (calcularPuntaje(jugador) >= 21) break;

        cout << "¿Pedir carta (p) o plantarte (s)? ";
        cin >> decision;

        if (decision == 'p') {
            repartirCarta(mazo, jugador);
            cout << "Repartiendo carta..." << endl;
        } else if (decision == 's') {
            cout << "Te plantas." << endl;
        } else {
            cout << "Opción no válida." << endl;
        }

    } while (decision != 's');

    int puntajeJugador = calcularPuntaje(jugador);

    // TURNO DEL DEALER
    cout << "\nTurno del dealer..." << endl;
    while (calcularPuntaje(dealer) < 17) {
        repartirCarta(mazo, dealer);
        cout << "El dealer pide carta..." << endl;
    }

    int puntajeDealer = calcularPuntaje(dealer);

    // MOSTRAR RESULTADOS
    cout << "\nCartas del dealer: ";
    for (string carta : dealer) cout << carta << " ";
    cout << "\nPuntaje del dealer: " << puntajeDealer << endl;

    cout << "\nCartas del jugador: ";
    for (string carta : jugador) cout << carta << " ";
    cout << "\nPuntaje del jugador: " << puntajeJugador << endl;

    // DETERMINAR GANADOR
    cout << "\n--- Resultado ---" << endl;

    if (puntajeJugador > 21) {
        cout << "Te pasaste de 21. Pierdes " << endl;
    } else if (puntajeDealer > 21) {
        cout << "El dealer se pasó de 21. ¡Ganas! " << endl;
    } else if (puntajeJugador > puntajeDealer) {
        cout << "¡Ganas con mejor puntaje! " << endl;
    } else if (puntajeDealer > puntajeJugador) {
        cout << "El dealer gana. " << endl;
    } else {
        cout << "Empate. " << endl;
    }

    return 0;
}
