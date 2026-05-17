#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cstdio>
using namespace std;

#pragma comment(lib, "winmm.lib")

int width = 800, height = 600;

enum GameState { MENU, PLAYING, PAUSED, GAME_OVER };
GameState gameState = MENU;

struct Chicken {
    float x, y;
    bool moveRight;
};

struct Object {
    float x, y;
    int type;
    float speed;
};

vector<Chicken> chickens;
vector<Object> objects;

float basketX = 350;
float basketWidth = 100;

int score = 0;
int timeLeft = 60;

// ---------------- TEXT ----------------
void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

// ---------------- ELLIPSE ----------------
void drawEllipse(float cx, float cy, float rx, float ry)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++)
    {
        float rad = i * 3.1416 / 180.0;

        glVertex2f(
            cx + rx * cos(rad),
            cy + ry * sin(rad)
        );
    }

    glEnd();
}

// ---------------- BACKGROUND ----------------
void drawBackground()
{
    glBegin(GL_QUADS);

    glColor3f(0.45, 0.75, 1.0);
    glVertex2f(0, 0);
    glVertex2f(width, 0);

    glColor3f(0.25, 0.55, 1.0);
    glVertex2f(width, height);
    glVertex2f(0, height);

    glEnd();

    glColor3f(0.0, 0.75, 0.1);

    glBegin(GL_QUADS);

        glVertex2f(0, 0);
        glVertex2f(width, 0);
        glVertex2f(width, 45);
        glVertex2f(0, 45);

    glEnd();
}

// ---------------- STICKS ----------------
void drawSticks()
{
    glColor3f(0.35, 0.18, 0.05);

    glLineWidth(5);

    glBegin(GL_LINES);

        glVertex2f(0, 500);
        glVertex2f(width, 500);

        glVertex2f(0, 420);
        glVertex2f(width, 420);

        glVertex2f(0, 340);
        glVertex2f(width, 340);

    glEnd();

    glLineWidth(1);
}

// ---------------- DISPLAY ----------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    drawSticks();

    glColor3f(0,0,0);

    char s[100];

    sprintf(s, "Score: %d", score);
    drawText(20, 560, s);

    sprintf(s, "Time: %d", timeLeft);
    drawText(650, 560, s);

    if(gameState == MENU)
    {
        drawText(300, 390, "CATCH THE EGGS");
        drawText(240, 340, "Press ENTER to Start");
    }

    glFlush();
}

// ---------------- CHICKEN ----------------
void drawChicken(float x, float y)
{
    glColor3f(1.0, 0.72, 0.35);
    drawEllipse(x, y, 28, 18);

    glColor3f(0.95, 0.50, 0.20);
    drawEllipse(x + 5, y - 2, 13, 9);

    glColor3f(1.0, 0.82, 0.45);
    drawEllipse(x + 28, y + 17, 11, 11);

    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLES);

        glVertex2f(x + 22, y + 27);
        glVertex2f(x + 27, y + 39);
        glVertex2f(x + 32, y + 27);

    glEnd();
}

// ---------------- EGG ----------------
void drawEgg(float x, float y, int type)
{
    glColor3f(0,0,0);

    drawEllipse(x, y, 11, 16);

    if(type == 0)
        glColor3f(1,1,1);

    else if(type == 1)
        glColor3f(0,0,1);

    else
        glColor3f(1,1,0);

    drawEllipse(x, y, 9, 14);
}

// ---------------- POOP ----------------
void drawPoop(float x, float y)
{
    glColor3f(0.35, 0.18, 0.05);

    drawEllipse(x, y, 10, 15);
}

// ---------------- OBJECT ----------------
void drawObject(Object &o)
{
    if(o.type == 3)
        drawPoop(o.x, o.y);

    else
        drawEgg(o.x, o.y, o.type);
}

// ---------------- BASKET ----------------
void drawBasket()
{
    glColor3f(1,0.5,0);

    glBegin(GL_QUADS);

        glVertex2f(basketX, 50);
        glVertex2f(basketX + basketWidth, 50);
        glVertex2f(basketX + basketWidth, 80);
        glVertex2f(basketX, 80);

    glEnd();
}

// ---------------- SPAWN ----------------
void spawnObject()
{
    for(int i = 0; i < chickens.size(); i++)
    {
        if(rand() % 140 == 0)
        {
            Object o;

            o.x = chickens[i].x;
            o.y = chickens[i].y - 30;

            int r = rand() % 100;

            if(r < 45)
                o.type = 0;

            else if(r < 70)
                o.type = 1;

            else if(r < 88)
                o.type = 2;

            else
                o.type = 3;

            o.speed = 1.5 + (rand() % 2);

            objects.push_back(o);
        }
    }
}

// ---------------- INIT ----------------
void init()
{
    glClearColor(1,1,1,1);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

// ---------------- MAIN ----------------
int main(int argc, char** argv)
{
    srand(time(0));

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(width, height);

    glutCreateWindow("Catch The Eggs");

    init();

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
