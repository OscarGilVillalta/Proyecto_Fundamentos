#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

// Estructura para manejar la dificultad del juego
struct Difficulty {
    int maxRows;       // Número máximo de filas
    int maxColumns;    // Número máximo de columnas
    int maxBombs;      // Número máximo de bombas

    void reset() {     // Restablece los valores predeterminados
        maxRows = 0;
        maxColumns = 0;
        maxBombs = 0;
    }
} difficulty;

// Estructura para manejar información de jugadores (para modo multijugador)
struct Players {
    string nombre;     // Nombre del jugador
    int points;        // Puntos acumulados
} players[4];

// Estructura para manejar el estado del juego
struct GameStatus {
    bool bombExplote;             // Indica si explotó una bomba
    bool repeatCoordinate;        // Indica si se repitió coordenada
    bool outOfRange;              // Indica si coordenada está fuera de rango
    bool dataTypeInvalid;         // Indica si el tipo de dato es inválido
    bool multiplayer;             // Indica si es modo multijugador
    vector<vector<int>> bomb_explote;  // Coordenadas de bombas explotadas
    vector<vector<int>> repeat;        // Coordenadas repetidas
    vector<vector<int>> treasureXY;    // Coordenadas de tesoros encontrados

    void reset() {     // Restablece el estado del juego
        bombExplote = false;
        repeatCoordinate = false;
        outOfRange = false;
        dataTypeInvalid = false;
        multiplayer = false;
        bomb_explote.clear();
        repeat.clear();
        treasureXY.clear();
    };
} game_status;

// Prototipos de funciones
void print_board();
int game_controls();
void menu_difficulty(int dif);
vector<vector<int>> random_coordinates();
bool prove_coordinates(const vector<int> &coordinate, const vector<vector<int>> &bombXY);
void game_over_message();
string sprite(string typeSprite);
bool victory(int points);
void save_game(const vector<vector<int>>& bombXY);
void load_game(vector<vector<int>>& bombXY);
int multiplayer(int &maxPlayers, int &attempt);

int main() {
    srand(time(NULL));
    sprite("Title");
    menu_difficulty(2);
    return 0;
}

// Función para mostrar gráficos ASCII del juego
string sprite(string typeSprite) {
    if (typeSprite == "Title") {
        cout << " ___                                                                      ___ \n"
                "( _ )--------------------------------------------------------------------( _ )\n"
                " | |                                                                      | | \n"
                " | |                                                                      | | \n"
                " | |   ######  ######  ##  ##  #####           ######  ##  ##  ######     | | \n"
                " | |   ##        ##    ### ##  ##  ##            ##    ##  ##  ##         | | \n"
                " | |   ####      ##    ## ###  ##  ##            ##    ######  ####       | | \n"
                " | |   ##        ##    ##  ##  ##  ##            ##    ##  ##  ##         | | \n"
                " | |   ##      ######  ##  ##  #####             ##    ##  ##  ######     | | \n"
                " | |   ................................................................   | | \n"
                " | |   ######  #####   ######   ####    ####    ##  ##  #####   ######    | | \n"
                " | |   ##      ##  ##  ##      ##  ##  ##       ##  ##  ##  ##  ##        | | \n"
                " | |   ##      #####   ####    ######   ####    ##  ##  #####   ####      | | \n"
                " | |   ##      ##  ##  ##      ##  ##      ##   ##  ##  ##  ##  ##        | | \n"
                " | |   ##      ##  ##  ######  ##  ##   ####     ####   ##  ##  ######    | | \n"
                " | |   ................................................................   | | \n"
                " | |                                                                      | | \n"
                " |_|                                                                      |_| \n"
                "(___)--------------------------------------------------------------------(___)";
    }
    return "";
}

// Función para configurar la dificultad del juego
void menu_difficulty(int dif) {
    switch (dif) {
        case 1: // Fácil
            difficulty.maxRows = 10;
            difficulty.maxColumns = 10;
            difficulty.maxBombs = 3;
            break;
        case 2: // Medio
            difficulty.maxRows = 20;
            difficulty.maxColumns = 20;
            difficulty.maxBombs = 10;
            break;
        case 3: // Difícil
            difficulty.maxRows = 30;
            difficulty.maxColumns = 30;
            difficulty.maxBombs = 16;
            break;
        default: // Multijugador
            break;
    }
    game_controls();
}

// Función para generar coordenadas aleatorias de bombas
vector<vector<int>> random_coordinates() {
    vector<vector<int>> bombXY;
    int bombsTotal = difficulty.maxBombs * difficulty.maxColumns;
    
    while (bombXY.size() < bombsTotal) {
        int bombX = rand() % difficulty.maxRows + 1;
        int bombY = rand() % difficulty.maxColumns + 1;
        vector<int> candidate = {bombX, bombY};
        
        if (find(bombXY.begin(), bombXY.end(), candidate) == bombXY.end()) {
            bombXY.push_back(candidate);
        }
    }
    return bombXY;
}

// Función para verificar si las coordenadas son válidas
bool prove_coordinates(const vector<int> &coordinate, const vector<vector<int>> &bombXY) {
    game_status.repeat.push_back(coordinate);
    
    // Verifica si está fuera de rango
    if ((coordinate[0] <= 0 || coordinate[0] > difficulty.maxRows) ||
        (coordinate[1] <= 0 || coordinate[1] > difficulty.maxColumns)) {
        game_status.outOfRange = true;
        return true;
    }
    
    // Verifica si es coordenada repetida
    if (find(game_status.repeat.begin(), game_status.repeat.end(), coordinate) != game_status.repeat.end()) {
        game_status.repeatCoordinate = true;
        return true;
    }
    
    // Verifica si es una bomba
    if (find(bombXY.begin(), bombXY.end(), coordinate) != bombXY.end()) {
        game_status.bomb_explote.push_back(coordinate);
        game_status.bombExplote = true;
        return true;
    }
    
    game_status.treasureXY.push_back(coordinate);
    return false;
}

// Función para mostrar mensaje de fin de juego
void game_over_message() {
    if (game_status.bombExplote) {
        cout << "¡Has pisado una bomba!\n";
    } else if (game_status.repeatCoordinate) {
        cout << "¡Coordenada repetida! No es válido.\n";
    } else if (game_status.outOfRange) {
        cout << "¡Coordenada fuera del rango establecido!\n";
    } else if (game_status.dataTypeInvalid) {
        cout << "Dato inválido. Debe ingresar un número entero dentro del rango.\n";
    }
}

// Función para mostrar mensaje de victoria
bool victory(int points) {
    cout << "¡Has sobrevivido!\nPuntos: " << points << "\n";
    return true;
}

// Función para guardar el estado del juego
void save_game(const vector<vector<int>>& bombXY) {
    ofstream archivo("partida.txt");
    if (!archivo) {
        cout << "No se pudo guardar la partida.\n";
        return;
    }

    // Guarda la configuración de dificultad
    archivo << difficulty.maxRows << " " << difficulty.maxColumns << " " << difficulty.maxBombs << "\n";

    // Guarda las coordenadas de las bombas
    archivo << bombXY.size() << "\n";
    for (const auto& coord : bombXY) {
        archivo << coord[0] << " " << coord[1] << "\n";
    }

    // Guarda las coordenadas repetidas
    archivo << game_status.repeat.size() << "\n";
    for (const auto& coord : game_status.repeat) {
        archivo << coord[0] << " " << coord[1] << "\n";
    }

    // Guarda las bombas explotadas
    archivo << game_status.bomb_explote.size() << "\n";
    for (const auto& coord : game_status.bomb_explote) {
        archivo << coord[0] << " " << coord[1] << "\n";
    }

    // Guarda los tesoros encontrados
    archivo << game_status.treasureXY.size() << "\n";
    for (const auto& coord : game_status.treasureXY) {
        archivo << coord[0] << " " << coord[1] << "\n";
    }

    archivo.close();
    cout << "Partida guardada correctamente.\n";
}

// Función para cargar una partida guardada
void load_game(vector<vector<int>>& bombXY) {
    ifstream archivo("partida.txt");
    if (!archivo) {
        cout << "No se pudo cargar la partida.\n";
        return;
    }

    // Carga la configuración de dificultad
    archivo >> difficulty.maxRows >> difficulty.maxColumns >> difficulty.maxBombs;

    int n;
    vector<int> coord(2);

    // Carga las coordenadas de las bombas
    archivo >> n;
    bombXY.clear();
    for (int i = 0; i < n; i++) {
        archivo >> coord[0] >> coord[1];
        bombXY.push_back(coord);
    }

    // Carga las coordenadas repetidas
    archivo >> n;
    game_status.repeat.clear();
    for (int i = 0; i < n; i++) {
        archivo >> coord[0] >> coord[1];
        game_status.repeat.push_back(coord);
    }

    // Carga las bombas explotadas
    archivo >> n;
    game_status.bomb_explote.clear();
    for (int i = 0; i < n; i++) {
        archivo >> coord[0] >> coord[1];
        game_status.bomb_explote.push_back(coord);
    }

    // Carga los tesoros encontrados
    archivo >> n;
    game_status.treasureXY.clear();
    for (int i = 0; i < n; i++) {
        archivo >> coord[0] >> coord[1];
        game_status.treasureXY.push_back(coord);
    }

    archivo.close();
    cout << "Partida cargada correctamente.\n";
}

// Función para imprimir el tablero de juego
void print_board() {
    bool bomb_coordinate, treasure_coordinate;
    int points = 0;

    for (int row = 1; row <= difficulty.maxRows; row++) {
        for (int column = 1; column <= difficulty.maxColumns; column++) {
            bomb_coordinate = false;
            treasure_coordinate = false;

            // Verifica si la posición contiene una bomba explotada
            for (const auto& bomb : game_status.bomb_explote) {
                if (bomb[0] == column && bomb[1] == row) {
                    bomb_coordinate = true;
                    break;
                }
            }

            // Verifica si la posición contiene un tesoro
            for (const auto& treasure : game_status.treasureXY) {
                if (treasure[0] == column && treasure[1] == row) {
                    treasure_coordinate = true;
                    break;
                }
            }

            // Imprime el símbolo correspondiente
            if (bomb_coordinate) {
                cout << " ! ";
            } else if (treasure_coordinate) {
                cout << " $ ";
            } else {
                cout << " # ";
            }
        }
        cout << endl;
    }
    points += 20;
}

// Función principal de control del juego
int game_controls() {
    int retire = 0;
    bool lose = false;
    int positionX = 0, positionY = 0, points = 0;
    vector<vector<int>> bombXY = random_coordinates();
    vector<int> coordinate;

    while (true) {
        // Opción para guardar o cargar partida
        cout << "\nOpciones:\n1. Continuar\n2. Guardar partida\n3. Cargar partida\n4. Salir\n";
        int opcion;
        cin >> opcion;

        if (opcion == 2) {
            save_game(bombXY);
            continue;
        } else if (opcion == 3) {
            load_game(bombXY);
            print_board();
            continue;
        } else if (opcion == 4) {
            break;
        }

        // Solicita coordenadas al jugador
        cout << "Ingrese sus coordenadas (X Y): ";
        cin >> positionX >> positionY;
        coordinate = {positionX, positionY};

        // Verifica las coordenadas
        lose = prove_coordinates(coordinate, bombXY);
        if (lose) {
            game_over_message();
            break;
        } else if (points >= 60) {
            cout << "¿Desea retirarse? (1=Sí, 0=No): ";
            cin >> retire;
            if (retire == 1) {
                victory(points);
                break;
            }
        }

        print_board();
    }

    return 0;
}

// Función para modo multijugador (a implementar)
int multiplayer(int &maxPlayers, int &attempt) {
    // Implementación pendiente
    return 0;
}