#include <iostream>        // Entrada y salida estándar
#include <fstream>         // Manejo de archivos
#include <vector>          // Uso de vectores dinámicos
using namespace std;       // Para evitar usar std::

// Estructura para dificultad
struct Dificulty {
    int maxRows;           // Filas del tablero
    int maxColumns;        // Columnas del tablero
    int maxRowsBombs;      // Cantidad de filas con bombas
    int maxColumnsBombs;   // Cantidad de columnas con bombas
} dificulty;               // Variable global de tipo Dificulty

// Función para cargar la partida
void cargarPartida(vector<vector<int>>& bombXY, vector<vector<int>>& repeat) {
    ifstream archivo("partida.txt");  // Abre el archivo de guardado
    if (!archivo) {                    // Verifica si el archivo no existe
        cout << "No se pudo cargar la partida.\n";  // Mensaje de error
        return;                        // Sale de la función
    }

    // Lee la dificultad desde el archivo
    archivo >> dificulty.maxRows >> dificulty.maxColumns
            >> dificulty.maxRowsBombs >> dificulty.maxColumnsBombs;

