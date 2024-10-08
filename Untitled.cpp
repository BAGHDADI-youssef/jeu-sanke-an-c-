#include <stdio.h>
#include <time.h> 
#include <stdlib.h> 
#include <string.h> // Changé de strings.h à string.h
#include <windows.h> 
#include <conio.h> 
#define V 21
#define H 75
#define N 100

typedef struct
{
    int x, y; // initial position
    int movX, movY;
    char imagen;
} snake;

typedef struct
{
    int x, y;
} fruit;

snake snk[N]; // maximun size
fruit frt;

void begin(int *size, char map[V][H]);
void intro(char map[V][H]);
void intro_data(char map[V][H], int *size);
void loop(char map[V][H], int size);
void input(char map[V][H], int *size, int *dead);
void update(char map[V][H], int size);
void intro_data2(char map[V][H], int size);
void gotoxy(int x, int y);
void show(char map[V][H]);

int main()
{
    int size;
    char map[V][H];

    printf("Jeu du Serpent - Utilisez W, A, S, D pour vous deplacer.\n");
    printf("Appuyez sur une touche pour commencer...\n");
    getch(); // Attend que l'utilisateur appuie sur une touche

    begin(&size, map);
    show(map);
    loop(map, size);

    printf("Game Over!\n");
    system("pause");
    return 0;
}

void begin(int *size, char map[V][H])
{
    int i;
    snk[0].x = 32;
    snk[0].y = 10;
    *size = 4;

    srand(time(NULL));

    frt.x = rand() % (H - 2) + 1;
    frt.y = rand() % (V - 2) + 1;

    for (i = 0; i < *size; i++)
    {
        snk[i].movX = 1;
        snk[i].movY = 0;
    }

    intro(map);
    intro_data(map, size);
}

void show(char map[V][H])
{
    int i, j;
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < H; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

void intro(char map[V][H])
{
    int i, j;
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < H; j++)
        {
            if (i == 0 || i == V - 1)
            {
                map[i][j] = '-';
            }
            else if (j == 0 || j == H - 1)
            {
                map[i][j] = '|';
            }
            else
            {
                map[i][j] = ' ';
            }
        }
    }
}

void intro_data(char map[V][H], int *size)
{
    int i;
    for (i = 1; i < *size; i++)
    {
        snk[i].x = snk[i - 1].x - 1;
        snk[i].y = snk[i - 1].y;
        snk[i].imagen = 'X';
    }
    snk[0].imagen = 'O';

    for (i = 0; i < *size; i++)
    {
        map[snk[i].y][snk[i].x] = snk[i].imagen;
    }
    map[frt.y][frt.x] = 'M';
}

void loop(char map[V][H], int size)
{
    int dead = 0;
    do
    {
        gotoxy(0, 0);
        show(map);
        input(map, &size, &dead);
        update(map, size);
        Sleep(100); // Ajoute une pause de 100 ms
    } while (dead == 0);
}

void input(char map[V][H], int *size, int *dead)
{
    int i;
    char key;

    if (snk[0].x == 0 || snk[0].x == H - 1 || snk[0].y == 0 || snk[0].y == V - 1)
    {
        *dead = 1;
    }

    for (i = 1; i < *size && *dead == 0; i++)
    {
        if (snk[0].x == snk[i].x && snk[0].y == snk[i].y)
        {
            *dead = 1;
        }
    }

    if (snk[0].x == frt.x && snk[0].y == frt.y)
    {
        *size += 1;
        snk[*size - 1].imagen = 'X';
        frt.x = rand() % (H - 2) + 1;
        frt.y = rand() % (V - 2) + 1;
    }

    if (*dead == 0)
    {
        if (kbhit())
        {
            key = getch();
            printf("Touche pressee : %c\n", key); // Débogage

            switch(key)
            {
                case 'w':
                case 'W':
                    if (snk[0].movY != 1) { snk[0].movX = 0; snk[0].movY = -1; printf("Mouvement vers le haut\n"); }
                    break;
                case 's':
                case 'S':
                    if (snk[0].movY != -1) { snk[0].movX = 0; snk[0].movY = 1; printf("Mouvement vers le bas\n"); }
                    break;
                case 'a':
                case 'A':
                    if (snk[0].movX != 1) { snk[0].movX = -1; snk[0].movY = 0; printf("Mouvement vers la gauche\n"); }
                    break;
                case 'd':
                case 'D':
                    if (snk[0].movX != -1) { snk[0].movX = 1; snk[0].movY = 0; printf("Mouvement vers la droite\n"); }
                    break;
            }
        }
    }
}

void update(char map[V][H], int size)
{
    intro(map);
    intro_data2(map, size);
    printf("Position du serpent : x=%d, y=%d\n", snk[0].x, snk[0].y); // Débogage
}

void intro_data2(char map[V][H], int size)
{
    int i;
    for (i = size - 1; i > 0; i--)
    {
        snk[i].x = snk[i - 1].x;
        snk[i].y = snk[i - 1].y;
    }

    snk[0].x += snk[0].movX;
    snk[0].y += snk[0].movY;

    for (i = 0; i < size; i++)
    {
        map[snk[i].y][snk[i].x] = snk[i].imagen;
    }

    map[frt.y][frt.x] = 'M';
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
