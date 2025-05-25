#include <iostream>
#include <vector>
#include <random>
using namespace std;

int encuentraTesoro()
{
    vector<int> posicion_X = {};
    vector<int> posicion_Y = {};
    bool tesoro = false;
    bool perder = false;

    while (true)
    {
        int abscisa = 0;
        int ordenada = 0;
        cout << "Ingrese una posicion de una fila (Eje X) y otra de una columna (Eje Y) \n Abscisa: ";
        cin >> abscisa;
        cout << " Ordenada: ";
        cin >> ordenada;

        posicion_X.push_back(abscisa);
        posicion_Y.push_back(ordenada);

        for(int p = 0; p < posicion_X.size()-1; p++){ //Detecta si hay una coordenada repetida
            if(posicion_Y[p] == ordenada && posicion_X[p] == abscisa){
                cout << "No se puede ingresar la misma coordenada por lo que usted pierde";
                perder = true;
            }
        }

        if(perder){ //Se activa si cometio algun error o perdio
            break;
        }

        cout << " "; //Estetica
        for (int i = 1; i < 6; i++) // Columna o eje "Y"
        {
            if(i == 1){
                    for(int m = 1; m < 10; m++){
                        cout << "  " << m;
                    }
                    cout << endl;
                }
            cout << i << " ";
            for (int j = 1; j < 10; j++) // Columna o eje "X"
            {  
                for(int k = 0; k < posicion_X.size(); k++){ //Se imprime en donde esta el tesoro
                    if(posicion_Y[k] == i && posicion_X[k] == j){
                        tesoro = true;
                    }
                }
                if(tesoro){
                    cout << " $ ";
                    tesoro = false;
                }else{
                    cout << " # ";
                }
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