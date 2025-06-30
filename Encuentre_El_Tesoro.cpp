#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

// Crea selectores de dificultas
struct Difficulty
{
    int maxRows;
    int maxColumns;
    int maxRowsBombs;

    void reset()
    { // Regresaa a valores prederteminados
        maxRows = 0;
        maxColumns = 0;
        maxRowsBombs = 0;
    }
} dificulty;

// Verifica que el juego aun se siga ejecutando
struct GameStatus
{
    bool bombExplote;
    bool repeatCoordinate;
    bool outOfRange;
    bool dataTypeInvalid;

    void reset()
    { // Regresaa a valores prederteminados
        bombExplote = false;
        repeatCoordinate = false;
        outOfRange = false;
        dataTypeInvalid = false;
    };
} gameOver;

// Imprime sprites
struct Sprite
{
    string title = "Title";
    string bomb = "Bomb";
    string treasure = "Treasure";
    string menu = "Menu";
    string finish = "Game Over";
};

// Seleccionar dificultad
void menu_difficulty(int dif);
// Generar coordenadas aleatorias
vector<vector<int>> random_coordinates();
// Verificar coordenadas
bool prove_coordinates(const vector<int> &coordinate, const vector<vector<int>> &bombXY, const vector<vector<int>> &repeat);
// Mensaje de en caso de perder
void game_over_message();
// Imprimir sprites
string sprite(string typeSprite);
// Mensaje de victoria
bool victory(int points);
// Tablero de juego
int board();

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
        dificulty.maxRowsBombs = 3;
        break;
    case 2: // Medio
        dificulty.maxRows = 20;
        dificulty.maxColumns = 20;
        dificulty.maxRowsBombs = 10;
        break;
    case 3: // Complicado
        dificulty.maxRows = 30;
        dificulty.maxColumns = 30;
        dificulty.maxRowsBombs = 16;
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
    int bombX = 0, bombY = 0, row = 0, column = 1;

    // Genera las coordenadas de las bombas
    for (column; column <= dificulty.maxColumns; column++)
    {
        row = 0;
        bombY = column;
        do
        {
            // Genera una coordenada aleatoria
            bombX = (rand() % dificulty.maxRows) + 1;
            // Verifica si la coordenada ya existe
            if (find(bombXY.begin(), bombXY.end(), vector<int>{bombX, bombY}) == bombXY.end())
            {
                // Agrega la coordenada a la lista de bombas
                bombXY.push_back({bombX, bombY});
                row++;
            }
        } while (row < dificulty.maxRowsBombs); // Verifica si se han generado todas las bombas
    }

    for (int i = 0; i < bombXY.size(); i++)
    {
        cout << bombXY[i][0] << " " << bombXY[i][1] << endl;
    }
    return bombXY;
}

// Verifica si la coordenada es valida
bool prove_coordinates(const vector<int> &coordinate, const vector<vector<int>> &bombXY, const vector<vector<int>> &repeat)
{
    // Si la coordenada esta fuera del rango
    if ((coordinate[0] <= 0 || coordinate[0] > dificulty.maxRows) ||
        (coordinate[1] <= 0 || coordinate[1] > dificulty.maxColumns))
    {
        gameOver.outOfRange = true;
        return true;
    }

    // Si la coordenada esta repetida
    if (find(repeat.begin(), repeat.end(), coordinate) != repeat.end())
    {
        gameOver.repeatCoordinate = true;
        return true;
    }

    // Si la coordenada es una bomba
    if (find(bombXY.begin(), bombXY.end(), coordinate) != bombXY.end())
    {
        gameOver.bombExplote = true;
        return true;
    }

    return false;
}

void game_over_message()
{
    // Si la coordenada es una bomba
    if (gameOver.bombExplote)
    {
        cout << "Usted a pisado una bomba\n";
    }
    // Si la coordenada es repetida
    else if (gameOver.repeatCoordinate)
    {
        cout << "Usted a repetido una coordenada lo cual no es valido\n";
    }
    // Si la coordenada esta fuera del rango
    else if (gameOver.outOfRange)
    {
        cout << "Usted a ingresado una coordenada fuera del rango establecido\n";
    }
    // Si el dato ingresado no es un numero entero
    else if (gameOver.dataTypeInvalid)
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

int board()
{
    int retire = 0;
    bool lose = false, bomb_coordinate = false, treasure_coordinate = false;
    int positionX = 0, positionY = 0, points = 0;
    vector<vector<int>> bombXY = random_coordinates();
    vector<int> coordinate;                // Coordenadas del usuario
    vector<vector<int>> treasureXY = {};   // Coordenadas del tesoro encontrado
    vector<vector<int>> repeat = {};       // Coordenadas repetidas
    vector<vector<int>> bombPosition = {}; // Coordenadas de las bombas que el usuario ha pisado

    while (true)
    {
        // Reinicia los valores de gameOver
        gameOver.reset();

        // Solicita las coordenadas al usuario
        cout << "Ingrese sus coordenadas\n"
             << "Posicion en X: ";
        cin >> positionX;
        cout << "Posicion en Y: ";
        cin >> positionY;
        coordinate = {positionX, positionY};

        // Verifica si la coordenada es valida
        lose = prove_coordinates(coordinate, bombXY, repeat);

        // Si la coordenada es invalida
        if (lose)
        {
            // Si la coordenada es una bomba
            if (gameOver.bombExplote)
            {
                bombPosition.push_back(coordinate);
            }
            // Muestra el mensaje de game over
            game_over_message();
            lose = false;
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

        // Agrega la coordenada a la lista de coordenadas repetidas
        repeat.push_back(coordinate);
        treasureXY.push_back(coordinate);
        points += 20;

        // Imprime el tablero de juego
        for (int row = 1; row <= dificulty.maxRows; row++)
        {
            for (int column = 1; column <= dificulty.maxColumns; column++)
            {
                // Reinicia las banderas para cada posición
                bomb_coordinate = false;
                treasure_coordinate = false;

                // Verifica si la coordenada es una bomba
                for (vector<int> bomb : bombPosition)
                {
                    if (bomb[0] == column && bomb[1] == row)
                    {
                        bomb_coordinate = true;
                        break;
                    }
                }

                // Verifica si la coordenada es un tesoro
                for (vector<int> treasure : treasureXY)
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
    }

    return 0;
}