#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

bool gameOver;
const int width = 40;
const int height = 20;
int x, y, targetX, targetY, score;
vector<pair<int, int>> enemies;

void Setup() {
    gameOver = false;
    x = width / 2;
    y = height - 1;
    score = 0;
    srand(time(0));
    targetX = rand() % width;
    targetY = 0;
    
    // Crear enemigos iniciales
    for (int i = 0; i < 5; i++) {
        enemies.push_back({rand() % width, rand() % (height / 2)});
    }
}

void Draw() {
    system("cls");
    
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
                
            if (i == y && j == x)
                cout << "A"; // Nave del jugador
            else if (i == targetY && j == targetX)
                cout << "*"; // Disparo
            else {
                bool enemyHere = false;
                for (auto enemy : enemies) {
                    if (enemy.first == j && enemy.second == i) {
                        cout << "V"; // Enemigos
                        enemyHere = true;
                        break;
                    }
                }
                if (!enemyHere)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            x--;
            break;
        case 'd':
            x++;
            break;
        case ' ':
            targetX = x;
            targetY = y - 1;
            break;
        case 'q':
            gameOver = true;
            break;
        }
    }
}

void Logic() {
    // Mover disparo
    if (targetY > 0)
        targetY--;

    // Mover enemigos
    for (auto &enemy : enemies) {
        enemy.second++;
        if (enemy.second >= height) {
            enemy.second = 0;
            enemy.first = rand() % width;
        }
    }

    // Detectar colisión disparo-enemigo
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->first == targetX && it->second == targetY) {
            it = enemies.erase(it);
            score += 10;
            targetY = -1; // Resetear disparo
            enemies.push_back({rand() % width, 0}); // Nuevo enemigo
        }
        else {
            ++it;
        }
    }

    // Detectar colisión nave-enemigo
    for (auto enemy : enemies) {
        if (enemy.first == x && enemy.second == y) {
            gameOver = true;
        }
    }

    // Limitar movimiento dentro de pantalla
    if (x >= width) x = width - 1;
    if (x < 0) x = 0;
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100);
    }
    
    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}