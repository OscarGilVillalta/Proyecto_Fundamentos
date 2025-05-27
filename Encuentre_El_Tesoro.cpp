#include <iostream>
#include <vector>
using namespace std;

int encuentraTesoro()
{
    srand(time(NULL));

    int bomba = 1;
    int azar = 0;
    int inicioBombaColumna = 0;
    vector<int> posicion_X = {};
    vector<int> posicion_Y = {};

    vector<int> bomba_X_Y = {};
    vector<int> cantindadDeBombas = {};

    for (int i = 0; i < 5; i++) // Sistema de crear bombas aleatorias
    {
        bomba = 1;
        for (int j = 0; j < 5; j++)
        {
            int azar = rand() % 3; // Crea un numero al azar entre 0-2
            if (bomba < 7)
            {
                if (azar == 0) // Posicion de la bomba + 1
                {
                    bomba += 1;
                }
                else if (azar == 1) // Posicion de la bomba + 2
                {
                    bomba += 2;
                }
                else
                { // Posicion de la bomba + 3
                    bomba += 3;
                }
                bomba_X_Y.push_back(bomba); // Guarda la posicion de la bomba
            }
            else
            {
                cantindadDeBombas.push_back(j); // Guarda la cantidad de bombas que hay en una fila
                break;
            }
        }

        bomba_X_Y.push_back(0); // El "0" indicara el fin de la fila
    }

    bool tesoro = false;
    bool perder = false;
    bool mina = false;

    while (true)
    {

        if (mina == true)
        {
            cout << "Oh no!, usted a perdido porque a pisado una mina" << endl;
            break;
        }

        int abscisa = 0;
        int ordenada = 0;
        cout << "Ingrese una posicion de una fila (Eje X) y otra de una columna (Eje Y) \n Abscisa: ";
        cin >> abscisa;
        cout << " Ordenada: ";
        cin >> ordenada;

        posicion_X.push_back(abscisa);
        posicion_Y.push_back(ordenada);

        for (int p = 0; p < posicion_X.size() - 1; p++)
        { // Detecta si hay una coordenada repetida
            if (posicion_Y[p] == ordenada && posicion_X[p] == abscisa)
            {
                cout << "No se puede ingresar la misma coordenada por lo que usted pierde";
                perder = true;
            }
        }

        if (perder)
        { // Se activa si cometio algun error o perdio
            break;
        }

        int k = 0;                  // Definir el valor de "K"
        cout << " ";                // Estetica
        for (int i = 1; i < 6; i++) // Columna o eje "Y"
        {
            if (i == 1)
            {
                for (int m = 1; m < 11; m++) // Genera un espacio para asignar numero a las columnas
                {
                    cout << "  " << m;
                }
                cout << endl;
            }
            cout << i << " ";            // Genera un espacio para asignar numero a las filas
            for (int j = 1; j < 11; j++) // Columna o eje "X"
            {
                for (k; k < cantindadDeBombas.size(); k++) //Recorre la cantidad de bombas que hay en una fila
                {
                    if (cantindadDeBombas[k] == 0) //Cuando el arreglo llegue a un valor 0 se considera el final de una fila
                    {
                        break;
                    }
                    else if (cantindadDeBombas[k] == abscisa)
                    {
                        mina = true;
                    }
                }

                for (k = 0; k < posicion_X.size(); k++) // Se verifica la posicion del tesoro
                {
                    if (posicion_Y[k] == i && posicion_X[k] == j) //Se imrpime el lugar donde se va a poner el tesoro
                    {
                        tesoro = true;
                        break;
                    }
                }
                if (tesoro && !mina) //Si ambos son verdaderos se activa
                {
                    cout << " $ ";
                    tesoro = false;
                }
                else if (mina) //Si la mina se activa todas explotan
                {
                    cout << " ! ";
                }
                else //Imprime los lugares no explorados
                {
                    cout << " # ";
                }
            }

            if (i > 1)
            {
                k = cantindadDeBombas[i - 1] + cantindadDeBombas[i]; //Genera el proximo valor que tiene que tener "k" para el proximo ciclo
            }

            cout << endl;
        }
    }

    return 0;
}

int main()
{
    encuentraTesoro();

    return 0;
}