
#include<windows.h>
#include <gl\glut.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<time.h>
#include <assert.h>
#define MAX_CHAR       128
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#include <string>
#include <iostream>
#include <sstream>
float PI = 3.1415926f;
#define M_PI 3.1415926
GLfloat x = 0; //initial position of ball
GLfloat dx = 0.1; //increment left or right
GLfloat ymax; //max value of y
GLfloat v1 = 0.0; //shift of the screen
GLfloat vold = 0.0, _count = -1;
GLfloat width = 450, height = 650; //width and height of the screen
int xScreen, yScreen;
GLfloat p = -4;     //parameters used
GLfloat jumpHeight = 1.5;
GLfloat y = 0.15 + jumpHeight; //initial y location of ball
GLfloat vmax, yref = 1.5;
GLfloat acceleration = 0.000008, Velocity = 0.0;  //acceleration of ball.
GLfloat transX = 0.0, transdX = 0.002;    //variables for translation motion of bricks.
int v = 1, r = 1;
unsigned char pic_col[3];  //stores color combination of pixel just below ball
GLint page = 0;
GLfloat angle = 0.0;
int level = 0;
bool stop = false;
bool halt = false;
int score = 0;
int _time = 0;
int time_count = 0;
using namespace std;
string Introdution;

void plotPoints(int xc, int yc, int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    glEnd();
}

void bresenhamCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        plotPoints(xc, yc, x, y);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else {
            d = d + 4 * x + 6;
        }
    }
}

// Midpoint ellipse drawing function
void drawEllipse(int centerX, int centerY, int a, int b) {
    int x = 0;
    int y = b;
    int a2 = a * a;
    int b2 = b * b;
    int err = b2 - a2 * b + a2 / 4;
    int dx = 2 * b2 * x;
    int dy = 2 * a2 * y;

    while (b2 * x <= a2 * y) {
        glVertex2f(centerX + x, centerY + y);
        glVertex2f(centerX - x, centerY + y);
        glVertex2f(centerX + x, centerY - y);
        glVertex2f(centerX - x, centerY - y);

        if (err >= 0) {
            y--;
            dy -= 2 * a2;
            err -= dy;
        }
        x++;
        dx += 2 * b2;
        err += dx;
    }

    x = a;
    y = 0;
    err = a2 - b2 * a + b2 / 4;
    dx = 2 * b2 * x;
    dy = 2 * a2 * y;

    while (a2 * y <= b2 * x) {
        glVertex2f(centerX + x, centerY + y);
        glVertex2f(centerX - x, centerY + y);
        glVertex2f(centerX + x, centerY - y);
        glVertex2f(centerX - x, centerY - y);

        if (err <= 0) {
            x--;
            dx -= 2 * b2;
            err -= dx;
        }
        y++;
        dy += 2 * a2;
        err += dy;
    }
}

void zapdos() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear display window.// Set line segment color to red

#define EYE 1
    glNewList(EYE, GL_COMPILE);

    // Eye white polygon
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(100, 400);
    glVertex2f(100, 390);
    glVertex2f(70, 410);
    glVertex2f(70, 440);
    glEnd();

    // Eye outline
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(100, 400);
    glVertex2f(100, 390);
    glVertex2f(70, 410);
    glVertex2f(70, 440);
    glEnd();

    // Pupil rectangle
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(75, 408);
    glVertex2f(75, 435);
    glVertex2f(70, 440);
    glVertex2f(70, 410);
    glEnd();

    // Circular highlight (pupil) using Bresenham's algorithm
    glColor3f(0.0, 0.0, 0.0);
    bresenhamCircle(90, 412, 12); // center at (90, 412), radius 12

    glEndList();

#define MOUTH 2
    glNewList(MOUTH, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    //midpoint ellipse
    drawEllipse(300, 300, 30, 20);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(292, 315);
    glVertex2f(295, 305);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(308, 315);
    glVertex2f(305, 305);
    glEnd();
    glEndList();

#define FACE 3
    glNewList(FACE, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.8, 0.0);
    glVertex2f(300, 450);
    glVertex2f(320, 350);
    glVertex2f(270, 360);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(280, 360);
    glVertex2f(230, 430);
    glVertex2f(250, 340);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(260, 350);
    glVertex2f(170, 320);
    glVertex2f(250, 300);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(250, 307);
    glVertex2f(220, 285);
    glVertex2f(270, 300);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(262, 305);
    glVertex2f(250, 280);
    glVertex2f(280, 295);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(310, 360);
    glVertex2f(400, 400);
    glVertex2f(350, 310);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(340, 330);
    glVertex2f(430, 320);
    glVertex2f(350, 290);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(350, 295);
    glVertex2f(390, 200);
    glVertex2f(310, 270);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(315, 272);
    glVertex2f(305, 220);
    glVertex2f(300, 270);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(300, 270);
    glVertex2f(285, 250);
    glVertex2f(290, 280);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(320, 350);
    glVertex2f(270, 360);
    glVertex2f(260, 350);
    glVertex2f(250, 307);
    glVertex2f(268, 301);
    glVertex2f(350, 310);
    glVertex2f(350, 290);
    glVertex2f(315, 272);
    glVertex2f(300, 270);
    glVertex2f(293, 275);
    glVertex2f(260, 295);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(290, 280);
    glVertex2f(285, 250);
    glVertex2f(300, 270);
    glVertex2f(305, 220);
    glVertex2f(313, 270);
    glVertex2f(390, 200);
    glVertex2f(350, 290);
    glVertex2f(430, 320);
    glVertex2f(356, 326);
    glVertex2f(400, 400);
    glVertex2f(315, 360);
    glVertex2f(300, 450);
    glVertex2f(272, 367);
    glVertex2f(230, 430);
    glVertex2f(248, 345);
    glVertex2f(170, 320);
    glVertex2f(245, 300);
    glVertex2f(220, 285);
    glVertex2f(260, 295);
    glVertex2f(250, 280);
    glVertex2f(280, 295);
    glEnd();
    glEndList();
#define FACES 4
    glNewList(FACES, GL_COMPILE);
    glRotated(30, 0, 0, 1);
    glTranslated(100, -200, 0);
    glScaled(1.2, 1.2, 0);
    glCallList(FACE);
    glRotated(-30, 0, 0, 1);
    glScaled(0.8, 0.8, 0);
    glTranslated(10, 110, 0);
    glCallList(EYE);
    glTranslated(-185, 60, 0);
    glCallList(MOUTH);
    glTranslated(413, -60, 0);
    glRotated(180, 0, 1, 0);
    glCallList(EYE);
    glEndList();
#define HAND 5
    glNewList(HAND, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(250, 320);
    glVertex2f(170, 340);
    glVertex2f(250, 290);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(210, 290);
    glVertex2f(170, 280);
    glVertex2f(250, 260);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.8, 0.0);
    glVertex2f(300, 300);
    glVertex2f(200, 350);
    glVertex2f(260, 300);
    glVertex2f(160, 320);
    glVertex2f(220, 280);
    glVertex2f(190, 260);
    glVertex2f(260, 260);
    glVertex2f(230, 230);
    glVertex2f(300, 260);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(300, 300);
    glVertex2f(200, 350);
    glVertex2f(255, 305);
    glVertex2f(160, 320);
    glVertex2f(220, 280);
    glVertex2f(190, 260);
    glVertex2f(260, 260);
    glVertex2f(230, 230);
    glVertex2f(300, 260);
    glEnd();
    glEndList();
#define BODY 6
    glNewList(BODY, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.8, 0.0);
    for (int i = 0;i < 360;i++)
        glVertex2f(55 * cos(i * PI / 180) + 300, 55 * sin(i * PI / 180) + 340);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0;i < 360;i++)
        glVertex2f(55 * cos(i * PI / 180) + 300, 55 * sin(i * PI / 180) + 340);
    glEnd();
    glEndList();
#define LEG 7
    glNewList(LEG, GL_COMPILE);

    glBegin(GL_POLYGON);
    glColor3f(0.64, 0.46, 0.22);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.8 * 28 * cos(i * PI / 180) + 280, 28 * sin(i * PI / 180) + 305);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.64, 0.46, 0.22);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.8 * 28 * cos(i * PI / 180) + 320, 28 * sin(i * PI / 180) + 305);
    glEnd();
    glEndList();
#define FEET 8
    glNewList(FEET, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2f(265, 300);
    glVertex2f(258, 268);
    for (int i = 100;i < 410;i++)
        glVertex2f(0.6 * 20 * cos(i * PI / 180) + 260, 20 * sin(i * PI / 180) + 250);
    glVertex2f(270, 270);
    glVertex2f(280, 290);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(265, 300);
    glVertex2f(258, 268);
    for (int i = 100;i < 410;i++)
        glVertex2f(0.6 * 20 * cos(i * PI / 180) + 260, 20 * sin(i * PI / 180) + 250);
    glVertex2f(270, 270);
    glVertex2f(280, 290);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(265, 240);
    glVertex2f(265, 230);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(260, 240);
    glVertex2f(260, 230);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(255, 240);
    glVertex2f(255, 232);
    glEnd();
    glEndList();
#define HOLE 9
    glNewList(HOLE, GL_COMPILE_AND_EXECUTE);
    glCallList(FEET);
    glTranslated(50, 0, 0);
    glCallList(FEET);
    glTranslated(-50, 0, 0);
    glCallList(LEG);
    glScaled(1.5, 1.5, 0);
    glTranslated(-100, -40, 0);
    glCallList(HAND);
    glTranslated(625, -10, 0);
    glRotated(180, 0, 1, 0);
    glCallList(HAND);
    glRotated(180, 0, 1, 0);
    glTranslated(-625, 10, 0);
    glTranslated(150, 90, 0);
    glScaled(0.5, 0.5, 0);
    glCallList(BODY);
    glCallList(FACES);

    glEndList();
    glFlush();
    // Process all OpenGL routines as quickly as possible.
}
void squirtle()
{
    //glClear (GL_COLOR_BUFFER_BIT); // Clear display window.// Set line segment color to red
    int n = 3600;
    int N = 0;
    float PI = 3.1415926f;
    float R = 100, r;
    float x, y, j, _a, b, k;
#define EYEt 8
    glNewList(EYEt, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0;i < 180;i++)
    {
        x = 0.3 * 100 * cos(i * PI / 180);
        y = 0.8 * 100 * sin(i * PI / 180);
        glVertex2f(x + 320, y + 390);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0;i < 180;i++)
    {
        x = 0.3 * 100 * cos(i * PI / 180);
        y = 0.8 * 100 * sin(i * PI / 180);
        glVertex2f(x + 320, y + 390);
    }
    glEnd();
    glEndList();
#define EYESt 9
    glNewList(EYESt, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = -5;i < 185;i++)
    {
        x = 0.3 * 90 * cos(i * PI / 180);
        y = 0.8 * 90 * sin(i * PI / 180);
        glVertex2f(x + 315, y + 397);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0;i < n;i++)
    {
        x = 0.5 * 20 * cos(i * PI / 180);
        y = 0.8 * 20 * sin(i * PI / 180);
        _a = x * cos(15 * PI / 180) + y * sin(15 * PI / 180);
        b = x * sin(15 * PI / 180) + y * cos(15 * PI / 180);
        glVertex2f(_a + 307, b + 450);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.73, 0.2, 0.078);
    for (int i = 50;i < 145;i++)
    {
        x = 0.3 * 120 * cos(i * PI / 180);
        y = 0.8 * 120 * sin(i * PI / 180);
        glVertex2f(x + 318, y + 335);
    }
    glVertex2i(342, 390);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0;i < n;i++)
    {
        x = 0.6 * 20 * cos(i * PI / 180);
        y = 0.8 * 20 * sin(i * PI / 180);
        glVertex2f(x + 318, y + 423);
    }
    glEnd();
    glEndList();
#define MOUTHt 10
    glNewList(MOUTHt, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 210;i < 270;i++)
        glVertex2f(20 * cos(i * PI / 180) + 215, 20 * sin(i * PI / 180) + 370);
    for (int i = 110;i > 70;i--)
        glVertex2f(50 * cos(i * PI / 180) + 230, 50 * sin(i * PI / 180) + 303);
    glVertex2f(260, 340);
    for (int i = 110;i > 70;i--)
        glVertex2f(50 * cos(i * PI / 180) + 290, 50 * sin(i * PI / 180) + 303);
    for (int i = 270;i < 330;i++)
        glVertex2f(20 * cos(i * PI / 180) + 315, 20 * sin(i * PI / 180) + 370);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(247, 360);
    glVertex2f(255, 355);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(272, 360);
    glVertex2f(265, 355);
    glEnd();
    glEndList();

#define FACEt 11
    glNewList(FACEt, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.49, 0.84, 0.98);
    for (int i = 0;i < n;i++)
    {
        x = 120 * cos(i * PI / 180);
        y = 0.8 * 120 * sin(i * PI / 180);
        glVertex2f(x + 260, y + 395);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0;i < n;i++)
    {
        x = 120 * cos(i * PI / 180);
        y = 0.8 * 120 * sin(i * PI / 180);
        glVertex2f(x + 260, y + 395);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 65;i < 85;i++)
        glVertex2f(0.3 * 100 * cos(i * PI / 180) + 215, 0.8 * 100 * sin(i * PI / 180) + 390);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 95;i < 115;i++)
        glVertex2f(0.3 * 100 * cos(i * PI / 180) + 310, 0.8 * 100 * sin(i * PI / 180) + 390);
    glEnd();
    glCallList(EYEt);
    glCallList(EYESt);
    glTranslated(-115, 0, 0);
    glCallList(EYEt);
    glTranslated(10, 0, 0);
    glCallList(EYESt);
    glTranslated(105, 0, 0);
    glCallList(MOUTHt);
    glEndList();
#define HANDt 12
    glNewList(HANDt, GL_COMPILE);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 390;i > 150;i--)
    {
        x = 0.4 * 71 * cos(i * PI / 180);
        y = 0.9 * 71 * sin(i * PI / 180);
        glVertex2f(x + 200, y + 200);
    }
    glEnd();
    glColor3f(0.49, 0.84, 0.98);
    glBegin(GL_POLYGON);
    glVertex2f(180, 160);
    glVertex2f(210, 150);
    glVertex2f(175, 135);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(180, 150);
    glVertex2f(210, 145);
    glVertex2f(200, 130);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(200, 145);
    glVertex2f(220, 160);
    glVertex2f(220, 135);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(180, 160);
    glVertex2f(175, 135);
    glVertex2f(210, 150);
    glVertex2f(183, 150);
    glVertex2f(200, 130);
    glVertex2f(210, 145);
    glVertex2f(200, 145);
    glVertex2f(220, 135);
    glVertex2f(220, 160);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.49, 0.84, 0.98);
    for (int i = 390;i > 150;i--)
    {
        x = 0.4 * 70 * cos(i * PI / 180);
        y = 0.9 * 70 * sin(i * PI / 180);
        glVertex2f(x + 200, y + 200);
    }
    glEnd();
    glEndList();
#define BODYt 13
    glNewList(BODYt, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.72, 0.48, 0.18);
    for (int i = 300;i < 425;i++)
    {
        x = 120 * cos(i * PI / 180);
        y = 0.85 * 120 * sin(i * PI / 180);
        glVertex2f(x + 282, y + 200);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.98, 0.97, 0.58);
    for (int i = 0;i < n;i++)
    {
        x = 0.95 * 120 * cos(i * PI / 180);
        y = 120 * sin(i * PI / 180);
        glVertex2f(x + 260, y + 200);
    }
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0;i < n;i++)
    {
        x = 0.95 * 120 * cos(i * PI / 180);
        y = 120 * sin(i * PI / 180);
        glVertex2f(x + 260, y + 200);
    }
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(170, 250);
    glVertex2f(350, 250);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(260, 250);
    glVertex2f(260, 150);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(200, 200);
    glVertex2f(320, 200);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(170, 250);
    glVertex2f(200, 200);
    glVertex2f(180, 140);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(350, 250);
    glVertex2f(320, 200);
    glVertex2f(340, 140);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(200, 120);
    glVertex2f(260, 150);
    glVertex2f(320, 120);
    glEnd();
    glEndList();
#define TAILt 14
    glNewList(TAILt, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.49, 0.84, 0.98);
    for (int i = 220;i < 290;i++)
        glVertex2f(90 * cos(i * PI / 180) + 130, 90 * sin(i * PI / 180) + 135);
    glVertex2f(0.9 * 40 * cos(10 * PI / 180) + 91, 40 * sin(10 * PI / 180) + 100);
    for (int i = 270;i > 0;i--)
        glVertex2f(20 * cos(i * PI / 180) + 100, 20 * sin(i * PI / 180) + 100);
    for (int i = 380;i > 270;i--)
        glVertex2f(30 * cos(i * PI / 180) + 91, 30 * sin(i * PI / 180) + 90);
    for (int i = 270;i > 0;i--)
        glVertex2f(0.9 * 40 * cos(i * PI / 180) + 91, 40 * sin(i * PI / 180) + 100);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 270;i > 0;i--)
        glVertex2f(20 * cos(i * PI / 180) + 100, 20 * sin(i * PI / 180) + 100);
    for (int i = 380;i > 270;i--)
        glVertex2f(30 * cos(i * PI / 180) + 91, 30 * sin(i * PI / 180) + 90);
    for (int i = 270;i > 0;i--)
        glVertex2f(0.9 * 40 * cos(i * PI / 180) + 91, 40 * sin(i * PI / 180) + 100);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 230;i < 290;i++)
        glVertex2f(90 * cos(i * PI / 180) + 130, 90 * sin(i * PI / 180) + 135);
    glEnd();
    glEndList();
#define HOLEt 15
    glNewList(HOLEt, GL_COMPILE);
    glTranslated(30, 70, 0);
    glCallList(TAILt);
    glTranslated(-30, -70, 0);
    glCallList(BODYt);
    glTranslated(0, -30, 0);
    glCallList(FACEt);
    glRotatef(20, 0.0f, 0.0f, 1.0f);
    glTranslated(150, -200, 0);
    glCallList(HANDt);
    glTranslated(-150, 200, 0);
    glRotatef(-20, 0.0f, 0.0f, 1.0f);
    glRotatef(-20, 0.0f, 0.0f, 1.0f);
    glTranslated(-60, -20, 0);
    glCallList(HANDt);
    glTranslated(60, 20, 0);
    glRotatef(20, 0.0f, 0.0f, 1.0f);
    glRotatef(-120, 0.0f, 0.0f, 1.0f);
    glTranslated(-535, -210, 0);
    glCallList(HANDt);
    glTranslated(535, 210, 0);
    glRotatef(120, 0.0f, 0.0f, 1.0f);
    glRotatef(120, 0.0f, 0.0f, 1.0f);
    glTranslated(-120, -660, 0);
    glCallList(HANDt);
    glTranslated(120, 660, 0);
    glRotatef(-120, 0.0f, 0.0f, 1.0f);
    glEndList();
#define COPYt 16
    glNewList(COPYt, GL_COMPILE_AND_EXECUTE);
    glCallList(HOLEt);
    //glTranslated(-1200,-1800,0);
    //glScaled(12,12,0);
    glFlush();
    // Process all OpenGL routines as quickly as possible.
}
void resetpar() { //reset all parameters
    x = 0; //initial position of ball
    dx = 0.1; //increment left or right
    ymax; //max value of y
    v1 = 0.0; //shift of the screen
    vold = 0.0;
    _count = -1;
    width = 450;
    height = 650; //width and height of the screen
    jumpHeight = 1.5;
    y = 0.15 + jumpHeight; //initial y location of ball
    acceleration = 0.000008;
    Velocity = 0.0;  //acceleration of ball.
    p = -4;
    angle = 0.0;
    transX = 0.0;
    transdX = 0.002;
    stop = false;
    halt = false;
    //score = 0;
    _time = 0;
    time_count = 0;
    level = 0;
}
void feature()
{
#define HAT 25
    glLineWidth(1.0f);
    glNewList(HAT, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(240, 165);
    for (int i = -10;i < 180;i++)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.4 * 40 * sin(i * PI / 180) + 195);
    glVertex2f(160, 165);
    for (int i = 180;i > 0;i--)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.2 * 40 * sin(i * PI / 180) + 170);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 180;i > 0;i--)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.2 * 40 * sin(i * PI / 180) + 165);
    glVertex2f(230, 155);
    for (int i = 40;i < 140;i++)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.2 * 40 * sin(i * PI / 180) + 150);
    glVertex2f(160, 165);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(240, 165);
    for (int i = -10;i < 180;i++)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.4 * 40 * sin(i * PI / 180) + 195);
    glVertex2f(160, 165);
    for (int i = 180;i > 0;i--)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.2 * 40 * sin(i * PI / 180) + 165);
    glVertex2f(230, 155);
    for (int i = 40;i < 140;i++)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.2 * 40 * sin(i * PI / 180) + 150);
    glVertex2f(160, 165);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 180;i > 0;i--)
        glVertex2f(40 * cos(i * PI / 180) + 200, 0.1 * 40 * sin(i * PI / 180) + 175);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0;i < 180;i++)
        glVertex2f(25 * cos(i * PI / 180) + 200, 25 * sin(i * PI / 180) + 179);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    for (int i = 180;i > 0;i--)
        glVertex2f(15 * cos(i * PI / 180) + 200, 15 * sin(i * PI / 180) + 179);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0;i < 180;i++)
        glVertex2f(25 * cos(i * PI / 180) + 200, 25 * sin(i * PI / 180) + 179);
    for (int i = 180;i > 0;i--)
        glVertex2f(15 * cos(i * PI / 180) + 200, 15 * sin(i * PI / 180) + 179);
    glEnd();
    glEndList();
#define FACEf 26
    glNewList(FACEf, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.8, 0.55);
    for (int i = -40;i < 40;i++)
        glVertex2f(0.5 * 50 * cos(i * PI / 180) + 232, 50 * sin(i * PI / 180) + 148);
    for (int i = 0;i < 180;i++)
        glVertex2f(50 * cos(i * PI / 180) + 200, 0.4 * 50 * sin(i * PI / 180) + 180);
    for (int i = 140;i < 220;i++)
        glVertex2f(0.5 * 50 * cos(i * PI / 180) + 168, 50 * sin(i * PI / 180) + 148);
    glVertex2f(200, 95);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = -40;i < 40;i++)
        glVertex2f(0.5 * 50 * cos(i * PI / 180) + 232, 50 * sin(i * PI / 180) + 148);
    for (int i = 0;i < 180;i++)
        glVertex2f(50 * cos(i * PI / 180) + 200, 0.4 * 50 * sin(i * PI / 180) + 180);
    for (int i = 140;i < 220;i++)
        glVertex2f(0.5 * 50 * cos(i * PI / 180) + 168, 50 * sin(i * PI / 180) + 148);
    glVertex2f(200, 95);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = -40;i < 220;i++)
        glVertex2f(0.5 * 20 * cos(i * PI / 180) + 232, 20 * sin(i * PI / 180) + 148);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = -40;i < 220;i++)
        glVertex2f(0.5 * 20 * cos(i * PI / 180) + 172, 20 * sin(i * PI / 180) + 148);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(225, 135);
    for (int i = -40;i < 130;i++)
        glVertex2f(0.5 * 20 * cos(i * PI / 180) + 232, 20 * sin(i * PI / 180) + 148);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 50;i < 220;i++)
        glVertex2f(0.5 * 20 * cos(i * PI / 180) + 172, 20 * sin(i * PI / 180) + 148);
    glVertex2f(182, 135);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = -40;i < 220;i++)
        glVertex2f(0.2 * 18 * cos(i * PI / 180) + 175, 18 * sin(i * PI / 180) + 146);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = -40;i < 220;i++)
        glVertex2f(0.2 * 18 * cos(i * PI / 180) + 230, 18 * sin(i * PI / 180) + 146);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = -30;i < 210;i++)
        glVertex2f(0.25 * 6 * cos(i * PI / 180) + 175, 6 * sin(i * PI / 180) + 152);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = -30;i < 210;i++)
        glVertex2f(0.25 * 6 * cos(i * PI / 180) + 230, 6 * sin(i * PI / 180) + 152);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.6, 0.2, 0.2);
    for (int i = 200;i < 340;i++)
        glVertex2f(0.6 * 30 * cos(i * PI / 180) + 200, 30 * sin(i * PI / 180) + 130);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.9, 0.5, 0.4);
    for (int i = 25;i < 155;i++)
        glVertex2f(15 * cos(i * PI / 180) + 200, 15 * sin(i * PI / 180) + 105);
    for (int i = 220;i < 320;i++)
        glVertex2f(0.6 * 30 * cos(i * PI / 180) + 200, 30 * sin(i * PI / 180) + 130);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 200;i < 340;i++)
        glVertex2f(0.6 * 30 * cos(i * PI / 180) + 200, 30 * sin(i * PI / 180) + 130);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(163, 127);
    glVertex2f(172, 130);
    glVertex2f(167, 122);
    glVertex2f(175, 125);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(226, 127);
    glVertex2f(235, 130);
    glVertex2f(230, 122);
    glVertex2f(238, 125);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(200, 132);
    glVertex2f(202, 130);
    glVertex2f(200, 128);
    glEnd();
    glEndList();
#define EARf 27
    glNewList(EARf, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.8, 0.55);
    for (int i = 90;i < 290;i++)
        glVertex2f(0.6 * 15 * cos(i * PI / 180) + 142, 15 * sin(i * PI / 180) + 137);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 90;i < 290;i++)
        glVertex2f(0.6 * 15 * cos(i * PI / 180) + 142, 15 * sin(i * PI / 180) + 137);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 180;i < 400;i++)
        glVertex2f(3 * cos(i * PI / 180) + 140, 3 * sin(i * PI / 180) + 128);
    for (int i = -60;i < 170;i++)
        glVertex2f(3 * cos(i * PI / 180) + 138, 3 * sin(i * PI / 180) + 140);
    glEnd();
    glEndList();
#define HAIRf 28
    glNewList(HAIRf, GL_COMPILE);
    glBegin(GL_POLYGON);
    glVertex2f(148, 178);
    glVertex2f(115, 160);
    glVertex2f(130, 155);
    glVertex2f(113, 145);
    glVertex2f(132, 138);
    glVertex2f(120, 135);
    glVertex2f(143, 125);
    glEnd();
    glEndList();
#define allf 29
    glNewList(allf, GL_COMPILE);

    glCallList(FACEf);
    glCallList(HAIRf);
    glCallList(EARf);
    glRotated(180, 0, 1, 0);
    glTranslated(-400, 0, 0);
    glCallList(HAIRf);
    glCallList(EARf);
    glEndList();
#define cop 30
    glNewList(cop, GL_COMPILE_AND_EXECUTE);
    //glTranslated(-110,180,0);
    glScaled(0.8, 0.8, 0);
    glTranslated(-100, 250, 0);
    glCallList(allf);
    glEndList();
    glFlush();
}
void pro()
{
#define backgro 41
    glNewList(backgro, GL_COMPILE_AND_EXECUTE);
    glBegin(GL_POLYGON);
    glColor3f(0.67, 1.0, 0.37);
    glVertex2f(0, 0);
    glColor3f(0.56, 0.84, 0.0);
    glVertex2f(0, 650);
    glColor3f(0.74, 0.83, 0.0);
    glVertex2f(450, 650);
    glVertex2f(450, 0);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.4, 0.4, 0.4);
    glVertex2f(5, 220);
    glVertex2f(5, 450);
    glVertex2f(445, 450);
    glVertex2f(445, 220);
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.9, 0.2);
    glVertex2f(50, 260);
    glVertex2f(400, 260);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 0.9, 0.2);
    glVertex2f(50, 400);
    glVertex2f(400, 400);
    glEnd();
    glEndList();
    glFlush();
}
void charmander()
{
    float Pi = 3.14;
    int m = 360;
#define EYEd 42
    glNewList(EYEd, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(8 * cos(2 * Pi / m * i) + 158, 15 * sin(2 * Pi / m * i) + 240);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.25, 0.6, 0.6);
    for (int i = 0; i < m; i++) {
        glVertex2f(5 * cos(2 * Pi / m * i) + 158, 13 * sin(2 * Pi / m * i) + 240);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(5 * cos(2 * Pi / m * i) + 158, 10.5 * sin(2 * Pi / m * i) + 244);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(3 * cos(2 * Pi / m * i) + 158, 4.5 * sin(2 * Pi / m * i) + 246);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(148, 228);
    glVertex2i(168, 228);
    glVertex2i(168, 223);
    glVertex2i(148, 223);
    glEnd();
    glEndList();

#define HEADd 43
    glNewList(HEADd, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(43 * cos(2 * Pi / m * i) + 180, 52 * sin(2 * Pi / m * i) + 232);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(45 * cos(2 * Pi / m * i) + 180, 35 * sin(2 * Pi / m * i) + 215);
    }
    glEnd();

    glBegin(GL_POLYGON);//????????
    glColor3f(1.0, 0.2, 0.3);
    for (int r = 0; r < 36; r++) {
        for (int i = 190; i < 350; i++) {
            glVertex2i(r * cos(i * Pi / 180) + 181, r * sin(i * Pi / 180) + 223);
        }
    }
    glEnd();

    glBegin(GL_POLYGON);//???
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 220; i < 320; i++) {
        glVertex2i(55 * cos(i * Pi / 180) + 181.5, 55 * sin(i * Pi / 180) + 256);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(220, 227);
    glVertex2i(138, 227);
    glVertex2i(138, 210);
    glVertex2i(220, 210);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2i(160, 205);
    glVertex2i(167, 203);
    glVertex2i(164, 198);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2i(200, 204.5);
    glVertex2i(193, 202.5);
    glVertex2i(197, 197.5);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2i(198, 192.5);
    glVertex2i(191, 189.5);
    glVertex2i(195, 194.5);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2i(163, 193.5);
    glVertex2i(167, 194.5);
    glVertex2i(170, 189.5);
    glEnd();

    glLineWidth(2.3f);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(169, 248);
    glVertex2f(168, 250);
    glVertex2f(167, 252);
    glVertex2f(166, 254);
    glVertex2f(165.5, 255);
    glVertex2f(164.5, 256);
    glVertex2f(163.5, 256.5);
    glVertex2f(163, 256.8);
    glVertex2f(162, 257);
    glEnd();

    glLineWidth(2.3f);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(193, 248);
    glVertex2f(194, 250);
    glVertex2f(195, 252);
    glVertex2f(196, 254);
    glVertex2f(196.5, 255);
    glVertex2f(197.5, 256);
    glVertex2f(198.5, 256.5);
    glVertex2f(199, 256.8);
    glVertex2f(200, 257);
    glEnd();

    glLineWidth(2.3f);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(174, 205);
    glVertex2f(174, 210);
    glVertex2f(187, 205);
    glVertex2f(187, 210);
    glEnd();

    glCallList(EYEd);
    glTranslatef(45.0, 0.0, 0.0);
    glCallList(EYEd);
    glEndList();

#define LEGS 44
    glNewList(LEGS, GL_COMPILE);
    glBegin(GL_POLYGON);//????????
    glColor3f(1.0, 0.5, 0.0);
    for (int r = 0; r < 17; r++) {
        for (int i = 160; i < 340; i++) {
            glVertex2i(r * cos(i * Pi / 180) + 113, (2.8) * r * sin(i * Pi / 180) + 105);
        }
    }
    glEnd();

    glBegin(GL_POLYGON);//????????
    glColor3f(1.0, 0.5, 0.0);
    for (int r = 0; r < 17; r++) {
        for (int i = 202; i < 380; i++) {
            glVertex2i(r * cos(i * Pi / 180) + 160, (2.8) * r * sin(i * Pi / 180) + 105);
        }
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(15 * cos(2 * Pi / m * i) + 105, 8 * sin(2 * Pi / m * i) + 64.5);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(15 * cos(2 * Pi / m * i) + 168, 8 * sin(2 * Pi / m * i) + 64);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(84, 60);
    glVertex2i(92, 60);
    glVertex2i(92, 68);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(92, 56);
    glVertex2i(101, 57);
    glVertex2i(101, 65);
    glVertex2i(94, 61);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(103, 53);
    glVertex2i(110, 55);
    glVertex2i(108, 64);
    glVertex2i(103, 59);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(178, 59);
    glVertex2i(188, 58);
    glVertex2i(181, 67);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(170, 55);
    glVertex2i(171, 65);
    glVertex2i(178, 57);
    glVertex2i(175, 54);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(159, 56);
    glVertex2i(161, 64);
    glVertex2i(168, 56);
    glVertex2i(165, 53);
    glEnd();

    glEndList();

#define HANDd 45
    glNewList(HANDd, GL_COMPILE);
    glBegin(GL_POLYGON);//?
    glColor3f(1.0, 0.5, 0.0);
    for (int r = 0; r < 15; r++) {
        for (int i = -80; i < 100; i++) {
            glVertex2i(4 * r * cos(i * Pi / 180) + 170, r * sin(i * Pi / 180) + 174);
        }
    }
    glEnd();

    glBegin(GL_POLYGON);//?
    glColor3f(1.0, 0.5, 0.0);
    for (int r = 0; r < 15; r++) {
        for (int i = 80; i < 260; i++) {
            glVertex2i(4 * r * cos(i * Pi / 180) + 101, r * sin(i * Pi / 180) + 174);
        }
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(44, 176);
    glVertex2i(49, 186);
    glVertex2i(54, 180);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(46, 178);
    glVertex2i(39, 173);
    glVertex2i(46, 169);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(45, 171);
    glVertex2i(50, 161);
    glVertex2i(54, 166);
    glEnd();


    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(213, 180);
    glVertex2i(220, 185);
    glVertex2i(223, 176);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(223, 177);
    glVertex2i(229, 173);
    glVertex2i(223, 169);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2i(215, 166);
    glVertex2i(222, 162);
    glVertex2i(224, 171);
    glEnd();
    glEndList();

#define BODYd 46
    glNewList(BODYd, GL_COMPILE);
    glLineWidth(10.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 300; i > 150; i--) {
        glVertex2i(75 * cos(i * Pi / 180) + 145, 75 * sin(i * Pi / 180) + 168);
    }
    for (int i = 180; i < 270; i++) {
        glVertex2i(100 * cos(i * Pi / 180) + 175, 100 * sin(i * Pi / 180) + 200);
    }
    glVertex2i(150, 150);
    glEnd();

    glLineWidth(18.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 190; i < 260; i++) {
        glVertex2i(88 * cos(i * Pi / 180) + 160, 88 * sin(i * Pi / 180) + 184);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(52 * cos(2 * Pi / m * i) + 181, 61 * sin(2 * Pi / m * i) + 140);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(35 * cos(2 * Pi / m * i) + 181, 50 * sin(2 * Pi / m * i) + 136);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    for (int r = 0; r < 20; r++) {
        for (int i = 210; i < 380; i++) {
            glVertex2i(r * cos(i * Pi / 180) + 70, 1.7 * r * sin(i * Pi / 180) + 225);
        }
    }
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(56, 209);
    glVertex2i(57, 241);
    glVertex2i(70, 218);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(60, 216);
    glVertex2i(64, 250);
    glVertex2i(78, 224);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(73, 233);
    glVertex2i(86, 259);
    glVertex2i(85, 228);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(62, 215);
    glVertex2i(74, 263);
    glVertex2i(83, 226);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(88, 228);
    glVertex2i(92, 244);
    glVertex2i(82, 239);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(53, 209);
    glVertex2i(50, 224);
    glVertex2i(66, 215);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    for (int r = 0; r < 12; r++) {
        for (int i = 210; i < 360; i++) {
            glVertex2i(1.2 * r * cos(i * Pi / 180) + 75, 2.2 * r * sin(i * Pi / 180) + 216);
        }
    }
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(88.5, 214);
    glVertex2i(85.5, 228);
    glVertex2i(78, 210);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(72, 210);
    glVertex2i(80, 233);
    glVertex2i(83, 215);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(65, 202);
    glVertex2i(73, 235);
    glVertex2i(77, 215);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(63, 202);
    glVertex2i(65, 228);
    glVertex2i(73, 210);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2i(58, 218);
    glVertex2i(63, 202);
    glVertex2i(68, 210);
    glEnd();
    glCallList(HEADd);
    glCallList(LEGS);
    glCallList(HANDd);
    glEndList();
#define ALLd 47
    glNewList(ALLd, GL_COMPILE_AND_EXECUTE);
    glScaled(0.4, 0.4, 0);
    glTranslated(300, 0, 0);
    glCallList(BODYd);
    glEndList();
    glFlush();
}
void background2() {
    float x, y;
    glBegin(GL_POLYGON);
    glColor3f(0.003, 0.183, 0.324);
    glVertex2f(0, 0);
    glVertex2f(0, 650);
    glVertex2f(450, 650);
    glVertex2f(450, 0);
    glEnd();
    for (int i = 0;i < 30;i++) {
        x = rand() % 450;//range from -200 to 200
        y = rand() % 650;//range from -200 to 200
        glBegin(GL_POLYGON);
        glColor3f(0.3, 0.472, 0.617);
        for (int j = 0;j < 360;j++)
        {
            glVertex2f(x + 1.5 * cos(2 * M_PI / 360 * j), y + 1.5 * sin(2 * M_PI / 360 * j));
        }
        glEnd();
    }
}
void plent1() {
    int x, y;
    for (int i = 0;i < 35;i++) {
        x = rand() % 128;//range from -200 to 200
        y = rand() % 130 + 390;//range from -200 to 200
        glBegin(GL_POLYGON);
        glColor3f(0.984, 0.988, 0.0015);
        //glColor3f(0.3,0.472,0.617);
        for (int j = 0;j < 360;j++)
        {
            glVertex2f(x + 2 * cos(2 * M_PI / 360 * j), y + 2 * sin(2 * M_PI / 360 * j));
        }
        glEnd();
    }
    glBegin(GL_POLYGON);
    glColor3f(0.843, 0.351, 0.292);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(60 + 55 * cos(2 * M_PI / 360 * i), 450 + 55 * sin(2 * M_PI / 360 * i));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.898, 0.492, 0.144);
    glVertex2f(45, 477);
    for (int i = 50;i < 70;i++)
    {
        glVertex2f(60 + 55 * cos(2 * M_PI / 360 * i), 450 + 55 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.898, 0.492, 0.144);
    glVertex2f(54, 454);
    for (int i = 190;i < 210;i++)
    {
        glVertex2f(60 + 55 * cos(2 * M_PI / 360 * i), 450 + 55 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.898, 0.492, 0.144);
    glVertex2f(45, 420);
    for (int i = 350;i < 370;i++)
    {
        glVertex2f(60 + 55 * cos(2 * M_PI / 360 * i), 450 + 55 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
}
void plent2() {
    int x, y;
    for (int i = 0;i < 25;i++) {
        x = rand() % 128 + 350;//range from -200 to 200
        y = rand() % 130 + 540;//range from -200 to 200
        glBegin(GL_POLYGON);
        glColor3f(0.984, 0.988, 0.0015);
        for (int j = 0;j < 360;j++)
        {
            glVertex2f(x + 2 * cos(2 * M_PI / 360 * j), y + 2 * sin(2 * M_PI / 360 * j));
        }
        glEnd();
    }
    glBegin(GL_POLYGON);
    glColor3f(0.871, 0.773, 0.574);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(390 + 45 * cos(2 * M_PI / 360 * i), 590 + 45 * sin(2 * M_PI / 360 * i));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.707, 0.558, 0.468);
    glVertex2f(390, 610);
    for (int i = 120;i < 140;i++)
    {
        glVertex2f(390 + 45 * cos(2 * M_PI / 360 * i), 590 + 45 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.707, 0.558, 0.468);
    glVertex2f(400, 590);
    for (int i = 340;i < 370;i++)
    {
        glVertex2f(390 + 45 * cos(2 * M_PI / 360 * i), 590 + 45 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.707, 0.558, 0.468);
    glVertex2f(390, 570);
    for (int i = 200;i < 220;i++)
    {
        glVertex2f(390 + 45 * cos(2 * M_PI / 360 * i), 590 + 45 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
}
void rocket() {
    double buttom[][2] = {
        {299.3,136.29},{309.14,152.72},{333.75,158.41},{396.68,121.27},{403.28,96.43},{393.57,80.1},{299.69,136.13},
        {324.66,107.01},{321.53,104.93},{317.36,102.63},{315.07,100.76},{312.98,99.3},{311.52,96.8},{310.48,95.55},{309.85,94.51},{311.11,92.84},{314.23,91.59},{317.36,89.51},{318.82,88.47},{351.97,68.88},{354.05,70.75},{354.89,73.05},{354.89,76.8},{356.14,81.17},{356.14,83.05},{356.14,86.17},{356.14,87.63},
    };
    glBegin(GL_POLYGON);
    glColor3f(0.593, 0.703, 0.804);
    for (int i = 0;i < 7;i++)
    {
        glVertex2f(buttom[i][0], buttom[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.593, 0.703, 0.804);
    for (int i = 7;i < 27;i++)
    {
        glVertex2f(buttom[i][0], buttom[i][1]);
    }
    glEnd();
    float wing[][2]{
        {387.91,70.51},{391.43,76.31},{410.91,107.37},{413.8,111.1},{415.88,111.72},{417.54,112.13},{420.02,113.17},{424.16,112.96},{426.65,112.54},{428.1,110.47},{430.79,107.37},{431.83,105.51},{431.83,103.02},{431.83,99.91},{431.83,95.77},{409.04,56.64},
        {397.44,84.8},{417.83,71.69},{409.06,57.12},{387.42,69.18},
    };
    glColor3f(0.824, 0.886, 0.937);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 16;i++)
    {
        glVertex2f(wing[i][0], wing[i][1]);
    }
    glEnd();
    glColor3f(0.937, 0.3, 0.304);
    glBegin(GL_POLYGON);
    for (int i = 16;i < 20;i++)
    {
        glVertex2f(wing[i][0], wing[i][1]);
    }
    glEnd();
    glTranslated(-117, 68, 0);
    glColor3f(0.824, 0.886, 0.937);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 16;i++)
    {
        glVertex2f(wing[i][0], wing[i][1]);
    }
    glEnd();
    glColor3f(0.937, 0.3, 0.304);
    glBegin(GL_POLYGON);
    for (int i = 16;i < 20;i++)
    {
        glVertex2f(wing[i][0], wing[i][1]);
    }
    glEnd();
    glTranslated(117, -68, 0);
    float body[][2]{
        {309.7,117.43},{347.39,181.1},{352.06,187.1},{353.06,190.77},{356.73,195.1},{359.73,199.1},{363.4,204.44},{368.07,209.44},{373.41,216.77},{378.75,220.11},{380.41,224.44},{431.45,192.1},{429.12,184.43},{427.78,179.1},{423.78,169.1},{421.44,161.43},{418.11,157.76},{414.44,149.1},{410.77,142.76},{406.77,138.76},{402.76,131.76},{401.09,127.43},{392.42,114.09},{388.42,106.76},{383.42,99.09},{371.41,80.42},
        {321.37,137.76},{309.03,116.42},{369.74,79.09},{384.08,99.09},
        {380.52,223.25},{383.65,225.48},{386.67,227.44},{390.2,230.31},{394.25,233.32},{400.27,236.85},{402.11,238.42},{405.25,239.99},{409.17,241.95},{412.84,243.52},{415.45,245.35},{418.07,246.53},{422.13,248.1},{424.61,248.1},{428.41,248.1},{431.81,245.22},{432.46,242.34},{433.25,238.03},{433.64,235.02},{434.16,230.71},{434.69,225.61},{434.3,222.34},{434.3,218.81},{434.03,213.44},{433.51,207.95},{433.12,202.98},{433.25,197.62},{432.59,194.74},{431.15,191.74},
    };
    glColor3f(0.824, 0.886, 0.937);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 26;i++)
    {
        glVertex2f(body[i][0], body[i][1]);
    }
    glEnd();
    glColor3f(0.937, 0.3, 0.304);
    glBegin(GL_POLYGON);
    for (int i = 26;i < 30;i++)
    {
        glVertex2f(body[i][0], body[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 30;i < 59;i++)
    {
        glVertex2f(body[i][0], body[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(383 + 23 * cos(2 * M_PI / 360 * i), 168 + 23 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
    glColor3f(0.4375, 0.714, 0.894);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(383 + 12 * cos(2 * M_PI / 360 * i), 168 + 12 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
    float fire[][2]{
        {385.2,42.69},{385.14,44.99},{385.3,48.17},{385.41,50.36},{385.25,52.71},{385.61,54.54},{386.03,56.52},{386.55,57.72},{388.12,59.39},{389.74,61.01},{390.73,61.27},{392.92,61.48},{395.69,61.42},{397.2,60.07},{398.93,58.97},{399.92,57.56},{400.33,55.42},{400.07,53.81},{398.66,51.72},{397.52,50.21},{396.21,48.9},{395.17,47.86},{393.13,46.66},{392.19,45.88},{390.47,44.99},{388.96,44.16},{387.6,43.27},{386.81,42.9},{385.35,42.12},
        {292.12,19.48},{291.98,25.1},{291.98,31.93},{292.39,39.15},{292.79,42.9},{293.73,48.79},{293.73,53.88},{295.06,58.96},{296.27,62.17},{297.34,65.12},{298.41,67.26},{300.56,69.4},{302.03,71.41},{305.11,74.89},{308.86,76.23},{313.41,77.7},{317.56,77.03},{323.32,76.23},{326.94,74.89},{329.88,72.21},{332.96,69.27},{334.57,65.79},{336.04,61.5},{336.44,58.69},{336.44,56.69},{336.58,52.54},{335.37,49.19},{333.9,45.58},{331.35,42.9},{328.28,40.76},{325.6,37.15},{321.58,34.87},{317.7,31.66},{312.34,28.98},{305.91,25.64},{302.43,23.63},{298.14,20.55},{294.53,19.75},{292.39,19.75},
        {307.92,48.26},{308.19,55.35},{308.72,60.97},{309.13,64.98},{310.2,68.33},{313.28,71.94},{315.15,74.49},{317.83,75.69},{321.31,75.42},{325.6,73.55},{327.47,71.28},{328.94,69.27},{329.61,65.39},{329.61,62.44},{328.54,59.36},{325.2,55.21},{321.71,53.47},{317.96,51.47},{315.02,49.06},{310.73,47.32},{309.13,46.78},
    };
    glColor3f(0.964, 0.585, 0.281);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 29;i++)
    {
        glVertex2f(fire[i][0], fire[i][1]);
    }
    glEnd();
    glTranslated(-117, 68, 0);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 29;i++)
    {
        glVertex2f(fire[i][0], fire[i][1]);
    }
    glEnd();
    glTranslated(117, -68, 0);

    glBegin(GL_POLYGON);
    for (int i = 29;i < 68;i++)
    {
        glVertex2f(fire[i][0], fire[i][1]);
    }
    glEnd();

    glColor3f(0.976, 0.628, 0.328);
    glBegin(GL_POLYGON);
    for (int i = 68;i < 89;i++)
    {
        glVertex2f(fire[i][0], fire[i][1]);
    }
    glEnd();

}
void timer(int t)
{

    if (!halt)
    {
        time_count++;
        if (time_count > 50)
        {
            _time++;
            time_count = 0;
            if (_time % 5 == 0)
            {
                score++;
            }
            if (_time == 8) {
                if (level == 4) {
                    page = level + 9;
                    resetpar();
                    glutSwapBuffers();
                    glutPostRedisplay();
                }
                else {
                    page = level + 2;
                    resetpar();
                    glutSwapBuffers();
                    glutPostRedisplay();
                }
            }
        }


    }

    glutPostRedisplay();
    glutTimerFunc(35, timer, 0);
}
void Score()
{
    glPushMatrix();
    glLoadIdentity();
    Introdution = "Score: ";
    stringstream ss;
    ss << score;
    string str = ss.str();
    Introdution = Introdution + str;
    glColor3f(0.0, 0.0, 1.0);
    glRasterPos2f(1.6, 1.7);
    for (int i = 0; i < Introdution.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Introdution[i]);
    }
    glPopMatrix();
}
void Timer()
{
    int hours, min, sec, sec_ten, sec_one;
    glPushMatrix();
    glLoadIdentity();
    sec = _time % 60;
    sec_one = sec % 10;
    sec_ten = sec / 10;
    min = (_time / 60) % 60;
    hours = _time / 3600;
    Introdution = "Time: ";
    stringstream ss;
    string str, str1;
    if (hours > 0)
    {
        ss << hours << ":";
    }
    if (min > 0)
    {
        ss << min << ":";
    }
    ss << sec_ten;
    ss << sec_one;
    str = ss.str();
    Introdution = Introdution + str;
    glColor3f(0.0, 0.0, 1.0);
    glRasterPos2f(1.6, 1.5);
    for (int i = 0; i < Introdution.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Introdution[i]);
    }
    glPopMatrix();
}
void drawString(const char* str, int x)
{
    static bool FirstCall = 1;
    static GLuint lists;
    int sizes;
    sizes = x;
    const char* face = "Constantia";
    if (FirstCall)
    {
        HFONT hFont = CreateFontA(sizes, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
        HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
        DeleteObject(hOldFont);
        lists = glGenLists(MAX_CHAR);
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
        FirstCall = 0;
    }
    for (; *str != '\0'; ++str)
        glCallList(lists + *str);
}
void tnr(GLfloat x, GLfloat y, GLfloat theta)
{
    glTranslatef(x, y, 0);
    glRotatef(theta, 0, 0, 1);
}
void rnt(GLfloat x, GLfloat y, GLfloat theta)
{
    glRotatef(theta, 0, 0, 1);
    glTranslatef(x, y, 0);
}
void background4()
{
    int n = 3600;
    int N = 0;
    float PI = 3.1415926f;
    float R = 100, r;
    float x, y, j, _a, b, k;
    //glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.
    glColor3f(0.0, 0.0, 1.0);      // Set line segment color to red.
#define moon 17
    glNewList(moon, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.4, 0.5, 0.47);
    for (int i = 290;i < 330;i++)
        glVertex2f(25 * cos(i * PI / 180) + 78, 25 * sin(i * PI / 180) + 98);
    for (int i = 220;i > 130;i--)
        glVertex2f(5 * cos(i * PI / 180) + 104, 5 * sin(i * PI / 180) + 90);
    for (int i = 340;i < 380;i++)
        glVertex2f(20 * cos(i * PI / 180) + 85, 25 * sin(i * PI / 180) + 105);
    for (int i = 200;i > 150;i--)
        glVertex2f(6 * cos(i * PI / 180) + 110, 6 * sin(i * PI / 180) + 117);
    for (int i = 370;i < 436;i++)
        glVertex2f(25 * cos(i * PI / 180) + 80, 25 * sin(i * PI / 180) + 115);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 290;i < 330;i++)
        glVertex2f(25 * cos(i * PI / 180) + 78, 25 * sin(i * PI / 180) + 98);
    for (int i = 220;i > 130;i--)
        glVertex2f(5 * cos(i * PI / 180) + 104, 5 * sin(i * PI / 180) + 90);
    for (int i = 340;i < 380;i++)
        glVertex2f(20 * cos(i * PI / 180) + 85, 25 * sin(i * PI / 180) + 105);
    for (int i = 200;i > 150;i--)
        glVertex2f(6 * cos(i * PI / 180) + 110, 6 * sin(i * PI / 180) + 117);
    for (int i = 370;i < 436;i++)
        glVertex2f(25 * cos(i * PI / 180) + 80, 25 * sin(i * PI / 180) + 115);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.4 * 2 * cos(i * PI / 180) + 90, 0.2 * 8 * sin(i * PI / 180) + 108);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 290;i < 440;i++)
        glVertex2f(0.4 * 2 * cos(i * PI / 180) + 90, 0.8 * 2 * sin(i * PI / 180) + 120);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 360;i > 50;i--)
    {
        x = 2 * cos(i * PI / 180);
        y = 2 * sin(i * PI / 180);
        _a = x * cos(30 * PI / 180) + y * sin(30 * PI / 180);
        b = x * sin(30 * PI / 180) + y * cos(30 * PI / 180);
        glVertex2f(_a + 95, b + 85);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 330;i > 160;i--)
        glVertex2f(0.4 * 3 * cos(i * PI / 180) + 88, 0.8 * 3 * sin(i * PI / 180) + 135);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.4 * 5 * cos(i * PI / 180) + 98, 0.8 * 5 * sin(i * PI / 180) + 105);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 290;i < 440;i++)
        glVertex2f(0.4 * 8 * cos(i * PI / 180) + 86, 0.8 * 8 * sin(i * PI / 180) + 90);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i < 360;i++)
    {
        x = 3 * cos(i * PI / 180);
        y = 3 * sin(i * PI / 180);
        _a = x * cos(150 * PI / 180) + y * sin(150 * PI / 180);
        b = x * sin(150 * PI / 180) + y * cos(150 * PI / 180);
        glVertex2f(_a + 98, b + 125);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 250;i > 120;i--)
        glVertex2f(0.4 * 2 * cos(i * PI / 180) + 102, 0.8 * 2 * sin(i * PI / 180) + 120);
    for (int i = 358;i < 455;i++)
        glVertex2f(8 * cos(i * PI / 180) + 94, 0.9 * 8 * sin(i * PI / 180) + 122);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.25, 0.24);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.4 * 5 * cos(i * PI / 180) + 98, 0.8 * 5 * sin(i * PI / 180) + 105);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 290;i < 440;i++)
        glVertex2f(0.4 * 8 * cos(i * PI / 180) + 86, 0.8 * 8 * sin(i * PI / 180) + 90);
    glEnd();
    glEndList();
#define moonn 18
    glNewList(moonn, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.7, 0.6, 0.55);
    for (int i = 290;i < 330;i++)
        glVertex2f(25 * cos(i * PI / 180) + 78, 25 * sin(i * PI / 180) + 98);
    for (int i = 220;i > 130;i--)
        glVertex2f(5 * cos(i * PI / 180) + 104, 5 * sin(i * PI / 180) + 90);
    for (int i = 340;i < 380;i++)
        glVertex2f(20 * cos(i * PI / 180) + 85, 25 * sin(i * PI / 180) + 105);
    for (int i = 200;i > 150;i--)
        glVertex2f(6 * cos(i * PI / 180) + 110, 6 * sin(i * PI / 180) + 117);
    for (int i = 370;i < 436;i++)
        glVertex2f(25 * cos(i * PI / 180) + 80, 25 * sin(i * PI / 180) + 115);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 290;i < 330;i++)
        glVertex2f(25 * cos(i * PI / 180) + 78, 25 * sin(i * PI / 180) + 98);
    for (int i = 220;i > 130;i--)
        glVertex2f(5 * cos(i * PI / 180) + 104, 5 * sin(i * PI / 180) + 90);
    for (int i = 340;i < 380;i++)
        glVertex2f(20 * cos(i * PI / 180) + 85, 25 * sin(i * PI / 180) + 105);
    for (int i = 200;i > 150;i--)
        glVertex2f(6 * cos(i * PI / 180) + 110, 6 * sin(i * PI / 180) + 117);
    for (int i = 370;i < 436;i++)
        glVertex2f(25 * cos(i * PI / 180) + 80, 25 * sin(i * PI / 180) + 115);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.4 * 2 * cos(i * PI / 180) + 90, 0.2 * 8 * sin(i * PI / 180) + 108);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 290;i < 440;i++)
        glVertex2f(0.4 * 2 * cos(i * PI / 180) + 90, 0.8 * 2 * sin(i * PI / 180) + 120);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 360;i > 50;i--)
    {
        x = 2 * cos(i * PI / 180);
        y = 2 * sin(i * PI / 180);
        _a = x * cos(30 * PI / 180) + y * sin(30 * PI / 180);
        b = x * sin(30 * PI / 180) + y * cos(30 * PI / 180);
        glVertex2f(_a + 95, b + 85);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 250;i > 120;i--)
        glVertex2f(0.4 * 2 * cos(i * PI / 180) + 102, 0.8 * 2 * sin(i * PI / 180) + 120);
    for (int i = 358;i < 455;i++)
        glVertex2f(8 * cos(i * PI / 180) + 94, 0.9 * 8 * sin(i * PI / 180) + 122);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 330;i > 160;i--)
        glVertex2f(0.4 * 3 * cos(i * PI / 180) + 88, 0.8 * 3 * sin(i * PI / 180) + 135);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.4 * 5 * cos(i * PI / 180) + 98, 0.8 * 5 * sin(i * PI / 180) + 105);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 290;i < 440;i++)
        glVertex2f(0.4 * 8 * cos(i * PI / 180) + 86, 0.8 * 8 * sin(i * PI / 180) + 90);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i < 360;i++)
    {
        x = 3 * cos(i * PI / 180);
        y = 3 * sin(i * PI / 180);
        _a = x * cos(150 * PI / 180) + y * sin(150 * PI / 180);
        b = x * sin(150 * PI / 180) + y * cos(150 * PI / 180);
        glVertex2f(_a + 98, b + 125);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.25, 0.24);
    for (int i = 0;i < 360;i++)
        glVertex2f(0.4 * 5 * cos(i * PI / 180) + 98, 0.8 * 5 * sin(i * PI / 180) + 105);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 290;i < 440;i++)
        glVertex2f(0.4 * 8 * cos(i * PI / 180) + 86, 0.8 * 8 * sin(i * PI / 180) + 90);
    glEnd();
    glEndList();
#define star 19
    glNewList(star, GL_COMPILE);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(90, 500);
    glVertex2f(150, 490);
    glVertex2f(180, 450);
    glVertex2f(220, 420);
    glVertex2f(210, 380);
    glVertex2f(280, 360);
    glVertex2f(310, 400);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINE_STRIP);
    glColor4f(1.0, 1.0, 1.0, 0.2);
    glVertex2f(90, 500);
    glVertex2f(150, 490);
    glVertex2f(180, 450);
    glVertex2f(220, 420);
    glVertex2f(210, 380);
    glVertex2f(280, 360);
    glVertex2f(310, 400);
    glEnd();
    glEndList();
#define planet 20
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glNewList(planet, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor4f(1.0, 1.0, 1.0, 0.8);
    for (int i = 0;i < 360;i++)
    {
        if (i > 0 && i < 180)
            glColor3f(0.5, 0.3, 0);
        if (i > 180)
            glColor3f(0.54, 0.35, 0.16);
        glVertex2f(35 * cos(i * PI / 180) + 100, 35 * sin(i * PI / 180) + 400);
    }
    glEnd();
    glLineWidth(5.0f);
    glBegin(GL_LINE_STRIP);
    glColor4f(0.6, 0.3, 0.2, 0.8);
    for (int i = 37;i < 310;i++)
    {
        x = 0.4 * 50 * cos(i * PI / 180);
        y = 50 * sin(i * PI / 180);
        _a = x * cos(90 * PI / 180) + y * sin(80 * PI / 180);
        b = x * sin(90 * PI / 180) + y * cos(80 * PI / 180);
        glVertex2f(_a + 101, b + 396);
    }
    glEnd();
    glEndList();
#define planet1 21
    glNewList(planet1, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.5, 0.7);
    for (int i = 10;i < 190;i++)
    {
        glVertex2f(35 * cos(i * PI / 180) + 360, 35 * sin(i * PI / 180) + 180);
    }
    glColor3f(0.5, 0.8, 0.9);
    for (int i = 200;i < 370;i++)
    {
        glVertex2f(35 * cos(i * PI / 180) + 360, 35 * sin(i * PI / 180) + 180);
    }
    glEnd();
    glEndList();
#define planet2 22
    glNewList(planet2, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor4f(0.46, 0.9, 0.7, 0.5);
    for (int i = 0;i < 130;i++)
        glVertex2f(50 * cos(i * PI / 180) + 30, 50 * sin(i * PI / 180) + 500);
    glColor3f(0.47, 0.8, 0.8);
    for (int i = 130;i < 160;i++)
        glVertex2f(50 * cos(i * PI / 180) + 30, 50 * sin(i * PI / 180) + 500);
    for (int i = 330;i < 360;i++)
        glVertex2f(50 * cos(i * PI / 180) + 30, 50 * sin(i * PI / 180) + 500);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.9, 0.9);
    for (int i = 150;i < 292;i++)
        glVertex2f(50 * cos(i * PI / 180) + 30, 50 * sin(i * PI / 180) + 500);
    glColor3f(0.68, 0.9, 0.9);
    for (int i = 158;i < 200;i++)
        glVertex2f(50 * cos(i * PI / 180) + 30, 50 * sin(i * PI / 180) + 500);
    for (int i = 290;i < 332;i++)
        glVertex2f(50 * cos(i * PI / 180) + 30, 50 * sin(i * PI / 180) + 500);
    glEnd();
    glEndList();
#define stars 23
    glNewList(stars, GL_COMPILE);
    glPointSize(2.0f);
    glColor4f(1.0, 1.0, 1.0, 0.5);
    for (int i = 0;i < 35;i++)
    {
        int j = rand() % 450 + 0;
        int h = rand() % 650 + 0;
        glBegin(GL_POINTS);
        glVertex2f(j, h);
        glEnd();
    }
    glEndList();
#define all 24
    glNewList(all, GL_COMPILE_AND_EXECUTE);
    glBegin(GL_POLYGON);
    glColor3f(0.4, 0.45, 0.6);
    glVertex2f(0, 0);
    glColor3f(0.03, 0.2, 0.36);
    glVertex2f(0, 650);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(450, 650);
    glColor3f(0.03, 0.2, 0.36);
    glVertex2f(450, 0);
    glEnd();
    glCallList(stars);
    glTranslated(120, 180, 0);
    glCallList(planet);
    glTranslated(-120, -180, 0);
    glCallList(planet1);
    glCallList(planet2);
    glTranslated(30, 250, 0);
    glScaled(3, 3, 0);
    glTranslated(-96, -150, 0);
    glCallList(moon);
    glTranslated(96, 150, 0);
    glScaled(0.7, 0.7, 0);
    glTranslated(-30, -250, 0);
    glColor3f(0.7, 0.6, 0.5);
    glTranslated(316, 250, 0);
    glRotated(180, 0, 1, 0);
    glCallList(moonn);
    glRotated(180, 0, 1, 0);
    glTranslated(-316, -250, 0);
    glEndList();
    glFlush();     // Process all OpenGL routines as quickly as possible.
}
void togebi() {

    glLineWidth(0.5);

    float body[236][2] = { {290,604},{317,597},{354,574.5},{374.5,559},{385.5,549.5},{400.5,558.5},{425.5,568.5},{440.5,574},{458,577.5},{472,582},{480.5,582},{481.5,582},{487.5,573.5},{492.5,559},{494.5,551.5},{495,548},{496.5,540.5},{497,533},{499,523.5},{501,509.5},{500,498},{504,479},{504,473},{508.5,467},{513,461},{517.5,453},{526,447.5},{530.5,440.5},{534.5,446.5},{540.5,453},{550.5,462},{559.5,471.5},{564,474},{567.5,476.5},{573.5,479.5},{576.5,480.5},{576.5,480.5},{582,480.5},{584.5,480.5},{585,482},{590.5,479},{594,474.5},{595,420},{593.5,414},{592,406.5},{589,398},{586.5,396.5},{584,388},{581.5,379.5},{576.5,374},{571,367},{568.5,363},{566.5,357.5},{569.5,346.5},{572,337.5},{572.5,329},{573,322},{574.5,314},{574,308},{574,299},{573.5,294.5},{572.5,292.5},{572.5,288},{572,285},{571,279},{570.5,270.5},{570,261.5},{567.5,251.5},{563,240},{558.5,228},{554.5,221},{549.5,212.5},{545,205.5},{537,194.5},{530.5,187},{523.5,179.5},{516,173.5},{512.5,169},{505,163},{500.5,160.5},{499.5,157},{504,155},{513.5,151},{515,152},{524,148},{528.5,146},{529.5,145.5},{532,143.5},{536.5,141.5},{539,140},{543.5,137.5},{549.5,132.5},{552,129.5},{554.5,126},{556.5,123},{558.5,119},{560.5,116},{560.5,112},{561,110.5},{559.5,106.5},{558.5,103.5},{555,101},{551.5,97.5},{549,96.5},{547,95},{544.5,92.5},{538,93},{531.5,91},{525,89.5},{520.5,89.5},{515,89.5},{506,90.5},{499,93},{489.5,95},{472,104.5},{454,112},{440.5,119.5},{433.5,124.5},{431,124.5},{425.5,124.5},{421.5,122.5},{415,120.5},{406.5,119},{395.5,116.5},{387,115.5},{373.5,114},{368.5,114},{359.5,113.5},{357.5,114},{348,115.5},{345,113.5},{336.5,115},{330.5,113.5},{322,115.5},{314,117},{306,119},{301.5,112.5},{294,106},{288.5,101.5},{281.5,95.5},{274,89},{270.5,88},{268.5,87.5},{267,88},{260,85.5},{259,83.5},{255,80.5},{250.5,80},{245.5,78},{241.5,77.5},{235.5,76},{206,76},{200.5,77},{194.5,78.5},{189.5,81},{185.5,83},{182,85.5},{180,88.5},{179,93.5},{179,96.5},{179.5,99},{180.5,101},{182.5,103.5},{185,108},{188.5,111.5},{198.5,121.5},{208.5,130.5},{220,137},{234.5,146},{230.5,149.5},{225,154.5},{220,159.5},{212.5,165.5},{204.5,174.5},{196.5,184},{189,193},{182.5,201.5},{178,209.5},{174.5,217.5},{170.5,223.5},{168.5,230},{166,238.5},{163,243},{162,249.5},{158.5,261},{156,269.5},{154.5,277},{154.5,287},{154,295.5},{151,298.5},{146,303.5},{140.5,307.5},{134.5,314.5},{126.5,322},{122.5,328.5},{119.5,336},{114.5,343},{112,349},{108,357.5},{108.5,375.5},{112.5,376.5},{121.5,380},{128.5,381.5},{139.5,382},{152,382},{152.5,388.5},{159.5,417},{149.5,439},{139.5,458.5},{131.5,479},{125.5,496.5},{123,504.5},{122,511.5},{120,518.5},{119.5,527.5},{120,536},{120,539.5},{129.5,541.5},{136.5,541.5},{142,542},{153.5,543},{164.5,542.5},{177,541.5},{187.5,542},{200,539},{210,537.5},{224,535.5},{232,532.5},{242,529},{250,527},{252.5,535},{259,549},{266,567.5},{274.5,589.5},{283,601.5},{290,604}
    };
    float eggshell[107][2] = { {515,454.5},{531,442},{568.5,356.5},{571.5,345},{571.5,339.5},{573.5,329.5},{575,313.5},{576,305},{574.5,290},{573.5,275.5},{572,265.5},{569,254},{564.5,237},{560,225.5},{554,213.5},{549,205.5},{542,196.5},{535,187},{526,178.5},{519,173},{514,168},{508,165},{505,160.5},{499,158},{493.5,153.5},{487,151.5},{481,145},{468.5,139},{459,135},{452.5,131.5},{442.5,127},{431,124},{420,121.5},{405.5,116},{395,115.5},{386.5,113.5},{375.5,113.5},{359.5,113},{340,113.5},{322.5,114.5},{307,117},{294,120},{282,125},{270,129.5},{255,135.5},{247,140},{238.5,147},{231,151},{220.5,159},{212,164.5},{200.5,177.5},{193,188},{185,196.5},{181,205},{176.5,214},{169,226},{161,242},{159.5,252},{156,261.5},{153.5,273.5},{153,281},{152.5,294.5},{188,364},{191.5,370.5},{196,375},{200,381.5},{212,371},{228.5,358},{247,344},{257,356},{261.5,369},{271,383.5},{280.5,379},{290.5,374},{301,364.5},{312,357.5},{326,352.5},{330,357},{337.5,366.5},{343.5,375.5},{348,388.5},{356,402.5},{367.5,398},{375.5,394.5},{380,394.5},{389.5,399.5},{392.5,404},{398.5,404},{403,400},{409,399},{414.5,403},{420,410.5},{424,416.5},{431,411.5},{435.5,406},{443,399},{452,386.5},{461,403.5},{465.5,417},{474,439.5},{485,432},{494.5,427.5},{507,421},{511.5,439},{516,453},{523.5,448},{515,454}
    };
    float blueTri1[13][2] = { {348,287},{392,323.5},{449,343.5},{451.5,310},{449.5,287.5},{447,267.5},{437.5,241},{423.5,218.5},{406,223},{391.5,231.5},{373.5,247.5},{361.5,265.5},{348,287}
    };
    float whiteTri1[10][2] = { {378,280.5},{395,294.5},{428.5,314},{428.5,290},{425.5,277},{421.5,261},{414,245.5},{397.5,257.5},{383,272.5},{378,280.5}
    };
    float blueTri2[22][2] = { {254.5,232},{262.5,228},{275,218.5},{294,205.5},{297.5,203},{315,185.5},{332.5,168},{340,153.5},{331,148.5},{322.5,142},{312,140.5},{297,139.5},{276,138},{267,150},{260.5,158.5},{253,177},{248.5,189.5},{246.5,203.5},{247,213},{248,223},{251.5,229},{254.5,232}
    };
    float whiteTri2[10][2] = { {271,204},{288.5,189.5},{310,166.5},{305,161},{296.5,159.5},{287,159.5},{278.5,160},{274.5,176},{270,196},{271,204}
    };
    float redRec1[16][2] = { {261.5,340},{277.5,340.5},{298.5,327.5},{303,322.5},{301.5,303.5},{299,291.5},{292,275.5},{285,268.5},{272.5,268.5},{267.5,272},{260,277},{245.5,290.5},{245,305},{247.5,317},{253,333},{261.5,340}
    };
    float whiteRec1[12][2] = { {261.5,329.5},{268.5,329},{277,327},{283,320.5},{285,308.5},{283.5,295.5},{279,283.5},{266.5,290},{262,296.5},{262,315},{262,323.5},{261.5,329.5}
    };
    float redRec2[20][2] = { {568,257},{559,269},{551,280.5},{534,263},{511,242.5},{489.5,213},{474,189.5},{471.5,182},{471.5,176.5},{474.5,169},{477.5,161},{486.5,156},{496,157},{511,164.5},{518.5,174},{538.5,193},{553.5,219},{557,225},{563.5,242.5},{568,257}
    };
    float whiteRec2[15][2] = { {555.5,221},{554,234},{546,243.5},{538,234},{528.5,223.5},{511.5,199.5},{501,183},{499,179},{502.5,174},{511.5,170},{514.5,170},{528.5,185},{543,202.5},{551,216},{555.5,221}
    };
    float redTri[13][2] = { {505,377},{502,368.5},{503,356.5},{505.5,344.5},{509,327},{521,335},{528,339.5},{532.5,347.5},{533,352.5},{527,357},{517.5,367},{508.5,376},{505,377}
    };
    float rightHand[25][2] = { {519.5,428.5},{530.5,440.5},{541.5,454.5},{556,468.5},{570,477.5},{575.5,480},{581,482},{585.5,481.5},{589.5,480.5},{593.5,476},{596,470},{594.5,423},{591.5,407},{586,392.5},{577.5,375.5},{570,362},{560,349.5},{553.5,351},{548,354.5},{540.5,360},{532.5,373.5},{528,383.5},{524,397.5},{520.5,414},{519.5,428.5}
    };
    float leftHand[37][2] = { {112,376},{121,379.5},{129.5,381.5},{137,383},{149,383},{158,382.5},{165,380},{174.5,377.5},{183,375},{194,368},{202,363.5},{209,357.5},{213.5,351},{215.5,345},{218.5,338},{222,331.5},{222.5,324.5},{223,316.5},{221.5,303.5},{218,296},{212.5,290.5},{208.5,287.5},{201,282},{195,279},{189,281},{182.5,283},{175.5,285.5},{164.5,291},{156,297},{145,304.5},{135,313},{128.5,320},{122.5,329},{120,336},{115.5,341},{110,350.5},{108,358.5}
    };
    float rightLeg[38][2] = { {499,158.5},{508,154.5},{518.5,151},{527.5,146.5},{536,142},{543,136.5},{550,131.5},{555,125.5},{558.5,119},{561,112},{561.5,107},{559,103.5},{557,101},{554.5,99},{551.5,97.5},{548.5,95},{542.5,92.5},{535.5,91},{530,90.5},{520.5,90.5},{514,91.5},{508.5,91.5},{501.5,93.5},{495.5,95},{486,98},{478.5,100.5},{466,106},{457,110.5},{451,114},{444,117.5},{436.5,122.5},{433,126},{440.5,129},{455,135.5},{469.5,142},{485,149.5},{494,156},{499,158.5}
    };
    float leftLeg[39][2] = { {236,146},{242,142.5},{249,138.5},{254.5,137},{261.5,132.5},{281.5,125.5},{293.5,122},{302.5,120},{305.5,118},{302,110.5},{296,105},{292.5,105},{286.5,98},{280,92.5},{273,88.5},{266.5,86},{259,81.5},{247,79},{240.5,76},{233,76},{106.5,72.5},{106.5,72.5},{207.5,76.5},{199.5,78.5},{193,80},{187.5,83.5},{181.5,87.5},{179,93.5},{179,97},{181,102.5},{184.5,108.5},{189.5,114.5},{197.5,122},{202.5,127.5},{208,131},{214,134.5},{220,138.5},{225.5,141.5},{232,144}
    };
    float rightToe[16][2] = { {521.5,130},{523,128.5},{526,127},{529.5,126},{532,124},{535,122.5},{536.5,121},{539.5,119.5},{541,118.5},{543.5,116.5},{545,114.5},{548,112},{550,109.5},{551.5,105.5},{553.5,102},{554,99}
    };
    float leftToe[14][2] = { {222,112},{218.5,111},{216.5,109.5},{213,108},{210,106},{205.5,104.5},{202.5,101.5},{199,99.5},{197,97},{194.5,94},{192,90.5},{190.5,87.5},{189.5,84.5},{188.5,82.5}
    };
    float rightEyebrow[13][2] = { {292.5,500.5},{289,491},{287,487.5},{285.5,482.5},{282.5,477},{283.5,472.5},{283.5,467},{283.5,460.5},{287,453.5},{289,451},{294.5,448.5},{297.5,445.5},{300.5,445.5}
    };
    float leftEyebrow[13][2] = { {418.5,504.5},{422,496.5},{428.5,491.5},{433,485},{434,481},{436,473.5},{435,468},{434,464},{432,464},{430,462.5},{420,460.5},{419.5,458},{416.5,457}
    };
    float pinkMouth[23][2] = { {367,425.5},{369.5,426},{371.5,426},{375,426},{378,424.5},{381.5,423},{383.5,421},{386,419},{388.5,416},{390,414},{390,412.5},{389,410.5},{387.5,409},{385,408.5},{382.5,408.5},{378.5,409.5},{376.5,410.5},{373.5,413},{371,417},{369,419.5},{367,423},{367,425},{367,425.5}
    };
    float redMouth[37][2] = { {357,467},{361.5,466.5},{365.5,466.5},{371,466.5},{374.5,467.5},{379.5,468},{382,468},{384.5,469},{387,466.5},{387.5,464},{387.5,461.5},{390,457.5},{392,450},{393,446},{393,441},{393.5,437},{394,432},{394.5,428},{394,424},{393,419.5},{391.5,414.5},{389.5,411.5},{388,409},{386,407.5},{383,407.5},{380,408},{378,409},{374.5,411.5},{371,415.5},{367.5,421},{365,428},{362.5,435},{361,442},{359.5,449.5},{357,457.5},{357,462},{357,467}
    };
    float redMouthOutline[9][2] = { {357,467},{361.5,466.5},{365.5,466.5},{371,466.5},{374.5,467.5},{379.5,468},{382,468},{384.5,469},{391.7,472.0}
    };
    float rightBrown[32][2] = { {392.5,510},{395,510},{398,508.5},{400.5,507.5},{404,504.5},{406,501.5},{409.5,496.5},{414.5,489},{417.5,481.5},{419.5,479},{419.5,475.5},{419.5,473.5},{419,470},{418,468.5},{417.5,466.5},{416.5,465.5},{415,465.5},{411.5,465.5},{411,466.5},{408.5,467.5},{406.5,470},{402.5,473.5},{399,478.5},{397.5,481.5},{395.5,485.5},{393,490},{392,495},{390,500},{390,502},{390,504.5},{391,506},{392.5,510}
    };
    float rightBlack[27][2] = { {393.5,509},{397.5,509},{401.5,506.5},{403.5,505},{405,502},{407,499.5},{410.5,495},{413.5,490.5},{416,483.5},{417,482},{415.5,478},{414.5,476},{412.5,474.5},{410,474.5},{407,475.5},{405,477.5},{402.5,480},{399.5,483},{397,485.5},{395.5,487.5},{393.5,491.5},{391,495.5},{390,499.5},{390,502.5},{390,505.5},{391,507},{393.5,509}
    };
    float rightWhite[19][2] = { {392.5,509},{395,509},{399,507},{401.5,505},{404,503},{405.5,499.5},{406.5,495},{406.5,490.5},{405,490},{402.5,489.5},{400,489.5},{397.5,491.5},{395,492.5},{394,496},{392.5,498.5},{391.5,501},{392,505},{393,507},{392.5,509}
    };
    float leftBrown[27][2] = { {309.5,502},{314,500},{316,498},{318,494.5},{320,490.5},{320,487.5},{321,483.5},{321.5,477},{321.5,471},{321.5,465.5},{320.5,460.5},{319,455.5},{318,453.5},{316,453},{312.5,453},{310,454},{309,455},{307.5,457},{306,459},{303,462.5},{303,469},{302.5,479},{303,489},{304,495.5},{306,499.5},{308.5,501},{309.5,502}
    };
    float leftBlack[25][2] = { {309.5,501},{313.5,500},{317.5,497.5},{318,492},{319.5,487},{320.5,483},{320.5,478.5},{321,474},{320.5,470},{318,468},{318,465.5},{316.5,463.5},{315.5,462.5},{314,461.5},{312,461.5},{310,462.5},{308,465},{306.5,467},{305,472},{305,477.5},{304.5,483.5},{304.5,491},{304.5,496.5},{306.5,498.5},{309.5,501}
    };
    float leftWhite[23][2] = { {310.5,500.5},{313.5,498},{315,495},{316.5,493},{317,491},{318,489},{318,485},{316.5,482},{316,479.5},{314,477.5},{312.5,477.5},{311,478},{309.5,478},{308,478.5},{306.5,481},{306,482.5},{305,485.5},{304,489.5},{304,492},{304.5,494},{306.5,495.5},{308,497.5},{310.5,500.5}
    };

#define BODY 1
    glNewList(BODY, GL_COMPILE);
    glColor3f(0.97f, 0.89f, 0.61f);
    glBegin(GL_POLYGON);
    glVertex2f(367, 240);
    for (int i = 0; i < 236; i++) {
        glVertex2f(body[i][0], body[i][1]);
    }
    glVertex2f(367, 300);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 236; i++) {
        glVertex2f(body[i - 1][0], body[i - 1][1]);
        glVertex2f(body[i][0], body[i][1]);
    }
    glEnd();
    glEndList();

#define EGGSHELL 2
    glNewList(EGGSHELL, GL_COMPILE);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(367, 116);
    for (int i = 0; i < 107; i++) {
        glVertex2f(eggshell[i][0], eggshell[i][1]);
    }
    glVertex2f(367, 116);
    glEnd();

    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 107; i++) {
        glVertex2f(eggshell[i - 1][0], eggshell[i - 1][1]);
        glVertex2f(eggshell[i][0], eggshell[i][1]);
    }
    glEnd();
    glEndList();

#define PATTERN 3
    glNewList(PATTERN, GL_COMPILE);
    //blue tri 1
    glColor3f(0.34f, 0.56f, 0.77f);
    glBegin(GL_POLYGON);
    glVertex2f(407, 280);
    for (int i = 0; i < 13; i++) {
        glVertex2f(blueTri1[i][0], blueTri1[i][1]);
    }
    glVertex2f(407, 280);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 13; i++) {
        glVertex2f(blueTri1[i - 1][0], blueTri1[i - 1][1]);
        glVertex2f(blueTri1[i][0], blueTri1[i][1]);
    }
    glEnd();
    //white tri 1
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(407, 280);
    for (int i = 0; i < 10; i++) {
        glVertex2f(whiteTri1[i][0], whiteTri1[i][1]);
    }
    glVertex2f(407, 280);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 10; i++) {
        glVertex2f(whiteTri1[i - 1][0], whiteTri1[i - 1][1]);
        glVertex2f(whiteTri1[i][0], whiteTri1[i][1]);
    }
    glEnd();
    //blue tri 2
    glColor3f(0.34f, 0.56f, 0.77f);
    glBegin(GL_POLYGON);
    glVertex2f(289, 178);
    for (int i = 0; i < 22; i++) {
        glVertex2f(blueTri2[i][0], blueTri2[i][1]);
    }
    glVertex2f(289, 178);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 22; i++) {
        glVertex2f(blueTri2[i - 1][0], blueTri2[i - 1][1]);
        glVertex2f(blueTri2[i][0], blueTri2[i][1]);
    }
    glEnd();
    //white tri 2
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(289, 178);
    for (int i = 0; i < 10; i++) {
        glVertex2f(whiteTri2[i][0], whiteTri2[i][1]);
    }
    glVertex2f(289, 178);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 10; i++) {
        glVertex2f(whiteTri2[i - 1][0], whiteTri2[i - 1][1]);
        glVertex2f(whiteTri2[i][0], whiteTri2[i][1]);
    }
    glEnd();
    //red rec 1
    glColor3f(0.75f, 0.33f, 0.25f);
    glBegin(GL_POLYGON);
    glVertex2f(276, 306);
    for (int i = 0; i < 16; i++) {
        glVertex2f(redRec1[i][0], redRec1[i][1]);
    }
    glVertex2f(276, 306);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 16; i++) {
        glVertex2f(redRec1[i - 1][0], redRec1[i - 1][1]);
        glVertex2f(redRec1[i][0], redRec1[i][1]);
    }
    glEnd();
    //white rec 1
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(271, 306);
    for (int i = 0; i < 12; i++) {
        glVertex2f(whiteRec1[i][0], whiteRec1[i][1]);
    }
    glVertex2f(271, 306);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 12; i++) {
        glVertex2f(whiteRec1[i - 1][0], whiteRec1[i - 1][1]);
        glVertex2f(whiteRec1[i][0], whiteRec1[i][1]);
    }
    glEnd();
    //red rec 2
    glColor3f(0.75f, 0.33f, 0.25f);
    glBegin(GL_POLYGON);
    glVertex2f(533, 203);
    for (int i = 0; i < 20; i++) {
        glVertex2f(redRec2[i][0], redRec2[i][1]);
    }
    glVertex2f(533, 203);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 20; i++) {
        glVertex2f(redRec2[i - 1][0], redRec2[i - 1][1]);
        glVertex2f(redRec2[i][0], redRec2[i][1]);
    }
    glEnd();
    //white rec 2
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(533, 203);
    for (int i = 0; i < 15; i++) {
        glVertex2f(whiteRec2[i][0], whiteRec2[i][1]);
    }
    glVertex2f(533, 203);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 15; i++) {
        glVertex2f(whiteRec2[i - 1][0], whiteRec2[i - 1][1]);
        glVertex2f(whiteRec2[i][0], whiteRec2[i][1]);
    }
    glEnd();
    //red tri
    glColor3f(0.75f, 0.33f, 0.25f);
    glBegin(GL_POLYGON);
    glVertex2f(511, 351);
    for (int i = 0; i < 13; i++) {
        glVertex2f(redTri[i][0], redTri[i][1]);
    }
    glVertex2f(511, 351);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 13; i++) {
        glVertex2f(redTri[i - 1][0], redTri[i - 1][1]);
        glVertex2f(redTri[i][0], redTri[i][1]);
    }
    glEnd();
    glEndList();

#define LIMBS 4
    glNewList(LIMBS, GL_COMPILE);
    //right hand
    glColor3f(0.97f, 0.89f, 0.61f);
    glBegin(GL_POLYGON);
    glVertex2f(558, 417);
    for (int i = 0; i < 25; i++) {
        glVertex2f(rightHand[i][0], rightHand[i][1]);
    }
    glVertex2f(558, 417);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 25; i++) {
        glVertex2f(rightHand[i - 1][0], rightHand[i - 1][1]);
        glVertex2f(rightHand[i][0], rightHand[i][1]);
    }
    glEnd();
    //left hand
    glColor3f(0.97f, 0.89f, 0.61f);
    glBegin(GL_POLYGON);
    glVertex2f(172, 332);
    for (int i = 0; i < 37; i++) {
        glVertex2f(leftHand[i][0], leftHand[i][1]);
    }
    glVertex2f(172, 332);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 37; i++) {
        glVertex2f(leftHand[i - 1][0], leftHand[i - 1][1]);
        glVertex2f(leftHand[i][0], leftHand[i][1]);
    }
    glEnd();
    //right toe
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 16; i++) {
        glVertex2f(rightToe[i - 1][0], rightToe[i - 1][1]);
        glVertex2f(rightToe[i][0], rightToe[i][1]);
    }
    glEnd();
    //left toe
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 14; i++) {
        glVertex2f(leftToe[i - 1][0], leftToe[i - 1][1]);
        glVertex2f(leftToe[i][0], leftToe[i][1]);
    }
    glEnd();
    glEndList();

#define FACE 5
    glNewList(FACE, GL_COMPILE);
    //right eyebrow
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 13; i++) {
        glVertex2f(rightEyebrow[i - 1][0], rightEyebrow[i - 1][1]);
        glVertex2f(rightEyebrow[i][0], rightEyebrow[i][1]);
    }
    glEnd();
    //left eyebrow
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 13; i++) {
        glVertex2f(leftEyebrow[i - 1][0], leftEyebrow[i - 1][1]);
        glVertex2f(leftEyebrow[i][0], leftEyebrow[i][1]);
    }
    glEnd();
    //red mouth
    glColor3f(0.6f, 0.07f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(377, 444);
    for (int i = 0; i < 37; i++) {
        glVertex2f(redMouth[i][0], redMouth[i][1]);
    }
    glVertex2f(377, 444);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 37; i++) {
        glVertex2f(redMouth[i - 1][0], redMouth[i - 1][1]);
        glVertex2f(redMouth[i][0], redMouth[i][1]);
    }
    glEnd();
    //pink mouth
    glColor3f(0.92f, 0.39f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(378, 417);
    for (int i = 0; i < 23; i++) {
        glVertex2f(pinkMouth[i][0], pinkMouth[i][1]);
    }
    glVertex2f(378, 417);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 23; i++) {
        glVertex2f(pinkMouth[i - 1][0], pinkMouth[i - 1][1]);
        glVertex2f(pinkMouth[i][0], pinkMouth[i][1]);
    }
    glEnd();
    //mouth outline
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 9; i++) {
        glVertex2f(redMouthOutline[i - 1][0], redMouthOutline[i - 1][1]);
        glVertex2f(redMouthOutline[i][0], redMouthOutline[i][1]);
    }
    glEnd();
    //right eye brown
    glColor3f(0.57f, 0.37f, 0.21f);
    glBegin(GL_POLYGON);
    glVertex2f(409, 486);
    for (int i = 0; i < 32; i++) {
        glVertex2f(rightBrown[i][0], rightBrown[i][1]);
    }
    glVertex2f(409, 486);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 32; i++) {
        glVertex2f(rightBrown[i - 1][0], rightBrown[i - 1][1]);
        glVertex2f(rightBrown[i][0], rightBrown[i][1]);
    }
    glEnd();
    //right eye black
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(409, 486);
    for (int i = 0; i < 27; i++) {
        glVertex2f(rightBlack[i][0], rightBlack[i][1]);
    }
    glVertex2f(409, 486);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 27; i++) {
        glVertex2f(rightBlack[i - 1][0], rightBlack[i - 1][1]);
        glVertex2f(rightBlack[i][0], rightBlack[i][1]);
    }
    glEnd();
    //right eye white
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(409, 486);
    for (int i = 0; i < 19; i++) {
        glVertex2f(rightWhite[i][0], rightWhite[i][1]);
    }
    glVertex2f(409, 486);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 19; i++) {
        glVertex2f(rightWhite[i - 1][0], rightWhite[i - 1][1]);
        glVertex2f(rightWhite[i][0], rightWhite[i][1]);
    }
    glEnd();
    //left eye brown
    glColor3f(0.57f, 0.37f, 0.21f);
    glBegin(GL_POLYGON);
    glVertex2f(311, 477);
    for (int i = 0; i < 27; i++) {
        glVertex2f(leftBrown[i][0], leftBrown[i][1]);
    }
    glVertex2f(311, 477);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 27; i++) {
        glVertex2f(leftBrown[i - 1][0], leftBrown[i - 1][1]);
        glVertex2f(leftBrown[i][0], leftBrown[i][1]);
    }
    glEnd();
    //left eye black
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(311, 477);
    for (int i = 0; i < 25; i++) {
        glVertex2f(leftBlack[i][0], leftBlack[i][1]);
    }
    glVertex2f(311, 477);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 25; i++) {
        glVertex2f(leftBlack[i - 1][0], leftBlack[i - 1][1]);
        glVertex2f(leftBlack[i][0], leftBlack[i][1]);
    }
    glEnd();
    //left eye white
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(311, 477);
    for (int i = 0; i < 23; i++) {
        glVertex2f(leftWhite[i][0], leftWhite[i][1]);
    }
    glVertex2f(311, 477);
    glEnd();
    glColor3f(0.0, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i < 23; i++) {
        glVertex2f(leftWhite[i - 1][0], leftWhite[i - 1][1]);
        glVertex2f(leftWhite[i][0], leftWhite[i][1]);
    }
    glEnd();
    glEndList();



    glCallList(BODY);
    glCallList(EGGSHELL);
    glCallList(PATTERN);
    glCallList(LIMBS);
    glCallList(FACE);

}
void background_sea() {

    glLineWidth(1);

    float wave1[105][2] = { {1.0,547.74},{15.308,545.55},{34.056,542.16},{55.986,536.88},{74.906,532.32},{93.869,528.06},{109.263,525.42},{123.797,523.08},{139.578,524.1},{151.145,525.87},{163.572,528.06},{168.431,529.23},{177.676,529.23},{188.426,526.29},{197.929,523.5},{205.497,521.31},{215.817,520.71},{226.137,521.46},{234.909,525.12},{244.842,528.66},{257.054,533.22},{269.868,535.26},{278.941,535.71},{284.015,533.34},{292.228,534.66},{300.828,535.71},{309.471,535.71},{319.791,535.71},{328.004,531.45},{334.411,527.19},{338.84,524.55},{341.377,522.63},{348.945,522.48},{353.589,524.55},{361.587,528.81},{367.263,533.22},{374.014,535.71},{385.366,540.69},{393.407,542.31},{403.512,543.78},{411.897,546.27},{421.615,549.66},{432.15,553.02},{448.146,554.49},{456.789,554.07},{464.572,551.55},{230.566,489.51},{230.566,489.51},{230.566,489.51},{230.566,489.51},{463.11,524.85},{450.468,524.25},{442.47,523.95},{438.041,522.63},{429.183,522.33},{420.11,520.29},{410.435,519.27},{401.792,518.67},{392.117,518.22},{385.581,514.86},{378.658,513.24},{368.553,511.32},{361.157,508.53},{346.408,507.66},{335.916,507.66},{323.661,507.96},{315.878,508.38},{304.741,508.98},{297.345,511.62},{288.315,513.69},{283.241,516.03},{278.425,517.8},{273.781,517.95},{265.783,517.95},{257.57,516.33},{248.282,514.11},{239.467,512.37},{233.361,510.75},{224.761,510.15},{219.042,510.15},{210.829,512.07},{195.693,512.64},{185.588,514.41},{175.053,515.43},{166.625,516.9},{160.089,516.9},{152.521,516.18},{143.878,515.16},{138.073,514.41},{131.322,513.96},{123.754,512.07},{118.078,510.15},{111.757,508.68},{101.222,506.04},{95.761,504.87},{84.366,504.15},{73.229,504.15},{60.587,504.15},{55.728,505.59},{41.194,505.89},{34.271,507.06},{24.338,509.13},{11.524,509.13},{4.34299999999999,508.11},{-3.225,506.64}
    };
    float wave2[105][2] = { {1.0,185.74},{15.308,183.55},{34.056,180.16},{55.986,174.88},{74.906,170.32},{93.869,166.06},{109.263,163.42},{123.797,161.08},{139.578,162.1},{151.145,163.87},{163.572,166.06},{168.431,167.23},{177.676,167.23},{188.426,164.29},{197.929,161.5},{205.497,159.31},{215.817,158.71},{226.137,159.46},{234.909,163.12},{244.842,166.66},{257.054,171.22},{269.868,173.26},{278.941,173.71},{284.015,171.34},{292.228,172.66},{300.828,173.71},{309.471,173.71},{319.791,173.71},{328.004,169.45},{334.411,165.19},{338.84,162.55},{341.377,160.63},{348.945,160.48},{353.589,162.55},{361.587,166.81},{367.263,171.22},{374.014,173.71},{385.366,178.69},{393.407,180.31},{403.512,181.78},{411.897,184.27},{421.615,187.66},{432.15,191.02},{448.146,192.49},{456.789,192.07},{464.572,189.55},{230.566,127.51},{230.566,127.51},{230.566,127.51},{230.566,127.51},{463.11,162.85},{450.468,162.25},{442.47,161.95},{438.041,160.63},{429.183,160.33},{420.11,158.29},{410.435,157.27},{401.792,156.67},{392.117,156.22},{385.581,152.86},{378.658,151.24},{368.553,149.32},{361.157,146.53},{346.408,145.66},{335.916,145.66},{323.661,145.96},{315.878,146.38},{304.741,146.98},{297.345,149.62},{288.315,151.69},{283.241,154.03},{278.425,155.8},{273.781,155.95},{265.783,155.95},{257.57,154.33},{248.282,152.11},{239.467,150.37},{233.361,148.75},{224.761,148.15},{219.042,148.15},{210.829,150.07},{195.693,150.64},{185.588,152.41},{175.053,153.43},{166.625,154.9},{160.089,154.9},{152.521,154.18},{143.878,153.16},{138.073,152.41},{131.322,151.96},{123.754,150.07},{118.078,148.15},{111.757,146.68},{101.222,144.04},{95.761,142.87},{84.366,142.15},{73.229,142.15},{60.587,142.15},{55.728,143.59},{41.194,143.89},{34.271,145.06},{24.338,147.13},{11.524,147.13},{4.34299999999999,146.11},{-3.225,144.64}
    };
    float fish1[35][2] = { {278.597,402.66},{283.026,403.11},{284.918,402.96},{286.165,404.43},{287.67,405.45},{288.917,406.62},{290.164,406.77},{290.594,405.15},{291.841,403.83},{292.701,402.21},{294.808,401.34},{297.345,401.19},{298.377,402.21},{300.269,402.21},{301.559,402.51},{300.484,400.59},{299.452,399.57},{299.022,398.1},{299.667,397.53},{299.452,396.63},{297.345,396.93},{295.023,398.25},{295.453,398.4},{291.239,397.65},{288.487,397.23},{288.272,396.21},{287.025,394.74},{284.273,395.91},{283.026,396.78},{281.564,397.53},{277.952,397.38},{276.275,397.38},{275.63,397.38},{275.243,398.7},{274.813,400.32}
    };
    float fish2[35][2] = { {310.992,412.66},{315.936,413.11},{318.048,412.96},{319.44,414.43},{321.12,415.45},{322.512,416.62},{323.904,416.77},{324.384,415.15},{325.776,413.83},{326.736,412.21},{329.088,411.34},{331.92,411.19},{333.072,412.21},{335.184,412.21},{336.624,412.51},{335.424,410.59},{334.272,409.57},{333.792,408.1},{334.512,407.53},{334.272,406.63},{331.92,406.93},{329.328,408.25},{329.808,408.4},{325.104,407.65},{322.032,407.23},{321.792,406.21},{320.4,404.74},{317.328,405.91},{315.936,406.78},{314.304,407.53},{310.272,407.38},{308.4,407.38},{307.68,407.38},{307.248,408.7},{306.768,410.32}
    };
    float fish3[35][2] = { {260.992,411.66},{265.936,412.11},{268.048,411.96},{269.44,413.43},{271.12,414.45},{272.512,415.62},{273.904,415.77},{274.384,414.15},{275.776,412.83},{276.736,411.21},{279.088,410.34},{281.92,410.19},{283.072,411.21},{285.184,411.21},{286.624,411.51},{285.424,409.59},{284.272,408.57},{283.792,407.1},{284.512,406.53},{284.272,405.63},{281.92,405.93},{279.328,407.25},{279.808,407.4},{275.104,406.65},{272.032,406.23},{271.792,405.21},{270.4,403.74},{267.328,404.91},{265.936,405.78},{264.304,406.53},{260.272,406.38},{258.4,406.38},{257.68,406.38},{257.248,407.7},{256.768,409.32}
    };
    float fish4[35][2] = { {255.992,382.66},{260.936,383.11},{263.048,382.96},{264.44,384.43},{266.12,385.45},{267.512,386.62},{268.904,386.77},{269.384,385.15},{270.776,383.83},{271.736,382.21},{274.088,381.34},{276.92,381.19},{278.072,382.21},{280.184,382.21},{281.624,382.51},{280.424,380.59},{279.272,379.57},{278.792,378.1},{279.512,377.53},{279.272,376.63},{276.92,376.93},{274.328,378.25},{274.808,378.4},{270.104,377.65},{267.032,377.23},{266.792,376.21},{265.4,374.74},{262.328,375.91},{260.936,376.78},{259.304,377.53},{255.272,377.38},{253.4,377.38},{252.68,377.38},{252.248,378.7},{251.768,380.32}
    };
    float fish5[35][2] = { {335.992,387.66},{340.936,388.11},{343.048,387.96},{344.44,389.43},{346.12,390.45},{347.512,391.62},{348.904,391.77},{349.384,390.15},{350.776,388.83},{351.736,387.21},{354.088,386.34},{356.92,386.19},{358.072,387.21},{360.184,387.21},{361.624,387.51},{360.424,385.59},{359.272,384.57},{358.792,383.1},{359.512,382.53},{359.272,381.63},{356.92,381.93},{354.328,383.25},{354.808,383.4},{350.104,382.65},{347.032,382.23},{346.792,381.21},{345.4,379.74},{342.328,380.91},{340.936,381.78},{339.304,382.53},{335.272,382.38},{333.4,382.38},{332.68,382.38},{332.248,383.7},{331.768,385.32}
    };
    float fish6[35][2] = { {288.597,382.66},{293.026,383.11},{294.918,382.96},{296.165,384.43},{297.67,385.45},{298.917,386.62},{300.164,386.77},{300.594,385.15},{301.841,383.83},{302.701,382.21},{304.808,381.34},{307.345,381.19},{308.377,382.21},{310.269,382.21},{311.559,382.51},{310.484,380.59},{309.452,379.57},{309.022,378.1},{309.667,377.53},{309.452,376.63},{307.345,376.93},{305.023,378.25},{305.453,378.4},{301.239,377.65},{298.487,377.23},{298.272,376.21},{297.025,374.74},{294.273,375.91},{293.026,376.78},{291.564,377.53},{287.952,377.38},{286.275,377.38},{285.63,377.38},{285.243,378.7},{284.813,380.32}
    };
    float turtel1[27][2] = { {63.48,145.44},{68.94,154.01},{74.92,161.28},{83.49,165.44},{91.81,170.37},{98.05,171.41},{105.84,171.93},{116.24,171.93},{125.07,169.86},{133.13,166.22},{138.07,161.54},{143.78,158.69},{143.78,153.23},{143.52,151.15},{138.59,148.82},{129.23,144.14},{122.47,139.73},{112.34,133.23},{105.84,131.41},{96.23,131.41},{91.55,131.41},{89.47,131.41},{85.31,132.45},{79.59,134.53},{71.02,136.87},{65.3,141.02},{63.48,145.44}
    };
    float turtel2[20][2] = { {66.86,141.02},{62.44,141.02},{56.73,141.02},{55.69,138.69},{54.65,135.83},{52.83,133.75},{52.83,131.15},{52.83,128.3},{54.13,126.22},{57.25,124.14},{61.14,123.36},{63.48,123.36},{66.86,125.18},{71.02,126.48},{73.1,130.12},{77.26,132.45},{80.89,133.49},{77.52,137.13},{73.36,138.43},{66.86,141.02}
    };
    float turtel3[21][2] = { {87.13,132.71},{86.87,129.86},{86.09,124.14},{86.09,118.43},{89.73,114.01},{96.75,109.86},{103.5,107.52},{108.44,105.7},{113.9,103.36},{115.2,105.44},{115.2,109.08},{112.86,112.71},{111.04,116.09},{109.48,118.69},{106.36,120.77},{103.76,121.54},{99.86,123.88},{95.71,129.08},{95.71,133.49},{87.13,133.49},{87.13,132.71}
    };
    float turtel4[27][2] = { {68.94,154.01},{68.94,154.01},{74.92,161.28},{83.49,165.44},{91.81,170.37},{98.05,171.41},{105.84,171.93},{116.24,171.93},{125.07,169.86},{133.13,166.22},{138.07,161.54},{143.78,158.69},{139.11,157.91},{134.69,155.05},{128.97,151.67},{123.77,149.34},{118.84,147},{113.9,144.92},{107.14,142.58},{100.38,141.28},{94.93,141.28},{89.47,141.8},{83.75,142.84},{77,145.7},{70.5,149.08},{68.68,150.38},{68.94,154.01}
    };
    float turtel5[15][2] = { {124.03,139.47},{125.07,135.31},{125.59,132.19},{129.75,131.41},{133.39,130.12},{135.47,129.6},{140.15,125.44},{143,127},{141.7,131.93},{140.93,138.69},{139.37,140.51},{135.47,142.32},{131.31,143.62},{132.87,145.96},{124.03,139.47}
    };
    float turtel6[20][2] = { {84.53,163.36},{86.35,163.1},{88.69,161.54},{88.69,157.91},{88.69,155.05},{91.03,156.61},{93.63,156.61},{98.31,155.83},{100.9,153.75},{102.72,152.71},{104.8,154.53},{107.14,156.87},{110.52,157.13},{113.12,157.65},{115.72,157.65},{114.42,160.5},{114.42,162.32},{114.42,165.96},{117.28,169.34},{120.39,170.89}
    };
    float rock1[30][2] = { {385.94,0.18},{384.15,7.05},{382.06,17.2},{380.56,24.07},{379.37,32.14},{380.26,42.89},{381.76,53.65},{383.85,60.22},{387.73,64.1},{391.32,68.88},{398.49,73.06},{405.37,85.61},{410.45,92.48},{413.74,104.72},{417.92,113.98},{417.92,117.57},{414.33,123.54},{414.33,131.31},{414.33,136.69},{417.02,140.57},{419.12,143.86},{423,149.23},{427.18,152.82},{432.56,153.41},{438.84,153.41},{447.21,153.41},{453.78,153.41},{459.76,152.52},{466.49,-10.87},{385.94,0.18}
    };
    float patrick1[58][2] = { {375.48,32.44},{374.58,36.32},{376.68,40.5},{381.46,54.24},{382.95,57.53},{377.57,62.01},{374.88,64.7},{373.69,67.39},{374.58,70.37},{376.08,71.87},{379.37,75.15},{382.35,76.35},{391.92,79.04},{393.71,82.32},{396.4,85.31},{400.88,90.39},{409.55,100.84},{412.24,105.32},{413.44,106.82},{415.83,108.91},{418.22,108.91},{421.51,106.22},{422.4,101.74},{422.7,86.2},{422.7,83.82},{422.7,80.83},{425.39,77.84},{431.67,75.75},{437.35,74.55},{441.53,74.55},{446.31,73.96},{448.7,72.46},{450.8,70.07},{450.8,66.49},{448.7,63.5},{444.82,59.92},{435.25,55.14},{429.28,51.85},{426.59,50.96},{426.59,45.88},{427.78,42},{430.17,37.51},{429.58,35.13},{428.98,32.14},{426.89,29.15},{423,26.46},{417.02,28.25},{411.05,30.05},{406.26,33.03},{402.08,37.81},{401.48,39.31},{401.18,42.29},{397.6,39.61},{393.41,36.32},{388.63,31.84},{382.35,30.94},{376.68,31.54},{375.48,32.44}
    };
    float patrick2[56][2] = { {393.41,47.07},{391.62,47.67},{389.83,50.66},{389.83,52.45},{391.32,53.05},{392.81,54.54},{393.71,57.53},{393.71,59.92},{393.41,61.41},{390.42,62.61},{388.33,65.89},{386.24,66.79},{386.24,67.98},{388.03,68.58},{389.83,68.88},{390.72,68.28},{394.01,68.28},{396.7,70.37},{397.9,72.76},{401.78,75.15},{406.86,78.44},{408.36,81.43},{409.25,83.22},{410.45,84.41},{411.34,84.41},{412.54,83.52},{412.54,80.53},{410.75,79.63},{412.24,76.05},{413.14,73.66},{416.13,71.57},{420.01,70.07},{421.21,70.07},{423.3,70.07},{424.49,69.78},{424.79,68.58},{423.6,65.59},{420.91,65.29},{418.82,65.29},{416.72,65.29},{414.93,62.61},{414.93,59.02},{417.02,52.75},{417.02,48.57},{416.72,47.97},{415.23,47.37},{413.74,48.57},{411.34,50.06},{408.65,53.05},{405.37,53.65},{403.87,54.54},{402.08,54.54},{397.9,55.14},{397,50.96},{394.91,48.87},{393.11,48.27}
    };
    float rock2[40][2] = { {-5.0,411.51},{2.78,417.48},{2.18,421.96},{2.18,424.95},{6.96,427.04},{9.95,434.81},{9.95,440.78},{9.65,449.44},{13.84,452.43},{24.3,463.48},{28.78,473.34},{37.15,478.42},{43.12,479.91},{45.52,484.09},{50.3,493.95},{53.88,501.42},{58.67,519.64},{65.24,527.71},{66.74,534.28},{69.13,545.33},{68.53,552.5},{65.54,556.08},{62.55,568.03},{60.46,572.81},{58.37,576.69},{58.07,584.46},{63.75,594.02},{62.55,606.27},{62.55,612.84},{58.67,614.03},{55.68,616.12},{52.69,629.57},{45.52,634.64},{35.35,638.23},{28.78,638.23},{18.32,634.94},{13.54,630.46},{9.95,630.46},{-5.69,632.85}
    };
    float coral[101][2] = { {70.44,537.86},{74.62,541.45},{77.31,539.95},{86.58,533.38},{87.47,528},{87.47,523.52},{90.16,520.54},{92.85,520.54},{96.74,524.42},{95.84,528.9},{95.84,532.19},{100.32,530.1},{108.39,530.1},{127.82,534.28},{127.82,539.65},{126.62,540.25},{124.23,539.95},{120.65,539.36},{117.36,539.06},{115.87,541.15},{119.15,545.93},{119.75,551.3},{119.45,553.1},{117.96,553.1},{115.27,553.69},{113.77,553.69},{111.08,550.11},{109.89,547.42},{108.99,544.14},{108.99,542.04},{108.99,539.36},{107.8,539.06},{103.31,538.46},{96.44,538.46},{89.86,542.64},{87.17,546.23},{87.17,550.11},{90.46,554.29},{93.45,561.16},{100.32,564.15},{130.81,574.9},{132.3,580.28},{129,580.8},{127.6,580.2},{128.72,581.18},{126.03,579.68},{124.23,579.68},{122.14,579.68},{121.24,579.68},{120.35,582.07},{121.54,591.03},{121.54,593.42},{121.24,594.02},{119.15,594.02},{116.16,594.62},{114.37,594.32},{112.88,588.94},{114.37,585.66},{114.37,580.28},{114.97,577.29},{114.37,576.1},{112.28,574.6},{107.2,573.41},{98.83,569.53},{94.65,566.84},{89.56,561.76},{88.37,559.67},{87.17,558.47},{85.98,557.58},{82.99,557.58},{80.3,558.47},{79.7,559.67},{78.51,561.76},{78.51,567.14},{78.81,570.42},{80.9,571.62},{87.17,570.12},{89.56,570.12},{91.36,574.31},{91.06,575.8},{88.67,576.1},{86.58,576.1},{84.18,576.1},{83.89,576.4},{82.39,578.79},{85.98,580.58},{87.77,582.97},{88.07,584.76},{88.07,587.15},{85.98,589.24},{82.39,589.84},{78.81,587.45},{72.83,579.68},{69.84,575.5},{68.64,569.23},{68.94,562.66},{71.03,556.08},{71.03,553.99},{71.03,550.71},{63.26,548.02},{70.44,537.86}
    };


#define WAVE_UPPER 1
    glNewList(WAVE_UPPER, GL_COMPILE);
    glColor3f(0.38f, 0.55f, 0.69f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 8; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    for (int i = 90; i < 105; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 7; i < 18; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    for (int i = 80; i < 91; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 16; i < 24; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    for (int i = 70; i < 81; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 23; i < 32; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    for (int i = 60; i < 71; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 31; i < 45; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    for (int i = 50; i < 61; i++) {
        glVertex2f(wave1[i][0], wave1[i][1]);
    }
    glEnd();

    glEndList();

#define WAVE_DOWN 2
    glNewList(WAVE_DOWN, GL_COMPILE);
    glColor3f(0.38f, 0.55f, 0.69f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 8; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    for (int i = 90; i < 105; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 7; i < 18; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    for (int i = 80; i < 91; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 16; i < 24; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    for (int i = 70; i < 81; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 23; i < 32; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    for (int i = 60; i < 71; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 31; i < 45; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    for (int i = 50; i < 61; i++) {
        glVertex2f(wave2[i][0], wave2[i][1]);
    }
    glEnd();

    glEndList();

#define FISH 3
    glNewList(FISH, GL_COMPILE);
    glColor3f(0.17f, 0.34f, 0.52f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 35; i++) {
        glVertex2f(fish1[i][0], fish1[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 35; i++) {
        glVertex2f(fish2[i][0], fish2[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 35; i++) {
        glVertex2f(fish3[i][0], fish3[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 35; i++) {
        glVertex2f(fish4[i][0], fish4[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 35; i++) {
        glVertex2f(fish5[i][0], fish5[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 35; i++) {
        glVertex2f(fish6[i][0], fish6[i][1]);
    }
    glEnd();

    glEndList();

#define TURTEL 4
    glNewList(TURTEL, GL_COMPILE);
    //hands
    glColor3f(0.82f, 0.67f, 0.54f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 21; i++) {
        glVertex2f(turtel3[i][0], turtel3[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 21; i++) {
        glVertex2f(turtel3[i][0], turtel3[i][1]);
    }
    glEnd();
    //legs
    glColor3f(0.82f, 0.67f, 0.54f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 15; i++) {
        glVertex2f(turtel5[i][0], turtel5[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 15; i++) {
        glVertex2f(turtel5[i][0], turtel5[i][1]);
    }
    glEnd();
    //back1
    glColor3f(0.16f, 0.39f, 0.11f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 27; i++) {
        glVertex2f(turtel1[i][0], turtel1[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 27; i++) {
        glVertex2f(turtel1[i][0], turtel1[i][1]);
    }
    glEnd();
    //back2
    glColor3f(0.16f, 0.29f, 0.11f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 27; i++) {
        glVertex2f(turtel4[i][0], turtel4[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 27; i++) {
        glVertex2f(turtel4[i][0], turtel4[i][1]);
    }
    glEnd();
    //head
    glColor3f(0.82f, 0.67f, 0.54f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        glVertex2f(turtel2[i][0], turtel2[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 20; i++) {
        glVertex2f(turtel2[i][0], turtel2[i][1]);
    }
    glEnd();
    //eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4);
    glBegin(GL_LINE_STRIP);
    glVertex2f(61.91, 134.51);
    glVertex2f(59.91, 134.51);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 20; i++) {
        glVertex2f(turtel6[i][0], turtel6[i][1]);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(turtel6[4][0], turtel6[4][1]);
    glVertex2f(80.88, 145.44);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(turtel6[9][0], turtel6[4][1]);
    glVertex2f(102.72, 141.03);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(turtel6[14][0], turtel6[4][1]);
    glVertex2f(123.23, 149.36);
    glEnd();
    glLineWidth(2);
    glEndList();

#define ROCK 5
    glNewList(ROCK, GL_COMPILE);
    //rock1
    glColor3f(0.59f, 0.47f, 0.26f);
    glBegin(GL_POLYGON);
    glVertex2f(450, 0);
    for (int i = 0; i < 30; i++) {
        glVertex2f(rock1[i][0], rock1[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 30; i++) {
        glVertex2f(rock1[i][0], rock1[i][1]);
    }
    glEnd();
    //patrick
    glColor3f(0.67f, 0.07f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2f(402.68, 62.91);
    for (int i = 0; i < 58; i++) {
        glVertex2f(patrick1[i][0], patrick1[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 58; i++) {
        glVertex2f(patrick1[i][0], patrick1[i][1]);
    }
    glEnd();
    glColor3f(0.92f, 0.53f, 0.48f);
    glBegin(GL_POLYGON);
    glVertex2f(402.68, 62.91);
    for (int i = 0; i < 56; i++) {
        glVertex2f(patrick2[i][0], patrick2[i][1]);
    }
    glEnd();
    //coral
    glColor3f(0.83f, 0.35f, 0.35f);
    glBegin(GL_POLYGON);
    glVertex2f(83.79, 547.64);
    for (int i = 0; i < 13; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(98.44, 536.9);
    for (int i = 12; i < 20; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(113.38, 534.6);
    for (int i = 19; i < 30; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(63.7, 539.8);
    for (int i = 30; i < 40; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(99.04, 563.55);
    for (int i = 38; i < 50; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(119.7, 573.4);
    for (int i = 50; i < 60; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(100.5, 565.3);
    for (int i = 60; i < 70; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(65.6, 540.6);
    for (int i = 65; i < 75; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(73.6, 569.8);
    for (int i = 74; i < 92; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(76.6, 555.5);
    for (int i = 89; i < 101; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(78.7, 543.8);
    for (int i = 63; i < 75; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(74.5, 545.0);
    for (int i = 65; i < 75; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 101; i++) {
        glVertex2f(coral[i][0], coral[i][1]);
    }
    glEnd();
    //rock2
    glColor3f(0.59f, 0.47f, 0.26f);
    glBegin(GL_POLYGON);
    glVertex2f(0, 610);
    for (int i = 0; i < 40; i++) {
        glVertex2f(rock2[i][0], rock2[i][1]);
    }
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 40; i++) {
        glVertex2f(rock2[i][0], rock2[i][1]);
    }
    glEnd();

    glEndList();

    glCallList(WAVE_UPPER);
    glCallList(WAVE_DOWN);
    glCallList(FISH);
    glCallList(TURTEL);
    glCallList(ROCK);

}
void levelChoose(int level) {
    switch (level) {
    case 0:
        r = rand() % 4;
        break;
    case 1:
        r = rand() % 5;
        break;
    case 2:
        r = rand() % 6;
        break;
    case 3:
        r = rand() % 7;
        break;
    case 4:
        r = rand() % 9;
        break;
    }
}
void background3(void)
{
    float Pi = 3.14;
    int m = 360;
    //glClearColor (0.7, 0.9, 1.0, 0.0);
#define eg 31//pink
    glNewList(eg, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.8, 1.0);//???
    for (int i = 0; i < m; i++)
        glVertex2f(13 * cos(2 * Pi / m * i) + 100, 17 * sin(2 * Pi / m * i) + 80);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);//???
    for (int i = 0; i < m; i++)
        glVertex2f(13 * cos(2 * Pi / m * i) + 100, 17 * sin(2 * Pi / m * i) + 80);
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 93, 3.5 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 100, 3.5 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 107, 3.5 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(88, 85);
    glVertex2f(95, 88);
    glVertex2f(101, 85);
    glVertex2f(107, 88);
    glVertex2f(113, 85);
    glEnd();
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(88, 72);
    glVertex2f(95, 74);
    glVertex2f(101, 72);
    glVertex2f(107, 74);
    glVertex2f(113, 72);
    glEnd();
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(90, 90);
    glVertex2f(95, 93);
    glVertex2f(100, 90);
    glVertex2f(105, 93);
    glVertex2f(110, 90);
    glEnd();
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(92, 67);
    glVertex2f(95, 70);
    glVertex2f(100, 66);
    glVertex2f(105, 70);
    glVertex2f(108, 67);
    glEnd();
    glEndList();
#define egg 32//orange
    glNewList(egg, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.58, 0.9);//???
    for (int i = 0; i < m; i++)
        glVertex2f(13 * cos(2 * Pi / m * i) + 150, 17 * sin(2 * Pi / m * i) + 80);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);//???
    for (int i = 0; i < m; i++)
        glVertex2f(13 * cos(2 * Pi / m * i) + 150, 17 * sin(2 * Pi / m * i) + 80);
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(0.66, 0.138, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 150, 2 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(0.66, 0.138, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 140, 2 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(0.66, 0.138, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 145, 2 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(0.66, 0.138, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 155, 2 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(0.66, 0.138, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 160, 2 * sin(2 * Pi / m * i) + 80);
    }
    glEnd();

    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(1.0, 0.175, 0.57);
    glVertex2f(138, 84);
    glVertex2f(141, 87);
    glVertex2f(145, 84);
    glVertex2f(148, 87);
    glVertex2f(151, 84);
    glVertex2f(154, 87);
    glVertex2f(157, 84);
    glVertex2f(160, 87);
    glVertex2f(163, 84);
    glEnd();
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(1.0, 0.175, 0.57);
    glVertex2f(138, 76);
    glVertex2f(141, 73);
    glVertex2f(145, 76);
    glVertex2f(148, 73);
    glVertex2f(151, 76);
    glVertex2f(154, 73);
    glVertex2f(157, 76);
    glVertex2f(160, 73);
    glVertex2f(163, 76);
    glEnd();

    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(1.0, 0.175, 0.57);
    glVertex2f(140, 90);
    glVertex2f(145, 93);
    glVertex2f(150, 90);
    glVertex2f(155, 93);
    glVertex2f(160, 90);
    glEnd();
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);//out_ear_left
    glColor3f(1.0, 0.175, 0.57);
    glVertex2f(142, 67);
    glVertex2f(145, 70);
    glVertex2f(150, 66);
    glVertex2f(155, 70);
    glVertex2f(158, 67);
    glEnd();

    glEndList();


#define eggs 33//purple
    glNewList(eggs, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.7);//???
    for (int i = 0; i < m; i++)
        glVertex2f(13 * cos(2 * Pi / m * i) + 200, 17 * sin(2 * Pi / m * i) + 80);
    glEnd();
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);//???
    for (int i = 0; i < m; i++)
        glVertex2f(13 * cos(2 * Pi / m * i) + 200, 17 * sin(2 * Pi / m * i) + 80);
    glEnd();
    glBegin(GL_POLYGON);//out_ear_left
    glColor3f(0.3, 0.8, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 192, 2 * sin(2 * Pi / m * i) + 72);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.8, 1.0);

    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.8, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 194, 2 * sin(2 * Pi / m * i) + 85);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.8, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 197, 2 * sin(2 * Pi / m * i) + 92);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.8, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 201, 2 * sin(2 * Pi / m * i) + 79);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.8, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 209, 2 * sin(2 * Pi / m * i) + 75);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.8, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 204, 2 * sin(2 * Pi / m * i) + 69);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.8, 1.0);
    for (int i = 0; i < m; i++) {
        glVertex2f(2 * cos(2 * Pi / m * i) + 207, 2 * sin(2 * Pi / m * i) + 88);
    }
    glEnd();
    glEndList();
#define sun 34
    glNewList(sun, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.5);
    for (int i = 0; i < m; i++) {
        glVertex2f(42 * cos(2 * Pi / m * i) + 350, 40 * sin(2 * Pi / m * i) + 520);
    }
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.2, 0.1, 0.01);
    for (int i = 0; i < m; i++) {
        glVertex2f(42 * cos(2 * Pi / m * i) + 350, 40 * sin(2 * Pi / m * i) + 520);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.35, 0.2, 0.1);
    for (int i = 0; i < m; i++) {
        glVertex2f(5 * cos(2 * Pi / m * i) + 365, 7 * sin(2 * Pi / m * i) + 525);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.35, 0.2, 0.1);
    for (int i = 0; i < m; i++) {
        glVertex2f(5 * cos(2 * Pi / m * i) + 338, 7 * sin(2 * Pi / m * i) + 525);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.6, 0.5);
    for (int i = 0; i < m; i++) {
        glVertex2f(6 * cos(2 * Pi / m * i) + 328, 3 * sin(2 * Pi / m * i) + 510);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.6, 0.5);
    for (int i = 0; i < m; i++) {
        glVertex2f(6 * cos(2 * Pi / m * i) + 376, 3 * sin(2 * Pi / m * i) + 510);
    }
    glEnd();

    glColor3f(0.35, 0.2, 0.1);
    glBegin(GL_LINE_STRIP);//???
    for (int i = 180; i < 365; i++) {
        glVertex2i(6 * cos(i * Pi / 180) + 353, 6 * sin(i * Pi / 180) + 510);
    }
    glEnd();

    glColor3f(0.2, 0.1, 0.01);
    glBegin(GL_LINES);//???
    glVertex2i(350, 570);//
    glVertex2i(350, 585);

    glVertex2i(375, 567);
    glVertex2i(386, 582);

    glVertex2i(393, 548);
    glVertex2i(408, 558);

    glVertex2i(400, 520);//
    glVertex2i(413, 520);

    glVertex2i(394, 494);
    glVertex2i(406, 485);

    glVertex2i(375, 475);
    glVertex2i(382, 463);

    glVertex2i(286, 520);//
    glVertex2i(300, 520);

    glVertex2i(304, 497);
    glVertex2i(290, 485);

    glVertex2i(319, 477);
    glVertex2i(309, 462);

    glVertex2i(350, 470);//
    glVertex2i(350, 455);

    glVertex2i(288, 556);
    glVertex2i(306, 546);

    glVertex2i(315, 577);
    glVertex2i(325, 562);


    glEnd();

    glEndList();

#define blocks 35
    glNewList(blocks, GL_COMPILE);

    glColor3f(0.7, 1.0, 0.5);
    glBegin(GL_POLYGON);
    glVertex2i(80, 100);
    glVertex2i(130, 100);
    glVertex2i(130, 120);
    glVertex2i(80, 120);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2i(80, 100);
    glVertex2i(130, 100);
    glVertex2i(130, 120);
    glVertex2i(80, 120);
    glEnd();

    glColor3f(0.7, 1.0, 0.5);
    glBegin(GL_POLYGON);//???
    for (int i = -90; i < 90; i++) {
        glVertex2i(10 * cos(i * Pi / 180) + 130, 10 * sin(i * Pi / 180) + 110);
    }
    glEnd();

    glColor3f(0.7, 1.0, 0.5);
    glBegin(GL_POLYGON);//???
    for (int i = 88; i < 272; i++) {
        glVertex2i(10 * cos(i * Pi / 180) + 80, 10 * sin(i * Pi / 180) + 110);
    }
    glEnd();


    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);//???
    for (int i = -90; i < 90; i++) {
        glVertex2i(10 * cos(i * Pi / 180) + 130, 10 * sin(i * Pi / 180) + 110);
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);//???
    for (int i = 88; i < 272; i++) {
        glVertex2i(10 * cos(i * Pi / 180) + 80, 10 * sin(i * Pi / 180) + 110);
    }
    glEnd();

    glColor3f(0.35, 0.75, 0.35);//??
    glBegin(GL_POLYGON);
    glVertex2i(70, 110);
    glVertex2i(88, 120);
    glVertex2i(96, 120);
    glVertex2i(70, 105);
    glEnd();

    glColor3f(0.35, 0.75, 0.35);
    glBegin(GL_POLYGON);
    glVertex2i(120, 120);
    glVertex2i(110, 120);
    glVertex2i(80, 101);
    glVertex2i(90, 101);
    glEnd();

    glColor3f(0.35, 0.75, 0.35);
    glBegin(GL_POLYGON);
    glVertex2i(135, 117);
    glVertex2i(140, 113);
    glVertex2i(120, 101);
    glVertex2i(110, 101);
    glEnd();

    glEndList();

#define grass 36
    glNewList(grass, GL_COMPILE);
    glColor3f(0.235, 0.7, 0.157);
    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(0, 15);
    glVertex2i(450, 15);
    glVertex2i(450, 0);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2i(0, 15);
    glVertex2i(450, 15);
    glEnd();
    glEndList();
#define right 37
    glNewList(right, GL_COMPILE);
    glBegin(GL_POLYGON);//????????
    glColor3f(0.235, 0.7, 0.157);
    for (int r = 0; r < 10; r++) {
        for (int i = -10; i < 180; i++) {
            glVertex2i(0.8 * r * cos(i * Pi / 180) + 20, 2 * r * sin(i * Pi / 180) + 15);
        }
    }
    glEnd();
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);//30_60
    for (int i = -10; i < 180; i++) {
        glVertex2f(8 * cos(i * Pi / 180) + 20, 18 * sin(i * Pi / 180) + 15);
    }
    glEnd();
    glEndList();

#define left 38
    glNewList(left, GL_COMPILE);
    glBegin(GL_POLYGON);//????????
    glColor3f(0.235, 0.7, 0.157);
    for (int r = 0; r < 10; r++) {
        for (int i = 0; i < 195; i++) {
            glVertex2i(0.8 * r * cos(i * Pi / 180) + 50, 2 * r * sin(i * Pi / 180) + 15);
        }
    }
    glEnd();
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);//30_60
    for (int i = 0; i < 195; i++) {
        glVertex2f(8 * cos(i * Pi / 180) + 50, 18 * sin(i * Pi / 180) + 15);
    }
    glEnd();
    glEndList();

#define mid 39
    glNewList(mid, GL_COMPILE);
    glBegin(GL_POLYGON);//????????
    glColor3f(0.235, 0.7, 0.157);
    for (int r = 0; r < 15; r++) {
        for (int i = 0; i < 180; i++) {
            glVertex2i(0.6 * r * cos(i * Pi / 180) + 100, 2 * r * sin(i * Pi / 180) + 15);
        }
    }
    glEnd();
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);//30_60
    for (int i = 0; i < 180; i++) {
        glVertex2f(9 * cos(i * Pi / 180) + 100, 28 * sin(i * Pi / 180) + 15);
    }
    glEnd();
    glEndList();


#define rainbow 40
    glNewList(rainbow, GL_COMPILE);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(90 * cos(i * Pi / 180) + 350, 90 * sin(i * Pi / 180) + 200);
    }
    glEnd();
    glColor3f(1.0, 0.5, 0.2);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(80 * cos(i * Pi / 180) + 350, 80 * sin(i * Pi / 180) + 200);
    }
    glEnd();
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(70 * cos(i * Pi / 180) + 350, 70 * sin(i * Pi / 180) + 200);
    }
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(60 * cos(i * Pi / 180) + 350, 60 * sin(i * Pi / 180) + 200);
    }
    glEnd();
    glColor3f(0.33, 0.8, 1.0);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(50 * cos(i * Pi / 180) + 350, 50 * sin(i * Pi / 180) + 200);
    }
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(40 * cos(i * Pi / 180) + 350, 40 * sin(i * Pi / 180) + 200);
    }
    glEnd();
    glColor3f(0.56, 0.295, 1.0);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(30 * cos(i * Pi / 180) + 350, 30 * sin(i * Pi / 180) + 200);
    }
    glEnd();
    glColor3f(0.7, 0.9, 1.0);
    glBegin(GL_POLYGON);//???
    for (int i = 20; i < 180; i++) {
        glVertex2i(20 * cos(i * Pi / 180) + 350, 20 * sin(i * Pi / 180) + 200);
    }
    glEnd();

    glEndList();
#define cloud 54
    glNewList(cloud, GL_COMPILE);
    //??
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(80 + 30 * cos(2 * M_PI * i / 100), 400 + 20 * sin(2 * M_PI * i / 100));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(110 + 30 * cos(2 * M_PI * i / 100), 425 + 25 * sin(2 * M_PI * i / 100));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(105 + 25 * cos(2 * M_PI * i / 100), 385 + 25 * sin(2 * M_PI * i / 100));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(150 + 30 * cos(2 * M_PI * i / 100), 420 + 25 * sin(2 * M_PI * i / 100));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(160 + 40 * cos(2 * M_PI * i / 100), 400 + 20 * sin(2 * M_PI * i / 100));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(145 + 25 * cos(2 * M_PI * i / 100), 380 + 20 * sin(2 * M_PI * i / 100));
    }
    glEnd();

    glEndList();
#define ALLb 41
    glNewList(ALLb, GL_COMPILE_AND_EXECUTE);
    glBegin(GL_POLYGON);
    glColor3f(1, 0.382, 0.003);
    glVertex2f(0, 0);
    glVertex2f(0, 650);
    glVertex2f(450, 650);
    glVertex2f(450, 0);
    glEnd();
    glCallList(grass);
    glCallList(cloud);
    glCallList(blocks);
    glScaled(1.5, 1.5, 0.0);
    glTranslatef(-125.0, -175.0, 0.0);
    glCallList(sun);
    glTranslatef(125.0, 175.0, 0.0);
    glScaled(0.6, 0.6, 0.0);
    glCallList(rainbow);

    glRotatef(20.0, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -10.0, 0.0);
    glCallList(right);
    glTranslatef(0.0, 10.0, 0.0);
    glRotatef(-20.0, 0.0, 0.0, 1.0);

    glRotatef(-15.0, 0.0, 0.0, 1.0);
    glTranslatef(-3.0, 10.0, 0.0);
    glCallList(left);
    glTranslatef(3.0, -10.0, 0.0);
    glRotatef(15.0, 0.0, 0.0, 1.0);

    glTranslatef(-65.0, -3.0, 0.0);
    glCallList(mid);
    glTranslatef(10.0, 3.0, 0.0);

    glRotatef(30.0, 0.0, 0.0, 1.0);
    glTranslatef(200.0, -145.0, 0.0);
    glCallList(left);
    glRotatef(-30.0, 0.0, 0.0, 1.0);
    glTranslatef(-200.0, 145.0, 0.0);

    glTranslatef(152.0, -125.0, 0.0);
    glCallList(mid);
    glTranslatef(-152.0, 125.0, 0.0);

    glTranslatef(170.0, -125.0, 0.0);
    glCallList(mid);
    glTranslatef(-170.0, 125.0, 0.0);

    glRotatef(-25.0, 0.0, 0.0, 1.0);
    glTranslatef(285.0, 10.0, 0.0);
    glCallList(right);
    glTranslatef(-285.0, -10.0, 0.0);
    glRotatef(25.0, 0.0, 0.0, 1.0);

    glRotatef(35.0, 0.0, 0.0, 1.0);
    glTranslatef(70.0, -385, 0.0);
    glCallList(eggs);
    glTranslatef(-70.0, 385.0, 0.0);
    glRotatef(-35.0, 0.0, 0.0, 1.0);

    glRotatef(-20.0, 0.0, 0.0, 1.0);
    glTranslatef(438.0, 30.0, 0.0);
    glCallList(right);
    glTranslatef(-438.0, -30.0, 0.0);
    glRotatef(20.0, 0.0, 0.0, 1.0);

    glRotatef(10.0, 0.0, 0.0, 1.0);
    glTranslatef(355.0, -200.0, 0.0);
    glCallList(left);
    glTranslatef(-355.0, 200.0, 0.0);
    glRotatef(-10.0, 0.0, 0.0, 1.0);

    glRotatef(10.0, 0.0, 0.0, 1.0);
    glTranslatef(338.0, -198.0, 0.0);
    glCallList(left);
    glTranslatef(-338.0, 198.0, 0.0);
    glRotatef(-10.0, 0.0, 0.0, 1.0);

    glRotatef(15.0, 0.0, 0.0, 1.0);
    glTranslatef(210.0, -205.0, 0.0);
    glCallList(left);
    glTranslatef(-210.0, 205.0, 0.0);
    glRotatef(-15.0, 0.0, 0.0, 1.0);

    glRotatef(10.0, 0.0, 0.0, 1.0);
    glTranslatef(273.0, -180.0, 0.0);
    glCallList(right);
    glTranslatef(-273.0, 180.0, 0.0);
    glRotatef(-10.0, 0.0, 0.0, 1.0);

    glRotatef(20.0, 0.0, 0.0, 1.0);
    glTranslatef(40.0, -235.0, 0.0);
    glCallList(eg);
    glTranslatef(-40.0, 235.0, 0.0);
    glRotatef(-20.0, 0.0, 0.0, 1.0);

    glRotatef(-10.0, 0.0, 0.0, 1.0);
    glTranslatef(-20.0, -152.0, 0.0);
    glCallList(egg);
    glTranslatef(20.0, 152.0, 0.0);
    glRotatef(10.0, 0.0, 0.0, 1.0);
    glTranslated(55, 0, 0);
    glEndList();

    glFlush();
}
void object()
{
    glColor3f(1, 0.85, 0.9);
    GLfloat p = M_PI / 180;
    
    glBegin(GL_POLYGON);
    for (int i = 180;i < 243;i++)
        glVertex2f(-70 + 54 * cos(i * p), -5 + 30 * sin(i * p));
    for (int i = 60;i < 180;i++)
        glVertex2f(-70 + 54 * cos(i * p), -5 + 10 * sin(i * p));
    glEnd();
    
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 180;i < 243;i++)
        glVertex2f(-70 + 54 * cos(i * p), -5 + 30 * sin(i * p));
    for (int i = 60;i < 180;i++)
        glVertex2f(-70 + 54 * cos(i * p), -5 + 10 * sin(i * p));
    glEnd();
    
    glColor3f(1, 0.85, 0.9);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
        glVertex2f(100 * cos(i * p), 100 * sin(i * p));
    glEnd();
    
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
        glVertex2f(100 * cos(i * p), 100 * sin(i * p));
    glEnd();
    
    glColor3f(1, 0.85, 0.9);
    tnr(-60, -80, -60);
    glBegin(GL_POLYGON);
    for (int i = 185;i < 375;i++)
        glVertex2f(15 * cos(i * p), 40 * sin(i * p));
    glEnd();
    
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 186;i < 375;i++)
        glVertex2f(15 * cos(i * p), 40 * sin(i * p));
    glEnd();
    rnt(60, 80, 60);
    
    glColor3f(1, 0.85, 0.9);
    tnr(40, -90, 60);
    glBegin(GL_POLYGON);
    for (int i = 160;i < 350;i++)
        glVertex2f(15 * cos(i * p), 40 * sin(i * p));
    glEnd();
    
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 165;i < 345;i++)
        glVertex2f(15 * cos(i * p), 40 * sin(i * p));
    glEnd();
    rnt(-40, 90, -60);
    
    glColor3f(1, 0.85, 0.9);
    glBegin(GL_POLYGON);
    for (int i = 100;i < 142;i++)
        glVertex2f(-30 + 30 * cos(i * p), 130 * sin(i * p));//glEnd();glColor3f(0,0,0);glBegin(GL_POLYGON);
    for (int i = 30;i < 70;i++)
        glVertex2f(-60 + 70 * cos(i * p), 54 + 80 * sin(i * p));
    glEnd();
    
    glColor3f(0.3, 0.12, 0);
    glBegin(GL_POLYGON);
    for (int i = 34;i < 70;i++)
        glVertex2f(-50 + 47 * cos(i * p), 67 + 54 * sin(i * p));
    for (int i = 100;i < 137;i++)
        glVertex2f(-32 + 15 * cos(i * p), 10 + 110 * sin(i * p));
    glEnd();

    
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 34;i < 70;i++)
        glVertex2f(-60 + 70 * cos(i * p), 54 + 80 * sin(i * p));
    for (int i = 100;i < 141;i++)
        glVertex2f(-30 + 30 * cos(i * p), 130 * sin(i * p));
    glEnd();

    glRotatef(-90, 0, 0, 1);
    glColor3f(1, 0.85, 0.9);
    glBegin(GL_POLYGON);
    for (int i = 100;i < 142;i++)
    {
        glVertex2f(-30 + 30 * cos(i * p), 130 * sin(i * p));
    }
    for (int i = 30;i < 70;i++)
    {
        glVertex2f(-60 + 70 * cos(i * p), 54 + 80 * sin(i * p));
    }
    glEnd();
    
    glColor3f(0.3, 0.12, 0);
    glBegin(GL_POLYGON);
    for (int i = 34;i < 70;i++)
    {
        glVertex2f(-50 + 47 * cos(i * p), 67 + 54 * sin(i * p));
    }
    for (int i = 100;i < 137;i++)
    {
        glVertex2f(-32 + 15 * cos(i * p), 10 + 110 * sin(i * p));
    }
    glEnd();
    
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 34;i < 70;i++)
    {
        glVertex2f(-60 + 70 * cos(i * p), 54 + 80 * sin(i * p));
    }
    for (int i = 100;i < 141;i++)
    {
        glVertex2f(-30 + 30 * cos(i * p), 130 * sin(i * p));
    }//glEnd();glColor3f(0,0,0);glBegin(GL_POLYGON);
    glEnd();
    glRotatef(90, 0, 0, 1);
    // white&line
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
        glVertex2f(-51 + 28 * cos(i * p), 20 + 28 * sin(i * p));
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
        glVertex2f(51 + 28 * cos(i * p), -20 + 28 * sin(i * p));
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
        glVertex2f(-51 + 28 * cos(i * p), 20 + 28 * sin(i * p));
    glEnd();
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
        glVertex2f(51 + 28 * cos(i * p), -20 + 28 * sin(i * p));
    glEnd();
    
    glColor3f(0.3, 0.62, 0.56);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
        glVertex2f(-51 + 21 * cos(i * p), 20 + 21 * sin(i * p));
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
        glVertex2f(51 + 21 * cos(i * p), -20 + 21 * sin(i * p));
    glEnd();

    glColor3f(0.4, 0.7, 0.63);
    glBegin(GL_POLYGON);
    for (int i = 200;i < 300;i++)
        glVertex2f(-51 + 21 * cos(i * p), 20 + 21 * sin(i * p));
    for (int i = 300;i > 180;i--)
        glVertex2f(-51 + 16 * cos(i * p), 20 + 16 * sin(i * p));
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 200;i < 300;i++)
        glVertex2f(51 + 21 * cos(i * p), -20 + 21 * sin(i * p));
    for (int i = 300;i > 180;i--)
        glVertex2f(51 + 16 * cos(i * p), -20 + 16 * sin(i * p));
    glEnd();
    glColor3f(0.3, 0.62, 0.56);
    glBegin(GL_POLYGON);
    for (int i = 300;i > 170;i--)
        glVertex2f(54 + 19 * cos(i * p), -20 + 19 * sin(i * p));
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 300;i > 180;i--)
        glVertex2f(-51 + 16 * cos(i * p), 20 + 16 * sin(i * p));
    glEnd();

    
    glColor3f(0.2, 0.5, 0.46);
    glBegin(GL_POLYGON);
    for (int i = -20;i < 80;i++)
        glVertex2f(-51 + 21 * cos(i * p), 20 + 21 * sin(i * p));
    for (int i = 100;i < 300;i++)
        glVertex2f(-41 + 15 * cos(i * p), 25 + 15 * sin(i * p));
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = -20;i < 80;i++)
        glVertex2f(51 + 21 * cos(i * p), -20 + 21 * sin(i * p));
    for (int i = 100;i < 300;i++)
        glVertex2f(61 + 15 * cos(i * p), -15 + 15 * sin(i * p));
    glEnd();
    
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
        glVertex2f(-40 + 8 * cos(i * p), 25 + 8 * sin(i * p));
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0;i < 360;i++)
        glVertex2f(62 + 8 * cos(i * p), -15 + 8 * sin(i * p));
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
        glVertex2f(62 + 8 * cos(i * p), -15 + 8 * sin(i * p));
    glEnd();
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
        glVertex2f(-40 + 8 * cos(i * p), 25 + 8 * sin(i * p));
    glEnd();
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
        glVertex2f(-51 + 21 * cos(i * p), 20 + 21 * sin(i * p));
    glEnd();
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
        glVertex2f(51 + 21 * cos(i * p), -20 + 21 * sin(i * p));
    glEnd();

    
    glColor3f(0, 0, 0);
    tnr(0, 25, 110);
    glBegin(GL_LINE_STRIP);
    for (int i = 120;i < 180;i++)
        glVertex2f(-70 + 54 * cos(i * p), -5 + 10 * sin(i * p));
    for (int i = 180;i < 230;i++)
        glVertex2f(-70 + 54 * cos(i * p), -5 + 30 * sin(i * p));
    glEnd();
    rnt(0, -25, -110);
    
    glBegin(GL_LINE_STRIP);
    for (int i = 235;i < 270;i++)
        glVertex2f(5 + 50 * cos(i * p), 15 + 50 * sin(i * p));
    glEnd();

    
    tnr(28, 60, 40);glColor3f(1, 0.85, 0.9);
    glBegin(GL_POLYGON);
    for (int i = -50;i < 89;i++)
    {
        glVertex2f(50 * cos(i * p), 50 * sin(i * p));
    }
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = -45;i < 86;i++)
        glVertex2f(50 * cos(i * p), 50 * sin(i * p));
    for (int i = 80;i < 178;i++)
        glVertex2f(60 * cos(i * p), 50 * sin(i * p));
    for (int i = 178;i < 270;i++)
        glVertex2f(-31 + 29 * cos(i * p), 38 * sin(i * p));
    glEnd();
    glRotatef(-5, 0, 0, 1);
    glBegin(GL_LINE_STRIP);
    for (int i = 260;i < 375;i++)
        glVertex2f(-25 + 26 * cos(i * p), -26 + 15 * sin(i * p));
    glEnd();
    rnt(-28, -60, -40);
    glBegin(GL_LINE_STRIP);
    for (int i = 130;i > 100;i--)
        glVertex2f(55 + 40 * cos(i * p), 5 + 40 * sin(i * p));
    for (int i = 280;i < 340;i++)
        glVertex2f(44 + 20 * cos(i * p), 64 + 20 * sin(i * p));

    glEnd();
    glFlush();
}
void background1() {
    glBegin(GL_POLYGON);
    glColor3f(0.386, 0.632, 0.222);
    glVertex2f(0, 0);
    glVertex2f(0, 650);
    glVertex2f(450, 650);
    glVertex2f(450, 0);
    glEnd();
    //light_green
    glBegin(GL_POLYGON);
    glColor3f(0.42, 0.636, 0.25);
    glVertex2f(66, 0);
    glVertex2f(66, 650);
    glVertex2f(126, 650);
    glVertex2f(126, 0);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.42, 0.636, 0.25);
    glVertex2f(191, 0);
    glVertex2f(191, 650);
    glVertex2f(251, 650);
    glVertex2f(251, 0);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.42, 0.636, 0.25);
    glVertex2f(311, 0);
    glVertex2f(311, 650);
    glVertex2f(371, 650);
    glVertex2f(371, 0);
    glEnd();
}
void line() {
    glLineWidth(3);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(14, 34);
    glVertex2f(14, 616);
    glVertex2f(436, 616);
    glVertex2f(436, 34);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(14, 325);
    glVertex2f(436, 325);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(292, 532);
    for (int i = 0;i < 90;i++) {
        glVertex2f(282 + 10 * cos(2 * M_PI * i / 360), 620 + 10 * sin(2 * M_PI * i / 360));
    }

    for (int i = 90;i < 180;i++) {
        glVertex2f(172 + 10 * cos(2 * M_PI * i / 360), 620 + 10 * sin(2 * M_PI * i / 360));
    }
    glVertex2f(162, 532);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(172, 542);
    glVertex2f(172, 616);
    glVertex2f(282, 616);
    glVertex2f(282, 542);
    glEnd();
    for (int i = 0;i < 3;i++) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(190 + i * 22, 616);
        glVertex2f(172, 598 - i * 22);
        glEnd();
    }
    for (int i = 3;i < 5;i++) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(190 + i * 22, 616);
        glVertex2f(162 + (i - 2) * 22, 542);
        glEnd();
    }
    for (int i = 5;i < 8;i++) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(282, 616 - (i - 4) * 22);
        glVertex2f(162 + (i - 2) * 22, 542);
        glEnd();
    }

    for (int i = 0;i < 3;i++) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(260 - i * 22, 616);
        glVertex2f(282, 598 - i * 22);
        glEnd();
    }
    for (int i = 3;i < 5;i++) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(260 - i * 22, 616);
        glVertex2f(298 - (i - 2) * 22, 542);
        glEnd();
    }
    for (int i = 5;i < 8;i++) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(172, 616 - (i - 4) * 22);
        glVertex2f(298 - (i - 2) * 22, 542);
        glEnd();
    }

    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(225 + 100 * cos(2 * M_PI / 360 * i), 325 + 100 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
}
void football() {
    glBegin(GL_POLYGON);
    glColor3f(0.39, 0.42, 0.468);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(400 + 40 * cos(2 * M_PI / 360 * i), 55 + 40 * sin(2 * M_PI / 360 * i));
    }
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < 360;i++)
    {
        glVertex2f(400 + 40 * cos(2 * M_PI / 360 * i), 55 + 40 * sin(2 * M_PI / 360 * i));
    }
    glEnd();

    int n = 540;
    int R = 18;
    glLineWidth(3);
    for (int i = -45;i < n;i += 108)
    {
        glColor3f(0.898, 0.910, 0.933);
        glBegin(GL_POLYGON);
        for (int j = -90;j < n;j += 108)
            glVertex2f((400 + 25 * sin((2 * M_PI / n * i) - 2 * M_PI / 5)) + 16 * cos(2 * M_PI / n * j), (55 + 25 * cos((2 * M_PI / n * i) - 2 * M_PI / 5)) + 16 * sin(2 * M_PI / n * j));
        glEnd();
        glColor3f(0.0, 0.0, 0.0);
        glLineWidth(3.0);
        glBegin(GL_LINE_LOOP);
        for (int j = -90;j < n;j += 108)
            glVertex2f((400 + 25 * sin((2 * M_PI / n * i) - 2 * M_PI / 5)) + 16 * cos(2 * M_PI / n * j), (55 + 25 * cos((2 * M_PI / n * i) - 2 * M_PI / 5)) + 16 * sin(2 * M_PI / n * j));
        glEnd();
    }


    glBegin(GL_POLYGON);
    glColor3f(0.39, 0.42, 0.468);
    for (int i = 0;i < n;i += 108)
        glVertex2f(400 + R * sin(2 * M_PI / n * i), 55 + R * cos(2 * M_PI / n * i));
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0;i < n;i += 108)
        glVertex2f(400 + R * sin(2 * M_PI / n * i), 55 + R * cos(2 * M_PI / n * i));
    glEnd();
}
void Piplup() {
    float body[][2] = {
        //0-45 mouth
        {504.23,368.38},{508.2,369.18},{514.56,370.5},{518.53,372.88},{527.01,373.15},{534.16,377.39},{539.46,381.36},{545.03,385.6},{550.59,392.22},{551.12,396.99},{553.5,403.34},{553.24,409.43},{552.97,417.11},{552.18,422.68},{551.65,424.79},{549.53,431.15},{549,435.12},{546.09,440.16},{544.76,442.54},{541.32,445.98},{537.08,450.48},{535.22,452.07},{533.64,453.93},{531.78,456.05},{529.13,456.84},{526.48,458.69},{523.57,459.75},{521.98,460.28},{514.82,459.49},{511.65,458.16},{506.35,453.4},{502.37,449.42},{496.81,440.16},{493.9,435.65},{490.98,430.89},{488.86,424.53},{486.74,413.67},{486.74,405.99},{487.54,399.63},{487.54,394.34},{489.92,388.51},{492.31,382.68},{495.22,378.18},{496.55,376.59},{499.19,373.42},{503.17,368.91},
        //46-59 mouthline
        {489.39,396.99},{492.57,394.07},{495.49,391.42},{498.93,388.25},{503.96,385.33},{510.32,382.68},{514.03,382.68},{519.33,382.42},{524.89,382.15},{533.37,383.48},{536.55,384.01},{540.26,384.54},{543.44,386.13},{545.56,387.45},
        //60-141
        {543.55,446.08},{544.83,453.32},{546.54,459.71},{546.54,465.25},{544.83,475.9},{545.68,480.59},{546.96,488.26},{547.81,493.8},{544.83,500.61},{542.7,504.45},{542.7,509.56},{542.7,514.67},{539.72,519.79},{539.72,523.2},{537.58,517.66},{537.58,514.25},{535.88,508.71},{534.17,502.74},{531.62,505.73},{528.63,507.86},{527.78,509.99},{524.8,513.82},{522.24,515.1},{520.96,519.79},{517.55,522.77},{512.86,527.46},{510.73,527.88},{509.88,528.73},{506.9,532.57},{505.62,533.85},{503.49,536.4},{501.78,537.68},{497.52,539.81},{497.52,535.13},{496.24,531.29},{495.39,529.59},{493.69,525.33},{491.93,520.89},{490.26,518.72},{489.25,515.87},{488.75,513.53},{487.75,509.84},{484.4,507},{484.23,501.81},{483.06,500.3},{482.22,498.12},{481.21,496.28},{480.88,495.11},{479.88,493.1},{477.53,492.26},{472.34,493.94},{466.98,495.11},{462.96,496.45},{460.11,496.78},{456.26,497.96},{453.42,498.63},{448.56,500.3},{445.38,500.3},{443.03,500.8},{440.02,500.3},{437.84,500.3},{435.5,500.8},{437,498.29},{440.69,493.6},{444.71,488.58},{446.21,487.07},{449.4,483.06},{453.58,475.52},{455.59,469.66},{459.78,463.13},{463.46,458.61},{469.16,450.41},{471.33,445.22},{473.18,440.03},{475.02,435.68},{477.51,429.87},{478.86,426.35},{480.54,423.5},{482.89,418.13},{484.23,414.27},{486.07,411.75},{544.98,443.3},
        //142-275-404-495 head
        {539.22,526.56},{538,531.26},{537.13,537.17},{535.21,541.35},{532.6,547.96},{529.82,552.84},{528.08,559.1},{524.07,564.67},{521.98,567.8},{519.89,570.24},{517.46,575.29},{515.19,578.24},{512.41,582.6},{509.28,587.12},{504.92,590.25},{503.18,592.34},{498.9,596.95},{495.22,600.45},{490.67,603.25},{487.17,606.58},{482.8,610.96},{478.59,612.88},{475.97,614.63},{472.64,618.13},{467.92,620.06},{463.71,623.21},{459.16,625.66},{454.26,628.46},{450.41,630.91},{445.86,633.18},{440.43,636.16},{434.83,637.91},{428.35,640.71},{424.33,643.33},{418.54,644.32},{413.91,645.87},{409.71,646.86},{405.4,648.29},{399.66,649.29},{395.58,649.95},{387.19,651.93},{382.88,651.82},{378.8,653.04},{368.75,652.6},{360.03,653.04},{355.5,653.04},{352.08,652.82},{347.88,652.38},{344.13,652.27},{338.06,652.49},{331.66,652.16},{328.12,652.05},{322.27,650.72},{315.76,650.5},{311.01,648.96},{305.27,648.07},{302.29,647.85},{298.2,647.3},{294.67,645.98},{291.58,645.54},{288.16,644.32},{281.49,640.7},{277.39,639.59},{273.7,637.64},{268.97,637.23},{265.14,635.42},{263.05,633.89},{259.99,631.8},{257.27,630.13},{253.52,627.77},{250.8,626.59},{248.78,625.33},{245.3,623.32},{243.15,621.65},{240.15,619.63},{237.93,617.89},{235.28,616.15},{233.12,614.9},{231.66,613.65},{230.34,612.47},{227.84,609.09},{223.21,606.77},
        {222.63,606.27},{219.41,602.38},{214.67,598.15},{209.77,593.75},{207.56,590.71},{204.52,585.46},{200.46,581.4},{196.06,577.34},{192.16,571.76},{187.76,566.17},{183.87,560.25},{179.3,552.47},{177.27,547.39},{174.56,541.3},{173.21,536.06},{170.84,529.12},{168.47,523.03},{166.44,515.08},{165.83,509.53},{164.3,504.79},{162.95,496.33},{161.43,487.19},{161.76,479.24},{160.75,471.46},{161.59,464.86},{161.59,454.88},{163.29,447.6},{165.15,438.63},{168.03,431.7},{169.38,425.94},{171.24,420.53},{176.83,408.89},{179.37,405.33},{182.59,400.43},{186.14,394.84},{188.85,387.57},{193.25,383.17},{196.8,377.93},{201.04,373.53},{204.08,369.8},{207.47,365.74},{210.51,362.36},{214.4,358.98},{217.96,354.75},{223.38,351.02},{226.93,348.49},{231.5,345.44},{236.41,341.04},{240.98,338.67},{245.38,335.29},{249.27,334.1},{252.32,332.75},{254.35,331.06},{248.93,329.54},{242.33,326.83},{235.73,323.28},{230.15,321.08},{224.39,319.38},{220.84,316.85},{216.6,314.48},{212.54,312.11},{208.65,310.25},{205.6,305.34},{203.74,305},{201.54,307.37},{199.68,310.25},{196.8,315.15},{194.26,318.37},{191.39,321.58},{188.85,325.48},{185.63,327.84},{182.59,330.38},{179.54,331.74},{176.83,333.93},{174.29,336.47},{171.92,337.66},{169.22,339.01},{166.68,339.86},{165.15,336.98},{165.66,331.74},{164.98,328.35},{164.82,323.28},{165.66,318.54},{166.68,314.48},{167.69,310.25},{169.22,306.36},{166.85,311.26},{164.31,313.63},{162.79,315.83},{161.77,318.62},{158.9,321.33},{157.37,323.53},{154.5,326.06},{151.62,330.46},{147.22,334.19},{144.51,336.72},{141.8,339.43},{137.74,343.15},{132.66,344.84},{127.59,345.69},{123.02,344.84},{120.31,340.62},{120.14,336.39},{120.99,328.09},{121.66,320.99},{123.02,318.11},{123.52,310.67},{125.22,305.76},{128.26,298.99},{130.63,293.58},{131.82,288.33},{134.36,281.9},{136.73,278.01},{139.26,273.78},{141.3,269.72},{144,266.17},{146.88,261.09},{149.08,258.05},{149.76,257.03},{147.9,253.48},{146.03,249.42},{143.49,245.02},{143.16,243.67},{142.31,240.52},{140.1,236.12},{139.77,231.89},{138.24,226.81},{137.06,221.73},{136.72,216.66},{138.41,211.24},{139.43,205.15},{140.95,201.6},{143.32,197.71},{146.37,194.83},{149.08,193.82},{153.48,191.79},{158.89,190.77},{163.12,190.6},{169.72,191.62},{175.14,192.63},{178.35,195.68},{180.56,197.37},{182.92,202.78},{183.09,206},{183.6,209.55},{184.79,212.94},{185.8,217.33},{188.17,220.55},{189.02,223.93},{190.71,227.82},{192.57,231.04},{194.6,233.58},{196.8,237.47},{198.16,239.84},{200.19,241.7},{202.39,244.58},{204.08,246.1},{207.13,248.47},{208.31,250.16},{210.17,253.71},{212.17,256.37},{213.52,258.4},{215.72,261.11},{218.26,263.47},{220.63,265.17},{223.17,266.86},{225.87,270.41},{228.92,271.77},{232.98,275.15},{235.52,277.01},{240.6,281.58},{243.14,283.61},{245.34,285.3},{248.72,287.16},{251.94,290.38},{254.99,292.41},{257.35,293.25},{261.42,294.95},{262.94,296.47},{265.48,296.47},{267,297.82},{267.17,296.13},{268.69,292.41},{269.37,289.87},{271.74,284.46},{273.09,279.55},{275.8,274.13},{277.16,268.21},{279.36,263.31},{283.76,259.08},{286.97,254.34},{291.37,250.62},{294.25,247.91},{296.62,244.69},{300.59,240.89},{304.15,238.18},{308.55,235.31},{313.29,232.77},{317.86,229.22},{322.77,226.17},{327.5,225.16},{331.06,222.96},{336.47,220.42},{341.38,217.88},{348.32,216.87},{354.07,215.85},{362.2,215.85},{367.11,215.85},{372.69,216.7},{378.28,218.39},{385.22,220.25},{391.82,223.63},{396.05,226.17},{399.94,228.03},{402.31,229.72},{404.85,234.12},{406.71,235.82},{409.59,238.35},{413.14,241.4},{414.33,244.11},{415.34,246.47},{417.04,250.37},{419.4,255.1},{420.76,257.98},{421.43,261.53},{421.43,265.43},{423.87,260.71},{427.77,258.34},{431.83,256.81},{436.57,254.95},{439.11,253.26},{442.49,252.25},{446.55,251.74},{450.79,250.05},{456.2,249.2},{462.12,248.52},{464.66,248.52},{471.77,248.35},{478.71,249.2},{483.45,250.22},{487.85,251.74},{491.41,253.94},{494.11,255.8},{497.33,259.01},{499.87,263.75},{501.73,267.98},{502.07,275.09},{500.78,274.32},{500.78,278.55},{500.28,282.44},{498.92,287.35},{498.24,291.58},{495.88,296.66},{492.66,300.89},{490.29,304.1},{488.94,306.47},{486.06,309.69},{483.52,312.05},{479.63,316.12},{475.9,319.67},{472.35,323.22},{470.32,324.58},{466.43,327.62},{462.7,329.82},{459.32,331.85},{456.1,332.7},{452.72,332.87},{466.37,341.71},{483.69,351.41},{497.55,361.8},{539.06,527.1},
        //495-601 eye
        {500.49,369.98},{496.15,374.03},{494.41,379.52},{491.81,382.13},{490.94,385.02},{489.21,389.35},{488.63,394.85},{487.18,404.39},{484,409.88},{482.55,418.84},{479.95,424.63},{477.92,427.52},{475.61,432.72},{473.87,437.92},{471.85,444.86},{467.8,448.33},{466.06,452.67},{464.32,455.56},{462.88,459.61},{458.83,463.37},{457.09,467.99},{455.93,471.46},{454.78,473.2},{453.33,475.51},{448.99,479.27},{445.52,483.89},{442.33,488.52},{438.86,492.86},{436.55,496.04},{431.05,501.24},{427.58,505.29},{421.5,508.47},{417.16,511.94},{414.84,515.12},{409.93,518.3},{403.27,522.34},{398.06,525.24},{394.01,527.84},{390.54,530.15},{384.17,531.31},{372.31,533.04},{366.23,534.78},{358.42,534.78},{352.06,534.78},{348.29,534.78},{342.22,534.78},{336.72,533.04},{331.51,531.89},{324.28,527.55},{321.38,525.81},{315.02,522.92},{308.94,519.45},{297.37,512.23},{292.45,507.6},{284.35,498.06},{277.98,487.65},{276.24,480.42},{272.77,472.91},{272.48,467.99},{270.75,463.37},{269.3,455.56},{267.57,444.29},{267.57,440.24},{267.57,432.14},{267.85,424.91},{267.85,413.93},{270.17,406.7},{272.77,400.63},{273.93,395.14},{278.27,385.6},{282.9,376.63},{290.13,366.23},{293.03,361.02},{294.62,359.16},{297.82,355.08},{300.45,352.46},{303.65,348.97},{306.86,346.34},{311.82,342.26},{315.32,339.35},{321.73,336.73},{326.69,333.52},{333.1,329.44},{338.35,327.4},{345.93,324.2},{353.51,321.28},{362.84,320.41},{369.26,319.54},{382.38,319.24},{391.71,320.7},{403.37,320.7},{414.45,321.28},{423.2,325.07},{427.28,325.36},{432.53,326.82},{438.07,328.86},{443.02,332.36},{452.94,333.81},{458.47,337.31},{463.43,339.06},{471.3,344.01},{478.88,347.8},{485.59,352.46},{491.13,355.08},{495.21,362.08},{496.96,366.45},{499.88,368.49},
        //602-673 eye-in
        {370.6,470.26},{365.29,467.88},{362.73,466.24},{360.9,464.22},{358.52,461.85},{356.87,457.64},{355.23,454.16},{353.58,449.77},{353.03,444.65},{351.38,440.81},{351.38,436.24},{351.38,431.67},{351.38,427.64},{351.38,423.98},{352.11,419.96},{353.03,415.94},{354.13,411.73},{355.59,405.69},{357.79,400.21},{358.88,398.56},{361.08,395.63},{362,393.81},{363.28,391.79},{364.74,389.23},{367.12,387.04},{368.58,385.58},{369.87,384.11},{371.7,381.19},{372.79,379.72},{376.09,379.17},{378.83,378.26},{381.58,376.98},{383.77,376.06},{386.34,376.06},{389.27,376.06},{392.01,376.98},{394.39,377.71},{396.59,379.54},{397.69,380.64},{399.15,383.01},{400.25,386.31},{402.81,389.78},{405,394.54},{405.37,397.1},{405.74,398.93},{406.84,400.94},{407.93,402.95},{408.3,406.06},{408.48,409.17},{409.03,411.18},{408.85,414.66},{408.67,417.04},{408.48,420.33},{408.3,425.27},{407.2,429.47},{406.47,432.58},{405.19,436.42},{404.82,439.35},{401.89,444.65},{400.8,449.77},{400.06,452.88},{396.95,455.26},{395.31,457.09},{393.29,459.83},{391.65,461.85},{389.81,464.22},{386.52,466.05},{382.68,468.25},{380.3,469.53},{375.91,470.62},{372.06,470.26},{368.04,468.61},
        //674-713 eyewhite
        {371.88,465.5},{370.78,465.5},{369.32,465.5},{367.85,463.86},{366.75,462.76},{365.66,461.66},{364.38,459.65},{363.09,457.82},{362.73,455.44},{362,453.43},{361.63,451.05},{361.63,448.86},{361.63,447.03},{362.18,443.74},{362.91,440.08},{364.19,438.25},{365.66,435.69},{366.75,433.5},{368.22,431.85},{370.78,427.64},{373.53,427.09},{376.46,428.01},{378.28,429.66},{380.3,432.22},{381.95,434.05},{383.77,437.34},{384.51,439.71},{384.51,442.28},{384.51,444.29},{383.77,448.31},{383.04,451.42},{381.58,455.81},{381.21,457.46},{380.3,459.1},{379.38,460.93},{377.92,461.85},{376.82,463.13},{375.17,464.41},{373.53,465.69},{372.06,465.69},
        //714-752 eye-black
        {364.74,432.4},{364.56,430.94},{364.56,428.56},{364.92,425.27},{365.29,421.42},{366.21,417.77},{367.3,412.65},{369.13,410.08},{370.6,407.16},{371.7,404.6},{373.53,403.32},{376.82,400.76},{379.2,398.38},{382.13,398.38},{384.32,398.38},{385.42,398.38},{387.99,399.48},{390.91,402.22},{390.91,404.05},{392.38,406.97},{392.74,408.07},{392.74,410.63},{392.93,413.38},{393.48,415.02},{394.57,418.68},{393.48,420.69},{393.66,422.52},{393.11,424.72},{393.11,425.63},{392.38,427.09},{391.65,428.56},{391.46,430.94},{390.36,432.4},{390,434.05},{389.08,435.87},{387.99,437.89},{386.15,438.8},{385.79,439.9},
        //752-920 body
        {262.81,295.64},{261.16,295.17},{259.16,293.52},{257.03,292.69},{254.2,290.69},{252.08,289.98},{245.83,285.86},{241.24,282.32},{237.34,279.49},{232.75,276.08},{230.39,272.54},{226.5,269.95},{223.43,266.89},{220.6,265},{217.18,262.06},{214.59,259.23},{210.7,255.93},{207.99,250.86},{204.57,246.62},{201.74,243.44},{198.44,239.55},{196.2,235.19},{193.96,231.89},{192.78,230.13},{188.28,221.43},{186.15,217.66},{184.74,212.01},{182.5,206},{181.91,201.05},{180.97,196.57},{180.02,189.38},{179.55,185.26},{179.32,177.6},{179.32,172.53},{180.73,167.23},{181.79,162.75},{183.34,160.07},{183.69,157.6},{184.63,152.3},{185.93,146.88},{186.76,144.17},{188.64,140.99},{190.76,137.1},{192.53,130.5},{195.13,128.14},{198.78,122.6},{201.73,118.6},{205.86,115.18},{207.39,113.41},{209.63,110.47},{212.58,107.41},{214.11,106.46},{216.47,104.93},{218.71,102.69},{220.95,100.1},{223.66,98.1},{225.19,96.8},{228.02,94.8},{230.74,92.44},{232.03,90.67},{235.02,87.9},{237.26,86.72},{239.62,84.13},{242.45,82.25},{245.75,78.24},{248.93,76.24},{251.29,74.12},{255.3,71.17},{259.19,68.58},{263.32,66.69},{267.09,65.63},{274.05,63.28},{279.35,63.04},{283.95,62.33},{287.49,62.33},{291.38,62.45},{297.98,63.39},{303.4,64.69},{309.65,66.69},{315.31,68.7},{316.73,71.76},{318.73,74.12},{322.03,75.77},{325.69,78.71},{330.64,81.66},{333.35,82.72},{337.12,83.54},{340.78,84.72},{345.62,85.19},{351.39,85.66},{356.58,85.9},{363.06,86.96},{368.72,88.02},{372.85,88.73},{374.15,89.32},{375.33,88.85},{375.77,91.02},{379.19,92.43},{385.68,94.91},{388.04,96.79},{390.87,98.21},{394.76,100.56},{398.17,101.86},{399.59,103.51},{402.3,105.04},{405.96,108.46},{410.2,111.29},{411.85,114.7},{414.8,116.35},{416.68,118.71},{418.57,122.13},{421.4,124.72},{424,127.19},{425.41,130.14},{426.47,131.43},{428.71,134.14},{431.07,137.68},{432.01,140.51},{433.19,143.34},{434.49,146.16},{436.61,148.87},{438.5,151.23},{440.74,155.35},{441.92,158.54},{442.39,160.3},{442.4,158.16},{443.22,160.17},{444.16,161.93},{444.64,165.59},{446.05,169.47},{446.87,174.07},{448.88,177.6},{449.59,180.43},{451.12,184.32},{452.54,188.92},{452.54,193.04},{453.48,197.99},{454.54,201.52},{454.54,205.41},{454.66,209.77},{455.01,213.31},{455.48,216.72},{455.48,220.38},{455.48,224.5},{455.48,228.27},{454.66,231.92},{454.89,234.52},{454.89,236.4},{454.19,237.7},{454.3,236.95},{454.3,239.31},{454.07,241.9},{454.07,244.73},{453.83,248.38},{452.42,250.5},{449.94,251.09},{446.64,251.91},{444.64,251.91},{441.81,252.5},{439.09,253.44},{435.56,254.86},{433.79,254.86},{432.49,255.68},{430.72,256.86},{429.07,257.33},{428.01,258.28},{426.36,259.81},{423.65,260.63},{422.94,260.87},
        //922-987 left-hand
        {333.52,219.16},{333.52,216.4},{334.13,210.26},{333.21,203.82},{331.68,194.92},{331.68,190.93},{329.84,183.56},{329.22,178.04},{327.99,174.35},{324.92,169.44},{324,163.92},{320.93,156.25},{319.09,150.42},{315.71,146.12},{313.87,141.82},{312.64,137.22},{308.03,132},{305.27,127.09},{302.81,124.64},{300.05,120.65},{296.67,116.05},{293.29,112.67},{288.99,109.91},{286.23,106.53},{281.93,102.85},{277.63,100.09},{271.49,97.94},{269.65,96.1},{266.58,94.56},{263.2,93.03},{260.43,93.64},{256.14,94.56},{254.29,97.94},{252.14,101.01},{248.46,104.69},{245.69,107.45},{243.85,110.22},{242.62,112.98},{240.78,116.66},{238.32,121.26},{236.79,126.17},{235.56,129.55},{234.33,132.62},{234.02,137.53},{231.57,141.82},{230.95,147.35},{229.11,152.87},{228.5,157.78},{228.19,164.84},{227.58,169.75},{226.96,176.2},{226.96,180.8},{226.65,188.78},{228.8,193.38},{228.8,198.29},{230.03,202.28},{230.95,208.11},{231.26,212.72},{232.8,215.48},{234.95,218.55},{235.87,223.76},{237.71,229.29},{240.17,233.28},{242.01,236.35},{243.85,240.34},{243.85,241.26},{246.31,244.02},
        //988-1040 w1
        {389.65,222.56},{391.58,220.62},{392.55,217.72},{394.01,215.66},{394.49,213.25},{395.22,210.95},{395.82,207.32},{396.43,203.94},{396.91,200.31},{397.27,197.41},{397.27,193.78},{397.27,190.63},{396.91,188.34},{396.06,185.8},{395.58,183.26},{393.64,179.87},{392.19,176.73},{389.77,173.7},{388.08,172.01},{385.78,169.23},{382.87,167.42},{380.33,164.63},{378.76,163.91},{376.22,162.82},{373.8,161.85},{371.01,161.37},{367.63,161.13},{364.48,161.61},{361.82,161.97},{358.92,162.7},{357.7,164.76},{355.89,166.81},{354.32,168.38},{352.14,170.8},{351.05,172.13},{350.57,174.79},{349.11,177.69},{348.63,180.72},{348.02,182.41},{346.81,185.07},{346.81,188.46},{346.81,191.72},{346.94,194.74},{348.02,198.25},{348.39,200.67},{348.51,202.73},{350.08,206.6},{351.53,209.5},{352.5,210.59},{353.23,213.12},{354.07,214.58},{354.56,215.42},
        //1040-1085 w2
        {454.08,237.32},{453,238.65},{451.3,239.74},{449.97,240.46},{446.94,240.95},{444.77,239.25},{443.32,237.32},{441.74,234.66},{440.53,232},{439.81,229.1},{437.99,225.95},{437.14,222.93},{436.42,220.27},{435.21,216.76},{434.48,213.13},{434.72,209.99},{433.63,204.67},{434.6,202.25},{434.6,199.71},{434.6,195.24},{434.6,191.85},{435.57,189.19},{436.06,186.05},{437.63,184.23},{439.32,182.18},{440.65,180.85},{442.47,180.36},{446.46,180.61},{450.45,181.69},{451.18,186.05},{451.3,188.34},{452.51,192.58},{453.48,196.33},{453.72,200.8},{453.72,203.82},{453.72,206.24},{454.69,209.14},{454.81,212.53},{454.81,216.04},{454.81,218.7},{454.81,221.6},{455.17,225.35},{454.81,227.64},{454.45,232},{454.45,235.38},
        //1085-1114 right hand
        {477.87,248.24},{479.41,245.14},{480.34,238.34},{482.51,229.07},{482.82,224.74},{483.13,218.56},{484.05,212.99},{484.05,204.03},{484.05,194.44},{484.05,186.71},{481.58,181.77},{480.96,172.8},{478.18,166.31},{476.32,160.74},{474.78,156.1},{471.06,153.94},{470.13,150.23},{467.66,147.14},{466.42,144.36},{464.26,143.74},{461.47,141.88},{458.38,140.03},{456.83,139.1},{451.26,137.55},{445.69,137.86},{441.36,137.86},{436.72,140.65},{434.56,141.57},{452.19,248.24},
        //1114-1175 left-foot
        {232.41,89.49},{229.48,88.33},{228.31,83.85},{228.51,76.83},{230.26,72.55},{232.99,66.9},{235.52,63.39},{238.45,59.5},{241.57,53.07},{246.83,48.98},{249.75,45.28},{253.07,41.77},{255.01,39.63},{258.33,36.12},{261.64,33.98},{264.37,30.47},{266.51,27.74},{268.46,26.58},{271,24.24},{274.12,21.71},{278.8,18.78},{282.3,16.45},{287.96,12.36},{291.66,10.99},{295.56,9.43},{299.85,8.85},{306.47,6.32},{310.18,4.37},{314.66,2.03},{317.2,0.67},{321.87,-0.29},{324.99,0.28},{327.53,-0.1},{329.67,-0.1},{332.4,-0.1},{336.1,1.06},{339.61,1.25},{342.14,1.45},{343.31,3.01},{345.46,3.98},{346.82,5.54},{347.02,8.66},{348.58,10.8},{348.77,15.28},{349.16,21.12},{348.19,24.43},{347.21,28.91},{346.24,31.84},{344.48,33.98},{343.12,37.49},{341.37,40.99},{338.25,47.22},{336.1,49.56},{332.2,52.87},{329.08,55.99},{325.77,59.69},{323.82,61.83},{321.68,65.34},{318.56,66.9},{315.05,68.46},{311.93,70.21},
        //1175-1197
        {289.52,44.69},{291.85,43.52},{294,41.77},{295.95,40.02},{297.51,37.29},{299.85,34.95},{300.82,33.39},{301.99,31.64},{302.77,29.89},{304.13,28.33},{306.28,26.19},{308.23,23.07},{309.01,21.32},{310.18,20.54},{310.76,19.76},{311.15,17.81},{311.93,16.06},{312.71,12.94},{313.88,10.41},{314.47,7.49},{314.66,5.34},{315.25,3.59},
        //1197-1212
        {347.77,14.62},{344.94,18.7},{343.06,22.78},{341.8,25.92},{338.97,29.37},{337.09,33.13},{333.32,36.9},{331.13,39.41},{328.3,42.23},{326.73,43.8},{325.79,45.68},{324.53,49.45},{321.39,51.96},{320.45,52.27},{316.99,53.53},
        //1212-1257 right-foot
        {415.49,114.33},{419.45,113.71},{423.16,112.84},{427.49,111.48},{430.96,110.25},{435.29,109.13},{440.98,106.29},{445.43,104.44},{449.39,102.83},{453.1,100.6},{457.68,97.76},{460.65,94.92},{463.74,92.81},{466.59,89.97},{470.43,87.13},{474.14,83.54},{477.85,79.09},{480.94,73.15},{482.55,69.82},{481.56,65.86},{478.71,62.15},{476.74,59.92},{470.92,58.19},{467.95,56.96},{463.5,55.6},{457.56,54.98},{451.12,56.59},{445.8,56.71},{440.11,57.57},{434.17,58.07},{430.46,59.31},{425.27,60.17},{419.33,63.26},{415.61,64.38},{411.53,67.22},{407.33,67.96},{402.87,70.68},{398.17,73.15},{394.83,74.27},{388.77,76.86},{384.68,79.21},{382.08,80.94},{378.87,83.66},{376.02,86.26},{373.92,89.1},
        //1257-1280
        {459.17,58.07},{458.8,60.91},{456.2,63.39},{455.45,64.25},{454.09,65.24},{452.24,66.85},{450.38,68.46},{448.53,69.44},{445.93,71.42},{443.95,72.54},{442.09,73.65},{439.37,74.76},{437.27,76.24},{434.55,77.11},{431.7,79.21},{430.58,80.08},{428.61,80.57},{426.13,80.7},{424.89,81.93},{423.41,82.8},{421.55,84.03},{418.71,85.27},{416.36,86.51},
        //1280-1297
        {468.32,84.65},{466.84,86.14},{464.73,88.49},{461.77,89.85},{459.04,91.95},{456.44,93.68},{453.48,94.67},{451.12,95.9},{448.65,96.52},{446.67,97.02},{445.43,98.01},{443.83,98.62},{442.83,99},{440.24,100.36},{437.64,101.1},{435.41,102.21},{433.43,102.83}
    };

#define rhand 48
    glNewList(rhand, GL_COMPILE);
    glColor3f(0.507, 0.691, 0.746);
    glBegin(GL_POLYGON);
    for (int i = 1085;i < 1114;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 1085;i < 1114;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glEndList();

#define lfoot 49
    glNewList(lfoot, GL_COMPILE);
    glColor3f(0.945, 0.8, 0.234);
    glBegin(GL_POLYGON);
    for (int i = 1114;i < 1175;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 1114;i < 1175;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 1175;i < 1197;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 1197;i < 1212;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glEndList();

#define rfoot 50
    glNewList(rfoot, GL_COMPILE);
    glColor3f(0.945, 0.8, 0.234);
    glBegin(GL_POLYGON);
    for (int i = 1212;i < 1257;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 1212;i < 1257;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 1257;i < 1280;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 1280;i < 1297;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glEndList();

#define Bodyp 51
    glNewList(Bodyp, GL_COMPILE);
    glColor3f(0.582, 0.789, 0.843);
    glBegin(GL_POLYGON);
    for (int i = 752;i < 921;i++) {
        glVertex2f(body[i][0], body[i][1]);
    }
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 752;i < 921;i++) {
        glVertex2f(body[i][0], body[i][1]);
    }
    glEnd();
    //left-hand
    glBegin(GL_LINE_STRIP);
    for (int i = 921;i < 988;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    //w1
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 988;i < 1040;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 988;i < 1040;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    //w2
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 1040;i < 1085;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 1040;i < 1085;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    //head
    glColor3f(0.414, 0.632, 0.781);
    glBegin(GL_POLYGON);
    glVertex2f(420, 405);
    for (int i = 142;i < 275;i++)
        glVertex2f(body[i][0], body[i][1]);
    glVertex2f(420, 405);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(183, 261);
    for (int i = 275;i < 404;i++)
        glVertex2f(body[i][0], body[i][1]);
    glVertex2f(252, 330);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(252, 330);
    for (int i = 404;i < 495;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 140;i < 495;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    //??
    glColor3f(0.582, 0.789, 0.843);
    glBegin(GL_POLYGON);
    for (int i = 60;i < 142;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 60;i < 141;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glEndList();

#define mouthp 52
    glNewList(mouthp, GL_COMPILE);
    glColor3f(0.945, 0.8, 0.234);
    glBegin(GL_POLYGON);
    for (int i = 0;i < 46;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i < 46;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 46;i < 60;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glEndList();

#define eyep 53
    glNewList(eyep, GL_COMPILE);
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 495;i < 602;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 495;i < 602;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    //eye-in
    glColor3f(0.179, 0.324, 0.554);
    glBegin(GL_POLYGON);
    for (int i = 602;i < 674;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 602;i < 674;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 714;i < 752;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 674;i < 714;i++)
        glVertex2f(body[i][0], body[i][1]);
    glEnd();
    glEndList();
    glLineWidth(1);
    glCallList(rhand);
    glCallList(lfoot);
    glCallList(rfoot);
    glCallList(Bodyp);
    glCallList(mouthp);
    glCallList(eyep);

}
void _level()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 0.9, 0.6, 0.5);
    glBegin(GL_POLYGON);
    glVertex2f(0, 250);
    glVertex2f(450, 250);
    glVertex2f(450, 300);
    glVertex2f(0, 300);
    glEnd();
}
void buffer(int r, GLfloat a)                //The function that takes displays random screens everytime its called. r=random variable.
{
    //    r=5;
    switch (r)
    {
    case 0:
        glColor3f(1, 1, 1);
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glRectf(2.35, a + 1.85, 1.35, a + 1.75);
        glRectf(-1.8, a + 1.15, -0.8, a + 1.25);
        glRectf(-1.7, a + 0.15, -0.7, a + 0.05);
        glRectf(1.5, a - 1.0, 0.5, a - 1.11);
        glRectf(-0.25, a - 0.85, 0.75, a - 0.75);
        glRectf(2.25, a + 0.7, 1.25, a + 0.6);
        glRectf(2.35, a + 0.15, 1.35, a + 0.25);
        glRectf(-1.85, a - 1.85, -0.85, a - 1.75);
        glRectf(0.75, a + 0.95, -0.25, a + 1.05);
        glRectf(-0.25, a + 1.70, -1.25, a + 1.80);

        glPopMatrix();
        break;

    case 1:
        glColor3f(1, 1, 1);
        /*transX+=transdX;
      if(transX>=0.5)
              transdX=-transdX;
      else if(transX<=-3.5)
          transdX=-transdX;*/
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glRectf(2.35, a + 1.85, 1.35, a + 1.75);
        glRectf(-1.8, a + 1.15, -0.8, a + 1.25);
        glRectf(-0.25, a - 0.85, 0.75, a - 0.75);
        //glRectf(-0.75,a-0.7,-0.25,a-0.6);
        glPushMatrix();
        glTranslatef(transX, 0, 0);
        glRectf(2.35, a + 0.15, 1.35, a + 0.25);
        glPopMatrix();
        glRectf(-1.85, a - 1.85, -0.85, a - 1.75);
        glPopMatrix();
        break;
    case 2:
        glColor3f(1, 1, 1);
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glRectf(2.35, a - 1.85, 1.35, a - 1.75);
        glRectf(-1.8, a - 1.15, -0.8, a - 1.25);
        glRectf(-1.7, a - 0.15, -0.7, a - 0.05);
        glRectf(1.5, a + 1.0, 0.5, a + 1.11);
        glRectf(-0.25, a + 0.85, 0.75, a + 0.75);
        glRectf(2.25, a - 0.7, 1.25, a - 0.6);
        glRectf(2.35, a - 0.15, 1.35, a - 0.25);
        glRectf(-1.85, a + 1.85, -0.85, a + 1.75);
        glRectf(0.75, a - 0.95, -0.25, a - 1.05);
        glRectf(-0.25, a - 1.70, -1.25, a - 1.80);

        glPopMatrix();
        break;

    case 3:
        glColor3f(1, 1, 1);
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glRectf(-1.85, a + 1.85, -0.85, a + 1.75);
        glRectf(2.3, a + 1.15, 1.3, a + 1.25);
        glRectf(2.2, a + 0.15, 1.2, a + 0.05);
        glRectf(0.5, a - 1.0, -0.5, a - 1.11);
        glRectf(0.75, a - 0.85, -0.25, a - 0.75);
        glRectf(-1.75, a + 0.7, -0.75, a + 0.6);
        glRectf(-1.85, a + 0.15, -0.85, a + 0.25);
        glRectf(2.35, a - 1.85, 1.35, a - 1.75);
        glRectf(-0.25, a + 0.95, 0.75, a + 1.05);
        glRectf(0.75, a + 1.70, 1.75, a + 1.80);

        glPopMatrix();
        break;

    case 4:
        glColor3f(1, 1, 1);
        transX += transdX;
        if (transX >= 0.5)
            transdX = -transdX;
        else if (transX <= -1.5)
            transdX = -transdX;
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glPushMatrix();
        glTranslatef(transX, 0, 0);
        glRectf(-0.35, a - 1.85, -1.35, a - 1.75);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-transX, 0, 0);
        glRectf(2.3, a - 1.15, 1.3, a - 1.25);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(transX, 0, 0);
        glRectf(2.2, a - 0.15, 1.2, a - 0.05);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-transX, 0, 0);
        glRectf(-1.0, a + 1.0, 0.0, a + 1.11);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(transX, 0, 0);
        glRectf(0.75, a + 0.85, -0.25, a + 0.75);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-transX, 0, 0);
        glRectf(-1.75, a - 0.7, -0.75, a - 0.6);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(transX, 0, 0);
        glRectf(-1.85, a - 0.15, -0.85, a - 0.25);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-transX, 0, 0);
        glRectf(2.35, a + 1.85, 1.35, a + 1.75);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(transX, 0, 0);
        glRectf(-0.25, a - 0.95, 0.75, a - 1.05);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-transX, 0, 0);
        glRectf(0.75, a - 1.70, 1.75, a - 1.80);
        glPopMatrix();

        glPopMatrix();
        break;

    case 5:
        glColor3f(1, 1, 1);
        transX += transdX;
        if (transX >= 3.5)
            transdX = -transdX;
        else if (transX <= -0.5)
            transdX = -transdX;
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glRectf(-0.25, a - 1.87, 0.75, a - 1.77);
        glRectf(-0.97, a - 0.9, 0.03, a - 0.8);
        glRectf(1.43, a + 0.1, 2.43, a + 0.2);
        glRectf(-0.35, a + 1.0, 0.65, a + 1.1);
        glPushMatrix();
        glTranslatef(transX, 0, 0);
        glRectf(-1.80, a + 1.9, 0.80, a + 1.8);
        glPopMatrix();
        glRectf(0.5, a + 1.4, 1.0, a + 1.5);
        glPopMatrix();
        break;

    case 6:
        glColor3f(1, 1, 1);
        angle += 0.15;                  //Rotational and translation effects are given to the bricks using functions and pushmatrix popmatrix operations.
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glRectf(-1.5, a - 1.5, -0.5, a - 1.4);
        glRectf(1.0, a - 1.0, 2.0, a - 0.9);
        glPushMatrix();
        glTranslatef(0, +a - 0.05, 0);
        glRotatef(angle, 0, 0, 1);
        glTranslatef(0, -a + 0.05, 0);
        glRectf(-0.25, a - 0.1, 0.75, a + 0.0);
        glPopMatrix();
        glRectf(-0.75, a - 0.7, 0.25, a - 0.6);
        glRectf(-1.0, a + 0.7, 0, a + 0.8);
        glRectf(0.5, a + 1.4, 1.5, a + 1.5);
        glPopMatrix();
        break;
    case 7:
        glColor3f(1, 1, 1);
        angle += 0.15;
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glPushMatrix();
        glTranslatef(1.25, a + 1.45, 0);
        glRotatef(angle, 0, 0, 1);
        glTranslatef(-1.25, -a - 1.45, 0);
        glRectf(1.5, a + 1.5, 1.0, a + 1.4);
        glPopMatrix();
        glRectf(-0.5, a + 1.0, -1.5, a + 0.9);
        glRectf(0.75, a + 0.1, -0.25, a + 0.0);
        glRectf(1.25, a + 0.7, 0.25, a + 0.6);
        glRectf(1.5, a - 0.7, 0.5, a - 0.8);
        glRectf(-0.5, a - 1.4, 0.5, a - 1.5);
        glPopMatrix();
        break;
    case 8:
        glColor3f(1, 1, 1);
        angle += 0.15;
        glPushMatrix();
        glTranslatef(0.0, v1, -2.0);
        glRectf(-1.85, a - 1.85, 0.85, a - 1.75);
        glRectf(1.8, a - 1.15, 0.8, a - 1.25);
        glPushMatrix();
        glTranslatef(0, a + 0.80, 0);
        glRotatef(angle, 0, 0, 1);
        glTranslatef(0, -a - 0.80, 0);
        glRectf(0.75, a + 0.85, -0.25, a + 0.75);
        glPopMatrix();
        glRectf(-1.85, a - 0.15, -0.85, a - 0.25);
        glRectf(2.35, a + 1.85, 1.35, a + 1.75);
        glPopMatrix();
        break;


    default:
        break;
    };
}
void Draw() {
    if (page == 0) {
        level = 0;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        pro();
        feature();
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(100, 180);
        drawString("Ash's hat has", 30);
        glRasterPos2f(100, 140);
        drawString("gone missing, please help", 30);
        glRasterPos2f(100, 100);
        drawString(" him find his hat!", 30);
        glRasterPos2f(100, 100);
        drawString(" him find his hat!", 30);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 0.0, 0.0, 0.5);
        glRasterPos2f(220, -50);
        drawString(" Press the 'p' key to start ", 10);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 12) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        resetpar();
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        pro();
        feature();
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(100, 180);
        drawString("You lost!", 30);
        glRasterPos2f(100, 140);
        drawString("Oops ", 30);
        glRasterPos2f(100, 100);
        drawString("Try again.", 30);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 0.0, 0.0, 0.5);
        glRasterPos2f(220, -50);
        drawString(" Press the 'b' key to start", 10);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 13) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        resetpar();
        score = 0;
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        pro();
        feature();
        glTranslated(480, -35, 0);
        glRotated(180, 0, 1, 0);
        glScaled(1.4, 1.3, 0);
        glCallList(HAT);
        glScaled(0.7, 0.7, 0);
        glRotated(180, 0, 1, 0);
        glTranslated(-480, 35, 0);
        glColor3f(0.57, 0.89, 0.9);
        glRasterPos2f(100, 230);
        drawString("YOU WIN!", 30);
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(100, 180);
        drawString("Thank you so much", 30);
        glRasterPos2f(100, 140);
        drawString("for helping me find", 30);
        glRasterPos2f(100, 100);
        drawString("my hat!", 30);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 0.0, 0.0, 0.5);
        glRasterPos2f(220, -50);
        drawString(" Press the 'b' key to restart", 10);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 1) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background1();
        line();
        football();
        glScaled(2, 2, 0);
        glTranslated(0, -120, 0);
        _level();
        glTranslated(0, 120, 0);
        glScaled(0.5, 0.5, 0);
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(50, 280);
        drawString("Level 1", 30);
        glRasterPos2f(170, 280);
        drawString("press 'm' to start", 100);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 11) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        acceleration = 0.000002;
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background1();
        line();
        football();
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        Score();
        Timer();
        if (_count == -1 || _count >= 4) { //When above buffer screen comes to main screen then count=vold-v1=4
            v = r;           //in this loop v takes initial value of r
            levelChoose(level);
            p += 4;
            _count = 0;
            vold = v1;              //vold is assigned v1
        }
        buffer(v, p);
        buffer(r, p + 4);

        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glScalef(0.0018, 0.00074, 1);
        togebi();
        glPopMatrix();

        xScreen = (int)((width - 0.1 + (width * (x + 0.3)) / 2) / 2);                     //contains the x and y coordinates on screen of
        yScreen = (int)((height + (height * (y - 0.016)) / 2) / 2 - 0.5);            //pixel below the ball
        glReadPixels(xScreen, yScreen, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pic_col); // reads pixel color.
        if (pic_col[2] > 240 && Velocity > 0)   //Condition is satisfied when color below ball is color of brick and ball is going down.
        {
            Velocity = -0.0069;
        }
        if (y >= yref && Velocity < 0)
        {
            Velocity += acceleration; //when y of ball is greater than yref then ball motion is stopped and screen moves.
            v1 += Velocity;
            _count = vold - v1;
        }
        else
        {
            Velocity += acceleration;  //Velocity is increased by acceleration
            y -= Velocity; //position is dependent on Velocity
        }

        if (y <= -2)
        {
            page = 12;  //if the ball falls out of the screen then end game screen is displayed.

        }

        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 2) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        level = 1;
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background2();
        rocket();
        plent1();
        plent2();
        glScaled(2.4, 2.4, 0);
        glTranslated(0, -150, 0);
        _level();
        glTranslated(0, 150, 0);
        glScaled(0.42, 0.42, 0);
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(50, 260);
        drawString("Level 2", 30);
        glRasterPos2f(170, 260);
        drawString("press 'm' to start", 30);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 22) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        acceleration = 0.0004;
        jumpHeight = 0.5;
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background2();
        rocket();
        plent1();
        plent2();
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        Score();
        Timer();
        if (_count == -1 || _count >= 4) { //When above buffer screen comes to main screen then count=vold-v1=4
            v = r;           //in this loop v takes initial value of r
            levelChoose(level);
            p += 4;
            _count = 0;
            vold = v1;              //vold is assigned v1
        }
        buffer(v, p);
        buffer(r, p + 4);

        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glScalef(0.008, 0.004, 1);
        charmander();
        glPopMatrix();

        xScreen = (int)((width + (width * (x + 1.41)) / 2) / 2);                     //contains the x and y coordinates on screen of
        yScreen = (int)((height + (height * (y - 0.016)) / 2) / 2 - 0.7);            //pixel below the ball
        glReadPixels(xScreen, yScreen, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pic_col); // reads pixel color.
        if (pic_col[1] > 240 && Velocity > 0)   //Condition is satisfied when color below ball is color of brick and ball is going down.
        {
            Velocity = -0.069;
        }
        if (y >= yref && Velocity < 0)
        {
            Velocity += acceleration; //when y of ball is greater than yref then ball motion is stopped and screen moves.
            v1 += Velocity;
            _count = vold - v1;
        }
        else
        {
            Velocity += acceleration;  //Velocity is increased by acceleration
            y -= Velocity; //position is dependent on Velocity
        }

        if (y <= -2)
        {
            page = 12;  //if the ball falls out of the screen then end game screen is displayed.

        }

        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 3) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        level = 2;
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background3();
        glScaled(2.4, 2.4, 0);
        glTranslated(-100, -180, 0);
        _level();
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(100, 260);
        drawString("Level 3", 30);
        glRasterPos2f(160, 260);
        drawString("press 'm' to start", 30);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 33) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        acceleration = 0.0001;
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background3();
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        Score();
        Timer();
        if (_count == -1 || _count >= 4) { //When above buffer screen comes to main screen then count=vold-v1=4
            v = r;           //in this loop v takes initial value of r
            levelChoose(level);
            p += 4;
            _count = 0;
            vold = v1;              //vold is assigned v1
        }
        buffer(v, p);
        buffer(r, p + 4);

        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glScalef(0.0018, 0.00074, 1);
        Piplup();
        glPopMatrix();

        xScreen = (int)((width - 0.2 + (width * (x + 0.1)) / 2) / 2);                     //contains the x and y coordinates on screen of
        yScreen = (int)((height + (height * (y - 0.01)) / 2) / 2 - 0.3);            //pixel below the ball
        glReadPixels(xScreen, yScreen, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pic_col); // reads pixel color.
        if (pic_col[2] > 240 && Velocity > 0)   //Condition is satisfied when color below ball is color of brick and ball is going down.
        {
            Velocity = -0.035;
        }
        if (y >= yref && Velocity < 0)
        {
            Velocity += acceleration; //when y of ball is greater than yref then ball motion is stopped and screen moves.
            v1 += Velocity;
            _count = vold - v1;
        }
        else
        {
            Velocity += acceleration;  //Velocity is increased by acceleration
            y -= Velocity; //position is dependent on Velocity
        }

        if (y <= -2)
        {
            page = 12;  //if the ball falls out of the screen then end game screen is displayed.

        }

        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 4) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        level = 3;
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background4();
        _level();
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(40, 260);
        drawString("Level 4", 30);
        glRasterPos2f(100, 260);
        drawString("press 'm' to start", 30);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 44) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        acceleration = 0.0002;
        glPushMatrix();
        glTranslatef(-3.0, -2, 0.0);
        glScalef(0.01345, 0.00625, 1);
        background4();
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        Score();
        Timer();
        if (_count == -1 || _count >= 4) { //When above buffer screen comes to main screen then count=vold-v1=4
            v = r;           //in this loop v takes initial value of r
            levelChoose(level);
            p += 4;
            _count = 0;
            vold = v1;              //vold is assigned v1
        }
        buffer(v, p);
        buffer(r, p + 4);

        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glScalef(0.0018, 0.00074, 1);
        squirtle();
        glPopMatrix();

        xScreen = (int)((width + (width * (x + 1.41)) / 2) / 2);                     //contains the x and y coordinates on screen of
        yScreen = (int)((height + (height * (y - 0.016)) / 2) / 2 - 0.7);            //pixel below the ball
        glReadPixels(xScreen, yScreen, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pic_col); // reads pixel color.
        if (pic_col[2] > 240 && Velocity > 0)   //Condition is satisfied when color below ball is color of brick and ball is going down.
        {
            Velocity = -0.069;
        }
        if (y >= yref && Velocity < 0)
        {
            Velocity += acceleration; //when y of ball is greater than yref then ball motion is stopped and screen moves.
            v1 += Velocity;
            _count = vold - v1;
        }
        else
        {
            Velocity += acceleration;  //Velocity is increased by acceleration
            y -= Velocity; //position is dependent on Velocity
        }

        if (y <= -2)
        {
            page = 12;  //if the ball falls out of the screen then end game screen is displayed.

        }

        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 5) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        level = 4;
        glPushMatrix();
        glTranslatef(-3.4, -2, 0.0);
        glScalef(0.014, 0.006, 1);
        background_sea();
        glScaled(2, 2, 0);
        glTranslated(0, -120, 0);
        _level();
        glTranslated(0, 120, 0);
        glScaled(0.5, 0.5, 0);
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(50, 280);
        drawString("Level 5", 30);
        glRasterPos2f(170, 280);
        drawString("press 'm' to start", 100);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else if (page == 55) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //GLfloat qaCustom[]= {0.9992,0.7992,0.4894,1.0};
        acceleration = 0.000009;
        glPushMatrix();
        glTranslatef(-3.4, -2, 0.0);
        glScalef(0.014, 0.006, 1);
        background_sea();
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        Score();
        Timer();
        if (_count == -1 || _count >= 4) { //When above buffer screen comes to main screen then count=vold-v1=4
            v = r;           //in this loop v takes initial value of r
            //r=rand()%9;int n=3600;
            int N = 0;
            float PI = 3.1415926f;
            float R = 100, r;
            float x, y, j, a, b, k;
            levelChoose(level);
            p += 4;
            _count = 0;
            vold = v1;              //vold is assigned v1
        }
        buffer(v, p);
        buffer(r, p + 4);

        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glScalef(0.0018, 0.00074, 1);
        togebi();
        glPopMatrix();

        xScreen = (int)((width + (width * (x + 0.1)) / 2) / 2);                     //contains the x and y coordinates on screen of
        yScreen = (int)((height + (height * (y - 0.016)) / 2) / 2 - 0.5);            //pixel below the ball
        glReadPixels(xScreen, yScreen, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pic_col); // reads pixel color.
        if (pic_col[2] > 240 && Velocity > 0)   //Condition is satisfied when color below ball is color of brick and ball is going down.
        {
            Velocity = -0.005;
        }
        if (y >= yref && Velocity < 0)
        {
            Velocity += acceleration; //when y of ball is greater than yref then ball motion is stopped and screen moves.
            v1 += Velocity;
            _count = vold - v1;
        }
        else
        {
            Velocity += acceleration;  //Velocity is increased by acceleration
            y -= Velocity; //position is dependent on Velocity
        }

        if (y <= -2)
        {
            page = 12;  //if the ball falls out of the screen then end game screen is displayed.

        }

        glutSwapBuffers();
        glutPostRedisplay();
    }

}
void reshape(int x, int y) {  //Function is calld when screen is resized
    width = x;
    height = y;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei)x, (GLsizei)y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.02, 3.02, -2.0, 2.0, -3.0, 3.0);
}
void KeyInput(unsigned char Key, int a, int b) {
    if (page == 0) {
        switch (Key) {
        case 'p':
            page = 1;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 1) {
        switch (Key) {
        case 'm':
            page = 11;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 11) {
        switch (Key) {
        case 'a':
            x -= dx;
            if (x <= -3.0)
                x = 3.0;
            break;
        case 'd':
            x += dx;
            if (x >= 3.0)
                x = -3.0;
            break;
        case 'm':
            page = 11;
            glutPostRedisplay();
        }
    }
    if (page == 2) {
        switch (Key) {
        case 'm':
            page = 22;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 22) {
        switch (Key) {
        case 'a':
            x -= dx;
            if (x <= -4.0)
                x = 4.0;
            break;
        case 'd':
            x += dx;
            if (x >= 4.0)
                x = -4.0;
            break;
        case 'm':
            page = 22;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 3) {
        switch (Key) {
        case 'm':
            page = 33;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 33) {
        switch (Key) {
        case 'a':
            x -= dx;
            if (x <= -3.0)
                x = 3.0;
            break;
        case 'd':
            x += dx;
            if (x >= 3.0)
                x = -3.0;
            break;
        case 'm':
            page = 33;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 4) {
        switch (Key) {
        case 'm':
            page = 44;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 44) {
        switch (Key) {
        case 'a':
            x -= dx;
            if (x <= -3.0)
                x = 3.0;
            break;
        case 'd':
            x += dx;
            if (x >= 3.0)
                x = -3.0;
            break;
        case 'm':
            page = 44;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 5) {
        switch (Key) {
        case 'm':
            page = 55;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 55) {
        switch (Key) {
        case 'a':
            x -= dx;
            if (x <= -3.0)
                x = 3.0;
            break;
        case 'd':
            x += dx;
            if (x >= 3.0)
                x = -3.0;
            break;
        case 'm':
            page = 55;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 12) {
        switch (Key) {
        case 'b':
            page = 0;
            glutPostRedisplay();
            break;
        }
    }
    if (page == 13) {
        switch (Key) {
        case 'b':
            page = 0;
            glutPostRedisplay();
            break;
        }
    }

}
void Initialize() {
    glClearColor(0.24f, 0.44f, 0.59f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.02, 3.02, -2.0, 2.0, -3.0, 3.0);

}
int main(int iArgc, char** cppArgv) {
    PlaySound(L"poko.wav", NULL, SND_ASYNC);
    glutInit(&iArgc, cppArgv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 10);
    glutInitWindowSize(450, 650);
    glutCreateWindow("Bounce_Beasts");
    Initialize();
    glutDisplayFunc(Draw);
    glutKeyboardFunc(KeyInput);
    glutReshapeFunc(reshape);
    glutTimerFunc(33, timer, 0);
    glutMainLoop();
}
