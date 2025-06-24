#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// Función para guardar partida (ahora con nombre personalizado)
void saveGame(int points, const vector<vector<int>>& player_moves, const vector<vector<int>>& bomb_positions) {
    string gameName;
    cout << "\nIngresa un nombre para guardar la partida (ej: 'Partida1'): ";
    cin >> gameName;

    ofstream file(gameName + ".txt");
    if (file.is_open()) {
        // Guardar metadatos
        file << "Nombre: " << gameName << endl;
        file << "Puntos: " << points << endl;

        // Guardar jugadas (posiciones seguras)
        file << "\nJugadas seguras:\n";
        for (const auto& move : player_moves) {
            file << move[0] << " " << move[1] << endl;
        }

        // Guardar bombas
        file << "\nBombas:\n";
        for (const auto& bomb : bomb_positions) {
            file << bomb[0] << " " << bomb[1] << endl;
        }

        file.close();
        cout << "¡Partida guardada como '" << gameName << ".txt'!\n";
    } else {
        cout << "Error al guardar.\n";
    }
}

int findTreasure() {
    srand(time(NULL));

    int leave_option = 0;
    int points = 0;
    int lose_condition = -1;
    vector<vector<int>> player_moves;
    vector<vector<int>> bomb_positions;
    vector<int> temp_bomb_x;

    // Generar bombas aleatorias
    bool is_duplicate;
    for (int bomb_y = 1; bomb_y <= 5; bomb_y++) {
        for (int bomb_x_count = 1; bomb_x_count <= 5; bomb_x_count++) {
            int bomb_x;
            do {
                is_duplicate = false;
                bomb_x = (rand() % 10) + 1;
                for (int x : temp_bomb_x) {
                    if (x == bomb_x) {
                        is_duplicate = true;
                        break;
                    }
                }
            } while (is_duplicate);
            temp_bomb_x.push_back(bomb_x);
        }

        for (int x : temp_bomb_x) {
            bomb_positions.push_back({x, bomb_y});
        }
        temp_bomb_x.clear();
    }

    bool is_playing = true;
    while (is_playing) {
        // Opción para guardar después de 3 tesoros
        if (leave_option >= 3) {
            cout << "\nOpcion extra: Escribe '99' para GUARDAR y salir\n";
        }

        if (leave_option == 3) {
            cout << "¡Puedes retirarte con tus puntos! (Escribe '3' para salir)\n";
            cin >> lose_condition;
        }

        int x, y;
        cout << "Ingresa coordenadas (X Y): ";
        cin >> x;

        // Guardar y salir si elige 99
        if (x == 99 && leave_option >= 3) {
            saveGame(points, player_moves, bomb_positions);
            return points;
        }

        cin >> y;

        // Validar coordenadas
        if (x < 1 || x > 10 || y < 1 || y > 5) {
            lose_condition = 0; // Fuera de rango
        } else {
            player_moves.push_back({x, y});
        }

        // Verificar repetición
        for (size_t i = 0; i < player_moves.size() - 1; i++) {
            if (player_moves[i][0] == x && player_moves[i][1] == y) {
                lose_condition = 1; // Jugada repetida
            }
        }

        // Dibujar tablero
        cout << "  ";
        for (int header = 1; header <= 10; header++) cout << " " << header;
        cout << endl;

        for (int row = 1; row <= 5; row++) {
            cout << row << " ";
            for (int col = 1; col <= 10; col++) {
                bool is_bomb = false;
                for (const auto& bomb : bomb_positions) {
                    if (bomb[0] == col && bomb[1] == row) {
                        is_bomb = true;
                        break;
                    }
                }

                if (lose_condition == 2 && is_bomb && x == col && y == row) {
                    cout << " ! "; // Bomba pisada
                } else {
                    bool is_move = false;
                    for (const auto& move : player_moves) {
                        if (move[0] == col && move[1] == row) {
                            is_move = true;
                            break;
                        }
                    }
                    cout << (is_move ? " $ " : " # "); // Tesoro o casilla vacía
                }
            }
            cout << endl;
        }

        // Verificar condiciones de fin de juego
        for (const auto& bomb : bomb_positions) {
            if (bomb[0] == x && bomb[1] == y) {
                lose_condition = 2; // Pisó bomba
                break;
            }
        }

        switch (lose_condition) {
            case 0: cout << "¡Coordenadas inválidas!\n"; break;
            case 1: cout << "¡Jugada repetida!\n"; break;
            case 2: cout << "¡Boom! Pisaste una bomba.\n"; break;
            case 3: cout << "¡Te retiraste con " << points << " puntos!\n"; break;
            default:
                points += 10;
                leave_option++;
                cout << "¡Tesoro encontrado! Puntos: " << points << endl;
                continue;
        }
        is_playing = false;
    }
    return points;
}

int main() {
    cout << "=== BUSCA TESOROS ===" << endl;
    int points = findTreasure();
    cout << "Puntos finales: " << points << endl;
    return 0;
}