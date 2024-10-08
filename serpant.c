#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <errno.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

int length;
int bend_no;
int len;
char key;
int life;  // Nombre de vies restantes
int applesEaten;  // Nombre de pommes mangées
void loadGame(char* filename);
void saveGame(char* filename, int showMessage);
void Delay(long double, int speed);
void Move(int speed, char* filename);
void Food();
int Score();
void Print();
void gotoxy(int x, int y);
void GotoXY(int x, int y);
void Bend();
void Boarder();
void Down();
void Left();
void Up();
void Right();
void ExitGame(char* filename);
int Scoreonly();
void chooseSpeed();
void startMenu();
void addFileExtension(char* filename, const char* extension);
void displaySnake();

struct coordinate {
    int x;
    int y;
    int direction;
};

typedef struct coordinate coordinate;

coordinate head, bend[500], apples[3], body[30];

int main() {
    startMenu();
    return 0;
}

// Fonction pour ajouter une extension par défaut au fichier si l'utilisateur n'en donne pas
void addFileExtension(char* filename, const char* extension) {
    if (strchr(filename, '.') == NULL) {
        strcat(filename, extension);
    }
}

// Menu principal pour choisir entre commencer une nouvelle partie ou charger une partie existante
void startMenu() {
    int choice;
    char filename[100];

    printf("Bienvenue dans Snake !\n");
    printf("1. Nouvelle Partie\n");
    printf("2. Charger une Partie\n");
    printf("Votre choix : ");
    scanf("%d", &choice);
    getchar(); // Pour consommer le retour à la ligne

    if (choice == 1) {
        printf("Entrez le nom de fichier pour sauvegarder cette partie (sans extension) : ");
        fgets(filename, 100, stdin);
        filename[strcspn(filename, "\n")] = 0;  // Enlever le caractère de nouvelle ligne
        addFileExtension(filename, ".dat");  // Ajouter l'extension par défaut si besoin

        // Test de création du fichier de sauvegarde
        FILE *file = fopen(filename, "wb");
        if (file == NULL) {
            printf("Erreur : Impossible de creer le fichier de sauvegarde : %s\n", strerror(errno));
            exit(1);  // Si on ne peut pas créer le fichier, on arrête tout
        } else {
            fclose(file);  // Fermer immédiatement le fichier pour valider la création
        }

        system("cls");
        printf("Creation d'une nouvelle partie...\n");
        length = 5;
        head.x = 25;
        head.y = 20;
        head.direction = RIGHT;
        life = 3; // Initialisation des vies
        applesEaten = 0; // Réinitialisation du compteur de pommes mangées
        bend_no = 0;
        bend[0] = head;
        chooseSpeed(); // Choisir la vitesse avant d'afficher le plateau
        Boarder();
        Food();
        Move(1, filename); // Commencer le jeu après la création
    } else if (choice == 2) {
        printf("Entrez le nom du fichier a charger (sans extension) : ");
        fgets(filename, 100, stdin);
        filename[strcspn(filename, "\n")] = 0;  // Enlever le caractère de nouvelle ligne
        addFileExtension(filename, ".dat");  // Ajouter l'extension par défaut si besoin

        // Afficher le chemin complet où le fichier est recherché
        printf("Chargement de la partie a partir du fichier: %s\n", filename);

        system("cls");
        loadGame(filename); // Charger la partie à partir du fichier
    } else {
        printf("Choix invalide. Veuillez redemarrer le programme.\n");
        exit(1);
    }
}

void Move(int speed, char* filename) {
    int a, i;
    do {
        Food();
        fflush(stdin);
        len = 0;
        for (i = 0; i < 30; i++) {
            body[i].x = 0;
            body[i].y = 0;
            if (i == length)
                break;
        }

        Delay(length, speed);
        Boarder();
        
        if (head.direction == RIGHT)
            Right();
        else if (head.direction == LEFT)
            Left();
        else if (head.direction == DOWN)
            Down();
        else if (head.direction == UP)
            Up();

        // Sauvegarde automatique après chaque déplacement
        saveGame(filename, 0);

        ExitGame(filename);
    } while (!kbhit());

    a = getch();
    if (a == 27) { // Touche "Échap" pour quitter
        saveGame(filename, 1); // Sauvegarder la partie avant de quitter
        system("cls");
        exit(0);
    }
    key = getch();

    if ((key == RIGHT && head.direction != LEFT && head.direction != RIGHT) ||
        (key == LEFT && head.direction != RIGHT && head.direction != LEFT) ||
        (key == UP && head.direction != DOWN && head.direction != UP) ||
        (key == DOWN && head.direction != UP && head.direction != DOWN)) {

        bend_no++;
        bend[bend_no] = head;
        head.direction = key;

        if (key == UP)
            head.y--;
        if (key == DOWN)
            head.y++;
        if (key == RIGHT)
            head.x++;
        if (key == LEFT)
            head.x--;

        // Sauvegarde après chaque changement de direction
        saveGame(filename, 0);
        
        Move(speed, filename);
    } else if (key == 27) {
        saveGame(filename, 1);
        system("cls");
        exit(0);
    } else {
        printf("\a");
        Move(speed, filename);
    }
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GotoXY(int x, int y) {
    HANDLE a;
    COORD b;
    fflush(stdout);
    b.X = x;
    b.Y = y;
    a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a, b);
}

void Down() {
    int i;
    for (i = 0; i <= (head.y - bend[bend_no].y) && len < length; i++) {
        GotoXY(head.x, head.y - i);
        {
            if (len == 0)
                printf("v");
            else
                printf("*");
        }
        body[len].x = head.x;
        body[len].y = head.y - i;
        len++;
    }
    Bend();
    if (!kbhit())
        head.y++;
}

void Up() {
    int i;
    for (i = 0; i <= (bend[bend_no].y - head.y) && len < length; i++) {
        GotoXY(head.x, head.y + i);
        {
            if (len == 0)
                printf("^");
            else
                printf("*");
        }
        body[len].x = head.x;
        body[len].y = head.y + i;
        len++;
    }
    Bend();
    if (!kbhit())
        head.y--;
}

void Delay(long double k, int speed) {
    Score();
    long double i;
    if (speed == 1) { // Vitesse lente
        for (i = 0; i <= (10000000 * k); i++);
    } else { // Vitesse rapide
        for (i = 0; i <= (5000000 * k); i++);
    }
}

void ExitGame(char* filename) {
    int i, check = 0;
    for (i = 4; i < length; i++) {
        if (body[0].x == body[i].x && body[0].y == body[i].y) {
            check++;
        }
        if (i == length || check != 0)
            break;
    }
    if (head.x <= 10 || head.x >= 70 || head.y <= 10 || head.y >= 30 || check != 0) {
        life--;  // Réduction de la vie
        if (life >= 0) {
            head.x = 25;
            head.y = 20;
            bend_no = 0;
            head.direction = RIGHT;
            Move(1, filename); // Relance à vitesse lente par défaut
        } else {
            system("cls");
            printf("Toutes les vies sont epuisees. Partie terminee !\n");

            // Supprimer le fichier de sauvegarde après la fin de la partie
            if (remove(filename) == 0) {
                printf("Le fichier de sauvegarde a ete supprime : %s\n", filename);
            } else {
                printf("Erreur lors de la suppression du fichier : %s\n", strerror(errno));
            }

            // Le jeu se termine ici, pas besoin de sauvegarder
            exit(0);
        }
    }
}

void Food() {
    if (applesEaten == 3) {
        length++; // Le serpent s'agrandit après avoir mangé 3 pommes
        applesEaten = 0;
    }

    for (int i = 0; i < 3; i++) {
        // Vérifier si seule la tête du serpent est à la position de la pomme
        if (head.x == apples[i].x && head.y == apples[i].y) {
            applesEaten++;
            apples[i].x = rand() % 70;
            if (apples[i].x <= 10)
                apples[i].x += 11;
            apples[i].y = rand() % 30;
            if (apples[i].y <= 10)
                apples[i].y += 11;
        }
        else if (apples[i].x == 0) {
            apples[i].x = rand() % 70;
            if (apples[i].x <= 10)
                apples[i].x += 11;
            apples[i].y = rand() % 30;
            if (apples[i].y <= 10)
                apples[i].y += 11;
        }

        // Afficher la pomme uniquement si elle n'est pas mangée
        GotoXY(apples[i].x, apples[i].y);
        printf("F");
    }
}

void Left() {
    int i;
    for (i = 0; i <= (bend[bend_no].x - head.x) && len < length; i++) {
        GotoXY((head.x + i), head.y);
        {
            if (len == 0)
                printf("<");
            else
                printf("*");
        }
        body[len].x = head.x + i;
        body[len].y = head.y;
        len++;
    }
    Bend();
    if (!kbhit())
        head.x--;
}

void Right() {
    int i;
    for (i = 0; i <= (head.x - bend[bend_no].x) && len < length; i++) {
        body[len].x = head.x - i;
        body[len].y = head.y;
        GotoXY(body[len].x, body[len].y);
        {
            if (len == 0)
                printf(">");
            else
                printf("*");
        }
        len++;
    }
    Bend();
    if (!kbhit())
        head.x++;
}

void Bend() {
    int i, j, diff;
    for (i = bend_no; i >= 0 && len < length; i--) {
        if (bend[i].x == bend[i - 1].x) {
            diff = bend[i].y - bend[i - 1].y;
            if (diff < 0)
                for (j = 1; j <= (-diff); j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y + j;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                    if (len == length)
                        break;
                }
            else if (diff > 0)
                for (j = 1; j <= diff; j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y - j;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                    if (len == length)
                        break;
                }
        } else if (bend[i].y == bend[i - 1].y) {
            diff = bend[i].x - bend[i - 1].x;
            if (diff < 0)
                for (j = 1; j <= (-diff) && len < length; j++) {
                    body[len].x = bend[i].x + j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                    if (len == length)
                        break;
                }
            else if (diff > 0)
                for (j = 1; j <= diff && len < length; j++) {
                    body[len].x = bend[i].x - j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printf("*");
                    len++;
                    if (len == length)
                        break;
                }
        }
    }
}

void Boarder() {
    system("cls"); // Nettoyer l'écran avant d'afficher la bordure
    int i;
    for (i = 10; i < 71; i++) {
        GotoXY(i, 10);
        printf("!");
        GotoXY(i, 30);
        printf("!");
    }
    for (i = 10; i < 31; i++) {
        GotoXY(10, i);
        printf("!");
        GotoXY(70, i);
        printf("!");
    }
}

int Score() {
    int score;
    GotoXY(20, 8);
    score = length - 5;
    printf("SCORE : %d", (length - 5));
    score = length - 5;
    GotoXY(50, 8);
    printf("Life : %d", life);
    return score;
}

// Sauvegarder la partie
void saveGame(char* filename, int showMessage) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur lors de la sauvegarde de la partie dans le fichier: %s\n", strerror(errno));
        return;
    }
    fwrite(&head, sizeof(coordinate), 1, file);
    fwrite(&length, sizeof(int), 1, file);  // Sauvegarde de la longueur du serpent
    fwrite(body, sizeof(coordinate), length, file);  // Sauvegarde du corps du serpent
    fwrite(&bend_no, sizeof(int), 1, file); // Sauvegarde du nombre de bends
    fwrite(bend, sizeof(coordinate), bend_no, file); // Sauvegarde des positions de bends
    fwrite(apples, sizeof(apples), 1, file);  // Sauvegarder les positions des pommes
    fwrite(&applesEaten, sizeof(int), 1, file);  // Sauvegarder le nombre de pommes mangées
    fwrite(&life, sizeof(int), 1, file);  // Sauvegarder les vies restantes
    fclose(file);

    // N'afficher le message que si showMessage est vrai
    if (showMessage) {
        printf("Partie sauvegardee dans le fichier: %s\n", filename);
    }
}

// Charger la partie
void loadGame(char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erreur: Fichier %s non trouve ou corrompu : %s.\n", filename, strerror(errno));
        exit(1);
    }
    fread(&head, sizeof(coordinate), 1, file);
    fread(&length, sizeof(int), 1, file);  // Charger la longueur avant de charger le corps du serpent
    fread(body, sizeof(coordinate), length, file);  // Charger le corps du serpent
    fread(&bend_no, sizeof(int), 1, file);  // Rechargement du nombre de bends
    fread(bend, sizeof(coordinate), bend_no, file);  // Rechargement des positions de bends
    fread(apples, sizeof(apples), 1, file);  // Recharger les positions des pommes
    fread(&applesEaten, sizeof(int), 1, file);  // Recharger le nombre de pommes mangées
    fread(&life, sizeof(int), 1, file);  // Recharger les vies restantes
    fclose(file);

    // Réinitialiser la première position de bend correctement
    bend[0] = head;
    
    // Afficher correctement la bordure du jeu
    system("cls");  // Nettoyer l'écran avant de reprendre la partie
    printf("Partie chargee depuis le fichier: %s !\n", filename);
    Boarder(); // Afficher la bordure

    // Afficher correctement le serpent à l'endroit où il a été sauvegardé
    displaySnake();  // Nouvelle fonction pour afficher le serpent au complet

    Delay(1, 1);  // Attendre un instant avant de reprendre le jeu pour éviter une collision immédiate
    Move(1, filename); // Reprendre le jeu avec la vitesse lente par défaut
}

// Fonction pour afficher le serpent après le chargement
void displaySnake() {
    int i;
    
    // Afficher la tête du serpent
    GotoXY(head.x, head.y);
    switch (head.direction) {
        case UP: printf("^"); break;
        case DOWN: printf("v"); break;
        case LEFT: printf("<"); break;
        case RIGHT: printf(">"); break;
    }

    // Afficher le corps du serpent
    for (i = 0; i < length; i++) {
        GotoXY(body[i].x, body[i].y);
        printf("*");
    }
}

// Choix de la vitesse
void chooseSpeed() {
    int choice;
    printf("Choisissez une vitesse : \n1. Lente\n2. Rapide\n");
    scanf("%d", &choice);
    getchar(); // Consommer la touche "entrée"
    system("cls"); // Nettoyer l'écran après avoir choisi la vitesse
}

