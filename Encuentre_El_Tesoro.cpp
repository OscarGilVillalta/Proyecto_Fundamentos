#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

// Crea selectores de dificultas
struct Difficulty
{
    int maxRows; // Numero de filas
    int maxColumns; // Numero de columnas
    int maxBombs; // Numero de bombas

    void reset()
    { // Regresaa a valores prederteminados
        maxRows = 0;
        maxColumns = 0;
        maxBombs = 0;
    }
} dificulty;

struct Players
{
    string nombre; // Nombre del jugador
    int points; // Puntos del jugador
} players[4];

// Verifica que el juego aun se siga ejecutando
struct GameStatus
{
    bool bombExplote; // Verifica si la coordenada es una bomba
    bool repeatCoordinate; // Verifica si la coordenada es repetida
    bool outOfRange; // Verifica si la coordenada esta fuera del rango
    bool dataTypeInvalid; // Verifica si el dato ingresado no es un numero entero
    bool multiplayer; // Verifica si el juego es multijugador
    vector<vector<int>> bomb_explote; // Almacena las coordenadas de las bombas
    vector<vector<int>> repeat; // Almacena las coordenadas repetidas
    vector<vector<int>> treasureXY; // Almacena las coordenadas de los tesoros

    void reset()
    { // Regresaa a valores prederteminados
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

// Imprime sprites
struct Sprite
{
    string title = "Title";
    string bomb = "Bomb";
    string treasure = "Treasure";
    string menu = "Menu";
    string finish = "Game Over";
};

// Imprimir tablero
void print_board();
// Control de juego
int game_controls();
// Seleccionar dificultad
void menu_difficulty(int dif);
// Generar coordenadas aleatorias
vector<vector<int>> random_coordinates();
// Verificar coordenadas
bool prove_coordinates(vector<int> &coordinate, const vector<vector<int>> &bombXY);
// Mensaje de en caso de perder
void game_over_message();
// Imprimir sprites
string sprite(string typeSprite);
// Mensaje de victoria
bool victory(int points);

int main()
{
    srand(time(NULL));
    sprite("Title");
    menu_difficulty(2);
    return 0;
}

// Funcion para imprimir "Sprites"
string sprite(string typeStrite)
{
    if (typeStrite == "Title") // Titulo del juego
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

// Selecto de menu_difficultyo
void menu_difficulty(int dif)
{
    switch (dif)
    {
    case 1: // Facil
        dificulty.maxRows = 10;
        dificulty.maxColumns = 10;
        dificulty.maxBombs = 3;
        break;
    case 2: // Medio
        dificulty.maxRows = 20;
        dificulty.maxColumns = 20;
        dificulty.maxBombs = 10;
        break;
    case 3: // Complicado
        dificulty.maxRows = 30;
        dificulty.maxColumns = 30;
        dificulty.maxBombs = 16;
        break;
    default: // Aqui tendria que ir el multijugador creo
        break;
    }

    board();
}

// Gener coordenadas aleatorias
vector<vector<int>> random_coordinates()
{
    vector<vector<int>> bombXY = {}; // Almacena la posicion de las bombas
    int bombX = 0, bombY = 0;

    int bombsTotal = dificulty.maxBombs * dificulty.maxColumns;
    while (bombXY.size() < bombsTotal)
    {
        int bombX = rand() % dificulty.maxRows + 1;
        int bombY = rand() % dificulty.maxColumns + 1;
        vector<int> candidate = {bombX, bombY};
        if (find(bombXY.begin(), bombXY.end(), candidate) == bombXY.end())
        {
            bombXY.push_back(candidate);
        }
    }

    return bombXY;
}

// Verifica si la coordenada es valida
bool prove_coordinates(const vector<int> &coordinate, const vector<vector<int>> &bombXY)
{
    game_status.repeat.push_back(coordinate);
    // Si la coordenada esta fuera del rango
    if ((coordinate[0] <= 0 || coordinate[0] > dificulty.maxRows) ||
        (coordinate[1] <= 0 || coordinate[1] > dificulty.maxColumns))
    {
        game_status.outOfRange = true;
        return true;
    }

    // Si la coordenada esta repetida
    if (find(game_status.repeat.begin(), game_status.repeat.end(), coordinate) != game_status.repeat.end())
    {
        game_status.repeatCoordinate = true;
        return true;
    }

    // Si la coordenada es una bomba
    if (find(bombXY.begin(), bombXY.end(), coordinate) != bombXY.end())
    {
        game_status.bomb_explote.push_back(coordinate);
        game_status.bombExplote = true;
        return true;
    }

    game_status.treasureXY.push_back(coordinate);
    return false;
}

void game_over_message()
{
    // Si la coordenada es una bomba
    if (game_status.bombExplote)
    {
        cout << "Usted a pisado una bomba\n";
    }
    // Si la coordenada es repetida
    else if (game_status.repeatCoordinate)
    {
        cout << "Usted a repetido una coordenada lo cual no es valido\n";
    }
    // Si la coordenada esta fuera del rango
    else if (game_status.outOfRange)
    {
        cout << "Usted a ingresado una coordenada fuera del rango establecido\n";
    }
    // Si el dato ingresado no es un numero entero
    else if (game_status.dataTypeInvalid)
    {
        cout << "A ingresado un dato invalido, debe ingresaar un numero entero en el rango establecido\n";
    }
}

// Mensaje de victoria
bool victory(int points)
{
    cout << "Usted a sobrevivido!\n"
         << "Puntos: " << points << "\n";

    return true;
}

int multiplayer(int &maxPlayers, int &attempt)
{

    return 0;
}

void print_board()
{
    bool bomb_coordinate = true, treasure_coordinate = true;
    int points = 0;
    for (int row = 1; row <= dificulty.maxRows; row++) // Recorre las filas
    {
        for (int column = 1; column <= dificulty.maxColumns; column++) // Recorre las columnas
        {
            // Reinicia los valores para cada posición
            bomb_coordinate = false;
            treasure_coordinate = false;

            // Verifica si la coordenada es una bomba
            for (vector<int> bomb : game_status.bomb_explote)
            {
                if (bomb[0] == column && bomb[1] == row)
                {
                    bomb_coordinate = true;
                    break;
                }
            }

            // Verifica si la coordenada es un tesoro
            for (vector<int> treasure : game_status.treasureXY)
            {
                if (treasure[0] == column && treasure[1] == row)
                {
                    treasure_coordinate = true;
                    break;
                }
            }

            // Si la coordenada es una bomba
            if (bomb_coordinate)
            {
                cout << " ! ";
            }
            else if (treasure_coordinate)
            {
                cout << " $ ";
            }
            // Si la coordenada no es una bomba
            else
            {
                cout << " # ";
            }
        }
        cout << endl;
    }
    points += 20;
}

int game_controls()
{
    
    int retire = 0;
    bool lose = false, bomb_coordinate = false, treasure_coordinate = false;
    int positionX = 0, positionY = 0, points = 0;
    vector<vector<int>> bombXY = random_coordinates(); // Genera las coordenadas de las bombas
    vector<int> coordinate; // Coordenadas del usuario

    while (true)
    {
        // Solicita las coordenadas al usuario
        cout << "Ingrese sus coordenadas\n"
             << "Posicion en X: ";
        cin >> positionX;
        cout << "Posicion en Y: ";
        cin >> positionY;
        coordinate = {positionX, positionY};

        // Verifica si la coordenada es valida
        lose = prove_coordinates(coordinate, bombXY);
        // Si la coordenada es invalida
        if (lose)
        {
            // Muestra el mensaje de game over
            game_over_message();
            // Sale del bucle
            break;
        }
        // Si el usuario ha llegado a 60 puntos
        else if (points >= 60)
        {
            // Pregunta si desea retirarse
            cout << "Desea retirarse? (s/n): ";
            cin >> retire;
            if (retire == 1)
            {
                victory(points);
                break;
            }
        }

        print_board();
    }

    return 0;
}