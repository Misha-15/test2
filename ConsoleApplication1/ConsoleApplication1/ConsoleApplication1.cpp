#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <ctime>
using namespace std;

//функция для инициализации динамического массива
int** CreateArray(int size) {
    int** arr = new int* [size];
    for (int i = 0; i < size; i++) {
        arr[i] = new int[2]; //две координаты: x и y
    }
    return arr;
}

//функция для удаления динамического массива
void DeleteArray(int** arr, int size) {
    for (int i = 0; i < size; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

//настройки начального состояния игры
void Setup(int& x, int& y, int& fruitX, int& fruitY, int& score, int& nTail, int**& tail, int& obstacleCount, int**& obstacles, int width, int height) {
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    tail = CreateArray(width * height);  //динамический массив для хвоста
    obstacleCount = 10;
    obstacles = CreateArray(obstacleCount); //динамический массив для препятствий

    //генерация препятствий
    for (int i = 0; i < obstacleCount; i++) {
        obstacles[i][0] = rand() % width;
        obstacles[i][1] = rand() % height;
    }
}

//функция для рисования игрового поля
void Draw(int x, int y, int fruitX, int fruitY, int score, int nTail, int** tail, int obstacleCount, int** obstacles, int width, int height) {
    system("cls");

    //верхняя граница
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    //поле игры
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#"; //левая граница

            if (i == y && j == x)
                cout << "O"; //голова змейки
            else if (i == fruitY && j == fruitX)
                cout << "F"; //фрукт
            else {
                bool print = false;

                //проверка на хвост змейки
                for (int k = 0; k < nTail; k++) {
                    if (tail[k][0] == j && tail[k][1] == i) {
                        cout << "o";
                        print = true;
                    }
                }

                //проверка на препятствия
                for (int k = 0; k < obstacleCount; k++) {
                    if (obstacles[k][0] == j && obstacles[k][1] == i) {
                        cout << "X";
                        print = true;
                    }
                }

                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#"; //правая граница
        }
        cout << "\n";
    }

    //нижняя граница
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << "\n";

    cout << "score: " << score << "\n";
}

//функция для управления движением змейки
void Input(eDirection& dir) {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) dir = UP;
            break;
        case 's':
            if (dir != UP) dir = DOWN;
            break;
        case 'x':
            dir = STOP; //прекращение игры
            break;
        }
    }
}

//функция для логики игры
void Logic(int& x, int& y, int& fruitX, int& fruitY, int& score, int& nTail, int** tail, eDirection& dir, int obstacleCount, int** obstacles, int width, int height, int& speed) {
    int prevX = x, prevY = y;  //запоминаем предыдущие координаты
    int prev2X, prev2Y;

    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    //столкновение с границами поля
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    //столкновение с хвостом
    for (int i = 0; i < nTail; i++)
        if (tail[i][0] == x && tail[i][1] == y)
            dir = STOP;

    //перемещение хвоста
    for (int i = 0; i < nTail; i++) {
        prev2X = tail[i][0];
        prev2Y = tail[i][1];
        tail[i][0] = prevX;
        tail[i][1] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    //столкновение с препятствиями
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i][0] == x && obstacles[i][1] == y)
            dir = STOP;
    }

    //поедание фрукта
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
        if (speed > 30) speed -= 5;
    }
}

//основной цикл игры
int main() {
    int x, y, fruitX, fruitY, score, nTail, obstacleCount, speed = 100;
    int width = 40, height = 20;
    int** tail = nullptr;
    int** obstacles = nullptr;
    eDirection dir = STOP;

    srand(time(0));

    Setup(x, y, fruitX, fruitY, score, nTail, tail, obstacleCount, obstacles, width, height);

    while (dir != STOP) {
        Draw(x, y, fruitX, fruitY, score, nTail, tail, obstacleCount, obstacles, width, height);
        Input(dir);
        Logic(x, y, fruitX, fruitY, score, nTail, tail, dir, obstacleCount, obstacles, width, height, speed);
        Sleep(speed);
    }

    cout << "Game over! Your score: " << score << "\n";

    DeleteArray(tail, width * height);
    DeleteArray(obstacles, obstacleCount);
    return 0;
}
