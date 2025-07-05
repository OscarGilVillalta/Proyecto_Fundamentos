#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

// Estructura para manejar la dificultad del juego
struct Difficulty
{
    int maxRows;    // Número máximo de filas
    int maxColumns; // Número máximo de columnas
    int maxBombs;   // Número máximo de bombas
    int maxBullets; // Número máximo de balas
    int maxShields; // Número máximo de escudos
    void reset()
    { // Restablece los valores predeterminados
        maxRows = 0;
        maxColumns = 0;
        maxBombs = 0;
        maxBullets = 0;
        maxShields = 0;
    }
} difficulty;

struct GameData
{
    int max_players;
    bool game_status = true;
    vector<vector<int>> bomb_explote; // Coordenadas de bombas explotadas
    vector<vector<int>> repeat;       // Coordenadas repetidas
    vector<vector<int>> treasureXY;   // Coordenadas de tesoros encontrados

    void reset()
    {
        bomb_explote.clear();
        repeat.clear();
        treasureXY.clear();
    }
} game_data;

// Estructura para manejar información de jugadores (para modo multijugador)
struct Players
{
    string name;   // Nombre del jugador
    int points;    // Puntos acumulados
    int bullets;   // Balas disponibles
    int shields;   // Escudo disponible
    bool is_alive; // Indica si el jugador está vivo
} players[5];

// Estructura para manejar el estado del juego
struct ErrorType
{
    bool bombExplote;      // Indica si explotó una bomba
    bool repeatCoordinate; // Indica si se repitió coordenada
    bool outOfRange;       // Indica si coordenada está fuera de rango
    bool dataTypeInvalid;  // Indica si el tipo de dato es inválido

    void reset()
    { // Restablece el estado del juego
        bombExplote = false;
        repeatCoordinate = false;
        outOfRange = false;
        dataTypeInvalid = false;
    };
} error_type;

// Prototipos de funciones
void print_board();
int game_menu();
void menu_difficulty(int dif);
vector<vector<int>> random_coordinates();
bool prove_coordinates(const vector<int> &coordinate, const vector<vector<int>> &bombXY);
void game_over_message();
string sprite(string typeSprite);
bool victory(int points);
void save_game(const vector<vector<int>> &bombXY);
void load_game(vector<vector<int>> &bombXY);
void game_multiplayer(vector<vector<int>> &bombXY);
void reset_game_state(vector<vector<int>>& bombXY);
void pistol_shot(int &turn, bool &lose);
void shield_protection(int &turn, bool &lose);
void player_action(int &turn, bool &lose);
void player_configuration();

int main()
{
    srand(time(NULL));
    sprite("Title");
    game_menu();
    return 0;
}

// Función para mostrar gráficos ASCII del juego
string sprite(string typeSprite)
{
    if (typeSprite == "Title")
    {
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
                " | |     ##    ##  ##  ##      ##  ##  ##       ##  ##  ##  ##  ##        | | \n"
                " | |     ##    #####   ####    ######   ####    ##  ##  #####   ####      | | \n"
                " | |     ##    ##  ##  ##      ##  ##      ##   ##  ##  ##  ##  ##        | | \n"
                " | |     ##    ##  ##  ######  ##  ##   ####     ####   ##  ##  ######    | | \n"
                " | |   ................................................................   | | \n"
                " | |                                                                      | | \n"
                " |_|                                                                      |_| \n"
                "(___)--------------------------------------------------------------------(___)\n";
    }
    return "";
}

// Función para configurar la dificultad del juego
void menu_difficulty(int dif)
{
    switch (dif)
    {
    case 1: // Fácil
        difficulty.maxRows = 10;
        difficulty.maxColumns = 10;
        difficulty.maxBombs = 30;
        difficulty.maxBullets = 10;
        difficulty.maxShields = 5;
        break;
    case 2: // Medio
        difficulty.maxRows = 20;
        difficulty.maxColumns = 20;
        difficulty.maxBombs = 80;
        difficulty.maxBullets = 10;
        difficulty.maxShields = 5;
        break;
    case 3: // Difícil
        difficulty.maxRows = 30;
        difficulty.maxColumns = 30;
        difficulty.maxBombs = 100;
        difficulty.maxBullets = 10;
        difficulty.maxShields = 5;
        break;
    default: // Multijugador
        break;
    }
}

// Función para generar coordenadas aleatorias de bombas
vector<vector<int>> random_coordinates()
{
    vector<vector<int>> bombXY;
    int bombsTotal = difficulty.maxBombs;

    while (bombXY.size() < bombsTotal)
    {
        int bombX = rand() % difficulty.maxRows + 1;
        int bombY = rand() % difficulty.maxColumns + 1;
        vector<int> candidate = {bombX, bombY};

        if (find(bombXY.begin(), bombXY.end(), candidate) == bombXY.end())
        {
            bombXY.push_back(candidate);
        }
    }
    return bombXY;
}

// Función para verificar si las coordenadas son válidas
bool prove_coordinates(const vector<int> &coordinate, const vector<vector<int>> &bombXY)
{

    // Verifica si está fuera de rango
    if ((coordinate[0] <= 0 || coordinate[0] > difficulty.maxColumns) ||
        (coordinate[1] <= 0 || coordinate[1] > difficulty.maxRows))
    {
        error_type.outOfRange = true;
        return true;
    }

    // Verifica si es coordenada repetida
    if (find(game_data.repeat.begin(), game_data.repeat.end(), coordinate) != game_data.repeat.end())
    {
        error_type.repeatCoordinate = true;
        return true;
    }

    // Verifica si es una bomba
    if (find(bombXY.begin(), bombXY.end(), coordinate) != bombXY.end())
    {
        game_data.bomb_explote.push_back(coordinate);
        error_type.bombExplote = true;
        return true;
    }

    game_data.treasureXY.push_back(coordinate);
    return false;
}

// Función para mostrar mensaje de fin de juego
void game_over_message()
{
    if (error_type.bombExplote)
    {
        cout << "¡Has pisado una bomba!\n";
    }
    else if (error_type.repeatCoordinate)
    {
        cout << "¡Coordenada repetida! No es válido.\n";
    }
    else if (error_type.outOfRange)
    {
        cout << "¡Coordenada fuera del rango establecido!\n";
    }
    else if (error_type.dataTypeInvalid)
    {
        cout << "Dato inválido. Debe ingresar un número entero dentro del rango.\n";
    }
}

// Función para mostrar mensaje de victoria
bool victory(int points)
{
    cout << "¡Has sobrevivido!\nPuntos: " << points << "\n";
    return true;
}

// Función para guardar el estado del juego
void save_game(const vector<vector<int>> &bombXY)
{
    ofstream archivo("partida.txt");
    if (!archivo)
    {
        cout << "No se pudo guardar la partida.\n";
        return;
    }

    // Guarda la configuración de dificultad
    archivo << difficulty.maxRows << " " << difficulty.maxColumns << " " << difficulty.maxBombs << "\n";

    // Guarda las coordenadas de las bombas
    archivo << bombXY.size() << "\n";
    for (const vector<int> &coordinates : bombXY)
    {
        archivo << coordinates[0] << " " << coordinates[1] << "\n";
    }

    // Guarda las coordenadas repetidas
    archivo << game_data.repeat.size() << "\n";
    for (const vector<int> &coordinates : game_data.repeat)
    {
        archivo << coordinates[0] << " " << coordinates[1] << "\n";
    }

    // Guarda las bombas explotadas
    archivo << game_data.bomb_explote.size() << "\n";
    for (const vector<int> &coordinates : game_data.bomb_explote)
    {
        archivo << coordinates[0] << " " << coordinates[1] << "\n";
    }

    // Guarda los tesoros encontrados
    archivo << game_data.treasureXY.size() << "\n";
    for (const vector<int> &coordinates : game_data.treasureXY)
    {
        archivo << coordinates[0] << " " << coordinates[1] << "\n";
    }

    archivo.close();
    cout << "Partida guardada correctamente.\n";
}

// Función para cargar una partida guardada
void load_game(vector<vector<int>> &bombXY)
{
    ifstream archivo("partida.txt");
    if (!archivo)
    {
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
    for (int i = 0; i < n; i++)
    {
        archivo >> coord[0] >> coord[1];
        bombXY.push_back(coord);
    }

    // Carga las coordenadas repetidas
    archivo >> n;
    game_data.repeat.clear();
    for (int i = 0; i < n; i++)
    {
        archivo >> coord[0] >> coord[1];
        game_data.repeat.push_back(coord);
    }

    // Carga las bombas explotadas
    archivo >> n;
    game_data.bomb_explote.clear();
    for (int i = 0; i < n; i++)
    {
        archivo >> coord[0] >> coord[1];
        game_data.bomb_explote.push_back(coord);
    }

    // Carga los tesoros encontrados
    archivo >> n;
    game_data.treasureXY.clear();
    for (int i = 0; i < n; i++)
    {
        archivo >> coord[0] >> coord[1];
        game_data.treasureXY.push_back(coord);
    }

    archivo.close();
    cout << "Partida cargada correctamente.\n";

    game_multiplayer(bombXY);
}

// Función para imprimir el tablero de juego
void print_board()
{
    bool bomb_coordinate, treasure_coordinate;

    cout << "\n=== TABLERO DE JUEGO ===\n";
    // Imprimir números de columnas
    cout << "   ";
    for (int col = 1; col <= difficulty.maxColumns; col++)
    {
        if (col < 10)
            cout << " " << col << " ";
        else
            cout << col << " ";
    }
    cout << endl;

    for (int row = 1; row <= difficulty.maxRows; row++)
    {
        // Imprimir número de fila
        if (row < 10)
            cout << " " << row << " ";
        else
            cout << row << " ";

        for (int column = 1; column <= difficulty.maxColumns; column++)
        {
            bomb_coordinate = false;
            treasure_coordinate = false;

            // Verifica si la posición contiene una bomba explotada
            for (const vector<int> &bomb : game_data.bomb_explote)
            {
                if (bomb[0] == column && bomb[1] == row)
                {
                    bomb_coordinate = true;
                    break;
                }
            }

            // Verifica si la posición contiene un tesoro
            for (const vector<int> &treasure : game_data.treasureXY)
            {
                if (treasure[0] == column && treasure[1] == row)
                {
                    treasure_coordinate = true;
                    break;
                }
            }

            // Imprime el símbolo correspondiente
            if (bomb_coordinate)
            {
                cout << " ! ";
            }
            else if (treasure_coordinate)
            {
                cout << " $ ";
            }
            else
            {
                cout << " # ";
            }
        }
        cout << endl;
    }
    cout << "Leyenda: # = Sin explorar, $ = Tesoro, ! = Bomba\n";
}

int game_menu()
{
    int opcion;
    vector<vector<int>> bombXY;

    while (true)
    {

        cout << R"( 
╔══════════════════════════════════════════════╗
║         ███ ENCUENTRA EL TESORO ███          ║
║──────────────────────────────────────────────║
║  1. Jugar modo Solitario                     ║
║  2. Jugar modo Multijugador                  ║
║  3. Cargar partida                           ║
║  4. Guardar partida                          ║
║  5. Salir del juego                          ║
╚══════════════════════════════════════════════╝
Seleccione una opción: )";
        cin >> opcion;

        if (opcion == 1)
        {
            reset_game_state(bombXY);
            game_data.max_players = 1;

            int dif;
            cout << "\nSelecciona dificultad:\n";
            cout << "1. Fácil (10x10 - 30 bombas)\n";
            cout << "2. Medio (20x20 - 80 bombas)\n";
            cout << "3. Difícil (30x30 - 100 bombas)\n";
            cout << "Opción: ";
            cin >> dif;

            menu_difficulty(dif);
            bombXY = random_coordinates();
            player_configuration();
            game_multiplayer(bombXY);
        }
        else if (opcion == 2)
        {
            reset_game_state(bombXY);

            int dif;
            cout << "\nSelecciona dificultad para Multijugador:\n";
            cout << "1. Fácil (10x10 - 30 bombas)\n";
            cout << "2. Medio (20x20 - 80 bombas)\n";
            cout << "3. Difícil (30x30 - 100 bombas)\n";
            cout << "Opción: ";
            cin >> dif;

            cout << "\n¿Con cuántos jugadores deseas jugar? (1-4): ";
            cin >> game_data.max_players;

            if (game_data.max_players < 1 || game_data.max_players > 4)
            {
                cout << "Número de jugadores inválido. Debe ser entre 1 y 4.\n";
                continue;
            }

            menu_difficulty(dif);
            player_configuration();
            bombXY = random_coordinates();
            game_multiplayer(bombXY);
        }
        else if (opcion == 3)
        {
            load_game(bombXY);
            print_board();
        }
        else if (opcion == 4)
        {
            save_game(bombXY);
        }
        else if (opcion == 5)
        {
            cout << "¡Gracias por jugar! Hasta la próxima.\n";
            break;
        }
        else
        {
            cout << "Opción no válida. Intenta de nuevo.\n";
        }
    }

    return 0;
}

// Función para modo multijugador
void game_multiplayer(vector<vector<int>> &bombXY)
{
    bool lose = false;
    int positionX = 0, positionY = 0, retire = 0;
    vector<int> coordinate;

    // Mostrar tablero inicial
    print_board();

    while (game_data.game_status)
    {
        for (int turn = 0; turn < game_data.max_players; ++turn)
        {
            if (players[turn].is_alive)
            {

                cout << "\nTurno de " << players[turn].name << " (Puntos: " << players[turn].points << ")\n";
                cout << "Ingrese sus coordenadas (X Y): \n";
                cout << "X: ";
                cin >> positionX;
                cout << "Y: ";
                cin >> positionY;

                coordinate = {positionX, positionY};

                // Verifica las coordenadas
                lose = prove_coordinates(coordinate, bombXY);
                game_data.repeat.push_back(coordinate);

                player_action(turn, lose);

                if (lose)
                {
                    game_over_message();
                    if (error_type.bombExplote)
                    {
                        players[turn].is_alive = false;
                        cout << players[turn].name << " ha perdido!\n";
                    }
                }
                else
                {
                    players[turn].points += 20;
                    cout << "¡Tesoro encontrado! +20 puntos\n";
                }

                print_board();

                if (players[turn].points >= 60 && players[turn].is_alive)
                {
                    cout << "¿Desea retirarse? (1=Sí, 0=No): ";
                    cin >> retire;
                    if (retire == 1)
                    {
                        victory(players[turn].points);
                        players[turn].is_alive = false;
                    }
                }
            }
        }

        // Verificar si algún jugador sigue vivo
        game_data.game_status = false;
        for (int i = 0; i < game_data.max_players; i++)
        {
            if (players[i].is_alive)
            {
                game_data.game_status = true;
                break;
            }
        }
    }

    cout << "\n=== FIN DEL JUEGO ===\n";
    for (int i = 0; i < game_data.max_players; i++)
    {
        cout << players[i].name << ": " << players[i].points << " puntos\n";
    }
}

void player_configuration()
{
    for (int i = 0; i < game_data.max_players; i++)
    {
        cout << "Nombre del jugador " << i + 1 << ": ";
        cin >> players[i].name;
        players[i].points = 0;
        players[i].is_alive = true;
        players[i].bullets = difficulty.maxBullets;
        players[i].shields = difficulty.maxShields;
    }
}

// Función para reiniciar el estado del juego
void reset_game_state(vector<vector<int>> &bombXY)
{
    game_data.reset();
    error_type.reset();
    bombXY.clear();
    for (int i = 0; i < 4; i++)
    {
        players[i].points = 0;
        players[i].is_alive = false;
    }
}

// Función para disparar con pistola
void pistol_shot(int &turn, bool &lose)
{
    if (players[turn].bullets == 0)
    {
        cout << players[turn].name << " no tiene balas, no puede disparar\n";
        return;
    }
    else if (lose)
    {
        cout << players[turn].name << " a hecho explotar una bomba! +10 puntos\n";
    }
    else
    {
        cout << players[turn].name << " a desperdiciado una bala!, no gana puntos\n";
    }

    players[turn].bullets--;
    lose = false;
}

// Función para protegerse con escudo
void shield_protection(int &turn, bool &lose)
{
    if (players[turn].shields == 0)
    {
        cout << players[turn].name << " no tiene escudos, no puede protegerse\n";
        return;
    }
    else if (lose)
    {
        cout << players[turn].name << " se ha protegido de una bomba! +10 puntos\n";
        players[turn].shields--;
        lose = false;
    }
    else
    {
        cout << players[turn].name << " a gastado un escudo, no gana puntos\n";
        players[turn].shields--;
        lose = false;
    }
}

void player_action(int &turn, bool &lose)
{
    int action;
    cout << "¿Que desea hacer? (1=Excavar, 2=Protegerse, 3=Disparar con pistola): ";
    cin >> action;
    switch (action)
    {
    case 1: // Excavar
        cout << "Usted ha excavado... \n";
        break;
    case 2: // Protegerse
        shield_protection(turn, lose);
        break;
    case 3: // Disparar con pistola
        pistol_shot(turn, lose);
        break;
    default:
        cout << "Acción inválida, intente nuevamente\n";
        break;
    }
}