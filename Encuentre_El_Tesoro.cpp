#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

//Crea selectores de dificultas
struct Dificulty
{
    int maxRows;
    int maxColumns;
    int maxRowsBombs;
    int maxColumnsBombs;

    void reset()
    { //Regresaa a valores prederteminados
        maxRows = 0;
        maxColumns = 0;
        maxRowsBombs = 0;
        maxColumnsBombs = 0;
    }
} dificulty;

//Verifica que el juego aun se siga ejecutando
struct GameStatus
{
    bool bombExplote = false;
    bool repeatCoordinate = false;
    bool outOfRange = false;
    bool dataTypeInvalid = false;

    void reset()
    {//Regresaa a valores prederteminados
        bombExplote = false;
        repeatCoordinate = false;
        outOfRange = false;
        dataTypeInvalid = false;
    };
} gameOver;

//Imprime sprites
struct Sprite{
    string title = "Title";
    string bomb = "Bomb";
    string treasure = "Treasure";
    string menu = "Menu";
    string finish = "Game Over";
};

void dificultad(int dif);
vector<vector<int>> randomCoordinates(int maxRowsBombs, int maxColumnsBombs);
bool proveCoordinates(vector<int> coordinate, vector<vector<int>> bombXY, vector<vector<int>> repeat);
void gameOverMessage();
string sprite(string typeSprite);
int board();

int main()
{
    srand(time(NULL));
    dificultad(2);
    return 0;
}

//Funcion para imprimir "Sprites"
string sprite(string typeStrite){
    if(typeStrite == "Title"){
        cout << " __                                                                      __ \n"
        "( _ )--------------------------------------------------------------------( _ )\n"
        " |   |                                                                      |   | \n"
        " |   |                                                                      |   | \n"
        " |   |   ######  ######  ##  ##  #####           ######  ##  ##  ######     |   | \n"
        " |   |   ##        ##    ### ##  ##  ##            ##    ##  ##  ##         |   | \n"
        " |   |   ####      ##    ## ###  ##  ##            ##    ######  ####       |   | \n"
        " |   |   ##        ##    ##  ##  ##  ##            ##    ##  ##  ##         |   | \n"
        " |   |   ##      ######  ##  ##  #####             ##    ##  ##  ######     |   | \n"
        " |   |   ................................................................   |   | \n"
        " |   |   ######  #####   ######   ####    ####    ##  ##  #####   ######    |   | \n"
        " |   |   ##      ##  ##  ##      ##  ##  ##       ##  ##  ##  ##  ##        |   | \n"
        " |   |   ##      #####   ####    ######   ####    ##  ##  #####   ####      |   | \n"
        " |   |   ##      ##  ##  ##      ##  ##      ##   ##  ##  ##  ##  ##        |   | \n"
        " |   |   ##      ##  ##  ######  ##  ##   ####     ####   ##  ##  ######    |   | \n"
        " |   |   ................................................................   |   | \n"
        " |   |                                                                      |   | \n"
        " |_|                                                                      |_| \n"
        "(__)--------------------------------------------------------------------(__)";
    }
}

//Selecto de dificultado
void dificultad(int dif)
{
    switch (dif)
    {
    case 1://Facil
        dificulty.maxRows = 10;
        dificulty.maxColumns = 10;
        dificulty.maxRowsBombs = 3;
        dificulty.maxColumnsBombs = 10;
        break;
    case 2://Medio
        dificulty.maxRows = 20;
        dificulty.maxColumns = 20;
        dificulty.maxRowsBombs = 10;
        dificulty.maxColumnsBombs = 20;
        break;
    case 3://Complicado
        dificulty.maxRows = 30;
        dificulty.maxColumns = 30;
        dificulty.maxRowsBombs = 16;
        dificulty.maxColumnsBombs = 30;
        break;
    default://Aqui tendria que ir el multijugador creo 
        break;
    }

    board();
}

//Gener coordenadas aleatorias
vector<vector<int>> randomCoordinates(int maxRowsBombs, int maxColumnsBombs)
{
    vector<vector<int>> bombXY = {}; //Almacena la posicion de las bombas
    vector<vector<int>>::iterator repeat; //Un iterador para detectar repeticiones
    int bombX = 0, bombY = 0, row = 1, column = 1;

    for (column; column <= maxColumnsBombs; column++)
    {
        vector<int> coordinate;
        bombY = column;
        row = 0;
        do
        {
            bombX = rand() % maxRowsBombs + 1; 
            repeat = find(bombXY.begin(), bombXY.end(), coordinate);
            if (repeat == bombXY.end())
            {
                bombXY.push_back({bombX, bombY});
                row++;
            }
        } while (row < maxRowsBombs);
    }

    for (int i = 0; i <= 10; i++)
    {
        cout << bombXY[i][0] << " " << bombXY[i][1] << "\n";
    }
    return bombXY;
}

bool proveCoordinates(vector<int> coordinate, vector<vector<int>> bombXY, vector<vector<int>> repeat)
{

    if ((coordinate[0] < 0 || coordinate[0] > dificulty.maxRows) ||
        (coordinate[1] < 0 || coordinate[1] > dificulty.maxColumns))
    {
        gameOver.outOfRange = true;
        return true;
    }

    if (find(repeat.begin(), repeat.end(), coordinate) != repeat.end())
    {
        gameOver.repeatCoordinate = true;
        return true;
    }

    if (find(bombXY.begin(), bombXY.end(), coordinate) != bombXY.end())
    {
        gameOver.bombExplote = true;
        return true;
    }

    return false;
}

void gameOverMessage()
{
    if (gameOver.bombExplote)
    {
        cout << "Usted a pisado una bomba\n";
    }
    else if (gameOver.repeatCoordinate)
    {
        cout << "Usted a repetido una coordenada lo cual no es valido\n";
    }
    else if (gameOver.outOfRange)
    {
        cout << "Usted a ingresado una coordenada fuera del rango establecido\n";
    }
    else if (gameOver.dataTypeInvalid)
    {
        cout << "A ingresado un dato invalido, debe ingresaar un numero entero en el rango establecido\n";
    }
}

int board()
{
    bool lose = false, bomb;
    int positionX = 0, positionY = 0;
    vector<vector<int>> bombXY = randomCoordinates(dificulty.maxRowsBombs, dificulty.maxColumnsBombs);
    vector<int> coordinate;
    vector<vector<int>> repeat = {};
    vector<vector<int>> bombPosition = {};

    while (true)
    {
        gameOver.reset();

        cout << "Ingrese sus coordenadas\n"
             << "Posicion en X: ";
        cin >> positionX;
        cout << "Posicion en Y: ";
        cin >> positionY;
        coordinate = {positionX, positionY};

        lose = proveCoordinates(coordinate, bombXY, repeat);

        if (lose)
        {
            if(gameOver.bombExplote){
                bombPosition.push_back(coordinate);
            }
            gameOverMessage();
            lose = false;
        }

        for (int column = 1; column < dificulty.maxColumns; column++)
        {
            for (int row = 1; row < dificulty.maxRows; row++)
            {
                for(vector <int> bomba : bombPosition){
                    if(bomba[0] == row && bomba[1] == column){
                        bomb = true;
                    }
                }

                if(bomb){
                    cout << " ! ";
                    bomb = false;
                }else{
                    cout << " # ";
                }
            }
            cout << endl;
        }

        repeat.push_back(coordinate);
    }

    return 0;
}