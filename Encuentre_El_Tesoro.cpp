#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

int encuentraTesoro()
{
    srand(time(NULL));

    int retirarse = 0;
    int puntos = 0; // Almacena los puntos que ganas
    int perder = -1;
    int bomba_X = 0;
    int bomba_Y = 0;
    vector<vector<int>> posicion_X_Y = {};

    vector<vector<int>> posicion_bomba_X_Y = {}; // Arreglo para almacenar las posiciones de las bombas
    vector<int> posicion_Bomba_X = {};           // Arreglo para almacenar bombas en "X"

    bool repetido;
    for (int columna_bomba = 1; columna_bomba <= 5; columna_bomba++) // Un bucle de columnas
    {
        bomba_Y = columna_bomba;
        for (int fila_bomba = 1; fila_bomba <= 5; fila_bomba++) // Un bucle de filas
        {
            do // Verifica que la coordenada en "X" no se repita
            {
                repetido = false;
                bomba_X = (rand() % 10) + 1;

                for (int repetido_X : posicion_Bomba_X) // Se recorre el arreglo y si encuentra una posicion igual se rompe
                {
                    if (repetido_X == bomba_X)
                    {
                        repetido = true;
                        break;
                    }
                }
            } while (repetido);

            posicion_Bomba_X.push_back(bomba_X); // Se guardan los elmentos en otro arreglo
        }

        for (int coordenada_X_Bomba : posicion_Bomba_X)
        {
            posicion_bomba_X_Y.push_back({coordenada_X_Bomba, columna_bomba}); // Se guardan todas las coordenadas en fomarto (X,Y)
        }
        posicion_Bomba_X.clear(); // Se limpia el arreglo para volverlo a usar
    }

    bool tesoro = false;
    bool jugando = true;
    while (jugando)
    {
        if (retirarse == 3)
        {
            cout << "A conseguido al menos 3 tesoros, por lo que es libre de irse si usted desea! (Solo 1 vez aparecera este mensaje)" << endl
                 << "Escriba 3 si desea retirarse";
            cin >> perder;
        }

        int abscisa = 0;
        int ordenada = 0;
        cout << "Ingrese una posicion de una fila (Eje X) y otra de una columna (Eje Y) \n Abscisa: ";
        cin >> abscisa;
        cout << " Ordenada: ";
        cin >> ordenada;

        if ((abscisa >= 1 && abscisa <= 10) && (ordenada >= 1 && ordenada <= 5)) // Verifica que los valores esten dentro del rango o pierde
        {
            posicion_X_Y.push_back({abscisa, ordenada});
        }
        else
        {
            perder = 0;
        }

        for (int p = 0; p < posicion_X_Y.size() - 1; p++) // Detecta si hay una coordenada repetida
        {
            if (posicion_X_Y[p][0] == abscisa && posicion_X_Y[p][1] == ordenada)
            {
                perder = 1;
            }
        }

        cout << " ";                                  // Estetica
        for (int columna = 1; columna < 6; columna++) // Columna o eje "Y"
        {
            if (columna == 1)
            {
                for (int m = 1; m < 11; m++) // Genera un espacio para asignar numero a las columnas
                {
                    cout << "  " << m;
                }
                cout << endl;
            }
            cout << columna << " ";               // Genera un espacio para asignar numero a las filas
            for (int fila = 1; fila < 11; fila++) // fila o eje "X"
            {
                for (int coordenadas_bomba = 0; coordenadas_bomba < posicion_bomba_X_Y.size(); coordenadas_bomba++) // Se recorre el arreglo de las bombas
                {
                    if (posicion_bomba_X_Y[coordenadas_bomba][0] == abscisa && posicion_bomba_X_Y[coordenadas_bomba][1] == ordenada) // Si hay una coordenad igual pierde
                    {
                        perder = 2;
                    }
                }

                if (perder != 2)
                {
                    for (int posicion_Tesoro = 0; posicion_Tesoro < posicion_X_Y.size(); posicion_Tesoro++) // Se verifica la posicion del tesoro
                    {
                        if (posicion_X_Y[posicion_Tesoro][0] == columna && posicion_X_Y[posicion_Tesoro][1] == fila) // Se imrpime el lugar donde se va a poner el tesoro
                        {
                            tesoro = true;
                            break;
                        }
                    }
                }
                if (tesoro) // Si ambos son verdaderos se activa
                {
                    cout << " $ ";
                    tesoro = false;
                }
                else if (perder == 2)
                {
                    cout << " ! ";
                }
                else // Imprime los lugares no explorados
                {
                    cout << " # ";
                }
            }
            cout << endl;
        }
        switch (perder) // Menu donde se ponen los diferentes casos donde el jugador puede perder
        {
        case 0: // Poner valores fuera de los limites
            cout << "Usted perdio por no haber ingresado un valor dentro de los limites establecidos";
            jugando = false;
            break;
        case 1: // Repetir coordenadas
            cout << "Usted perdio por haber repetido coordenadas lo cual no es posible";
            jugando = false;
            break;
        case 2: // Pisar una bomba
            cout << "Oh no!, usted a pisado una bomba por lo cual a perdido";
            jugando = false;
            break;
        case 3: // Retirarse
            cout << "Ustes a decidido retirarse, pero se llevar sus tesorosn consigo!";
            jugando = false;
            break;
        default: // Gana puntos
            puntos += 10;
            break;
        }
    }

    return puntos;
}

int main()
{
    int puntos = 0;
    puntos = encuentraTesoro();

    return 0;
}