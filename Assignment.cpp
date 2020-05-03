//  ========================================================================
//  COSC363: Computer Graphics (2018);  University of Canterbury.
//
//  FILE NAME: Skybox.cpp
//  See Lab03.pdf for details
//  ========================================================================

#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include "loadTGA.h"
#include <ctime>
#include <string.h>
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures
float cdr=3.14159265/180.0;	//Conversion from degrees to radians;
float lookAngle = 0;		//Camera rotation
float camx = 0;      //Camera position
float camz = 900;
float gHeight = 250;
float camy = gHeight + 50;
int camSpeed = 0;
int camaSpeed = 0;
GLUquadric *q;    //Required for creating cylindrical objects

// Fountain------------------------------------------------------------
const int N = 14;  // Total number of vertices on the fountain

float vx[N] = {70, 70, 66, 66, 10, 10, 30, 5, 5, 20, 3, 3, 10, 0};
float vy[N] = {0, 10, 10, 2, 2, 10, 30, 30, 40, 55, 55, 70, 75, 75};
float vz[N] = {0};

// Water
float lx[30] = {0};
float ly[30] = {0};
float lz[30] = {0};
float theta = 0;

// Train
float trainx = -1000;
float trainz = -200;
float trainSpeed = 3;

// Controller
int ctx = 529;
int ct_speed = -1;
int ct_angle = 0;
bool ct_turning = false;
bool ct_view = false;

float grey[4] = {0.2, 0.2, 0.2, 1.0};
float white[4]  = {1.0, 1.0, 1.0, 1.0};
float black[4]  = {0, 0, 0, 1.0};
float yellow[4]  = {1, 1, 0, 1.0};
float offWhite[4] = {0.9, 0.9, 1, 1.0};

// Time
float minutes = ((time(NULL) / 60) - 660) % 1440;

// Ball
float ballx;
float bally;

// Timetable
int textMove = 0;
int iter = 0;
int textStart = 24;
int nText = 25;
char* text[] = {
    (char*)"London        5     16:55  On Time\n",\
    (char*) "Manchester    7     17:09  On Time\n",\
    (char*) "Birmingham    -     -      CANCELLED\n",\
    (char*) "Leeds         1     17:46  On Time\n",\
    (char*) "Glasgow       2     18:01  On Time\n",\
    (char*) "Liverpool     6     18:10  On Time\n",\
    (char*) "Portsmouth    5     18:18  Delayed\n",\
    (char*) "Newcastle     1     18:24  On Time\n",\
    (char*) "Sheffield     4     18:30  Delayed\n",\
    (char*) "Bristol       3     18:36  Delayed\n",\
    (char*) "Leicester     -     -      CANCELLED\n",\
    (char*) "Edinburgh     -     -      CANCELLED\n",\
    (char*) "Brighton      5     19:07  Early\n",\
    (char*) "Cardiff       1     19:14  On Time\n",\
    (char*) "Coventry      9     19:22  On Time\n",\
    (char*) "Hull          8     19:45  On Time\n",\
    (char*) "Preston       5     20:10  On Time\n",\
    (char*) "Swansea       -     -      CANCELLED\n",\
    (char*) "Milton Keynes 7     20:50  Delayed\n",\
    (char*) "Norwich       3     21:30  On Time\n",\
    (char*) "Oxford        8     22:00  On Time\n",\
    (char*) "Cambridge     1     22:15  On Time\n",\
    (char*) "York          2     22:40  Delayed\n",\
    (char*) "Exeter        6     23:30  On Time\n",\
    (char*) "Gloucester    3     00:00  On Time\n"
};


GLuint texId[15];

void loadGLTextures()				// Load bitmaps And Convert To Textures
{
    glGenTextures(15, texId); 		// Create texture ids
    // *** left ***
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("textures/left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("textures/front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("textures/right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** back***
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("textures/back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("textures/top.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** down ***
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    loadTGA("textures/down.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[6]);
    loadTGA("textures/entrance.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[7]);
    loadTGA("textures/station.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, texId[8]);
    loadTGA("textures/fountain.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[9]);
    loadTGA("textures/clock.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[10]);
    loadTGA("textures/grass.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, texId[11]);
    loadTGA("textures/floor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, texId[12]);
    loadTGA("textures/roof.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, texId[13]);
    loadTGA("textures/window.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, texId[14]);
    loadTGA("textures/controller.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

void makeWater() {
    for(int i = 0; i < 10; i++) {
        lx[i] = (i / 10.0) * 15;
        ly[i] = -0.3333 * lx[i] * lx[i] + 3.667 * lx[i] + gHeight  + 75;

        lx[i + 10] = (i / 10.0) * 10 + 15;
        ly[i + 10] = -0.8333 * lx[i + 10] * lx[i + 10] + 30.83 * lx[i + 10] + gHeight - 225;

        lx[i + 20] = (i / 9.0) * 10 + 25;
        ly[i + 20] = -0.5875 * lx[i + 20] * lx[i + 20] + 33.05 * lx[i + 20] + gHeight - 429.1;
    }
}

//-------------------------------------------------------------------

void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3, bool reverse)
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

    if(reverse) glNormal3f(-nx, -ny, -nz);
    else glNormal3f(nx, ny, nz);
}

//========================================================================================

void skybox() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    ////////////////////// LEFT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1000,  0, 1000);
    glTexCoord2f(1.0, 0);
    glVertex3f(-1000, 0., -1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1000, 1000., -1000);
    glTexCoord2f(0, 1.0);
    glVertex3f(-1000, 1000, 1000);
    glEnd();

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1000,  0, -1000);
    glTexCoord2f(1.0, 0);
    glVertex3f(1000, 0., -1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000, 1000, -1000);
    glTexCoord2f(0, 1.0);
    glVertex3f(-1000,  1000, -1000);
    glEnd();

////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(1000,  0, -1000);
    glTexCoord2f(1.0, 0);
    glVertex3f(1000, 0, 1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000, 1000,  1000);
    glTexCoord2f(0, 1.0);
    glVertex3f(1000,  1000,  -1000);
    glEnd();


    ////////////////////// REAR WALL ////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f( 1000, 0, 1000);
    glTexCoord2f(1.0, 0);
    glVertex3f(-1000, 0,  1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1000, 1000,  1000);
    glTexCoord2f(0, 1.0);
    glVertex3f( 1000, 1000, 1000);
    glEnd();

    /////////////////////// TOP //////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1000, 1000, -1000);
    glTexCoord2f(1.0, 0);
    glVertex3f(1000, 1000,  -1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000, 1000,  1000);
    glTexCoord2f(0, 1.0);
    glVertex3f(-1000, 1000, 1000);
    glEnd();

    /////////////////////// FLOOR //////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1000, 0., 1000);
    glTexCoord2f(1.0, 0);
    glVertex3f(1000, 0.,  1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000, 0., -1000);
    glTexCoord2f(0, 1.0);
    glVertex3f(-1000, 0., -1000);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

}

//---------------------------------------------------------------------

void drawFloor1()
{
    glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);

    //The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
    glBegin(GL_QUADS);
    for(int i = -1000; i < 1000; i += 5)
    {
        for(int j = -1000;  j < 1000; j += 5)
        {
            glVertex3f(i, gHeight, j);
            glVertex3f(i, gHeight, j+5);
            glVertex3f(i+5, gHeight, j+5);
            glVertex3f(i+5, gHeight, j);
        }
    }
    glEnd();

}

void drawFloor()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[10]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glNormal3f(0, 1, 0);
    glVertex3f(-1000, gHeight, 1000);
    glTexCoord2f(4, 0);
    glVertex3f(1000, gHeight, 1000);
    glTexCoord2f(4, 1.2);
    glVertex3f(1000, gHeight, 300);
    glTexCoord2f(0, 1.2);
    glVertex3f(-1000, gHeight, 300);

    glTexCoord2f(0, 2.7);
    glVertex3f(-1000, gHeight, -350);
    glTexCoord2f(4, 2.7);
    glVertex3f(1000, gHeight, -350);
    glTexCoord2f(4, 4);
    glVertex3f(1000, gHeight, -1000);
    glTexCoord2f(0, 4);
    glVertex3f(-1000, gHeight, -1000);

    glTexCoord2f(0, 1.2);
    glVertex3f(-1000, gHeight, 300);
    glTexCoord2f(0.85, 1.2);
    glVertex3f(-575, gHeight, 300);
    glTexCoord2f(0.85, 2.7);
    glVertex3f(-575, gHeight, -350);
    glTexCoord2f(0, 2.7);
    glVertex3f(-1000, gHeight, -350);

    glTexCoord2f(3.15, 1.2);
    glVertex3f(575, gHeight, 300);
    glTexCoord2f(4, 1.2);
    glVertex3f(1000, gHeight, 300);
    glTexCoord2f(4, 2.7);
    glVertex3f(1000, gHeight, -350);
    glTexCoord2f(1, 1);
    glVertex3f(575, gHeight, -350);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texId[11]);
    glBegin(GL_QUADS);
    for(int j = -350; j < 300; j += 5) {
        for(int i = -575; i < 575; i += 5) {
            glTexCoord2f((i + 575) / 287.5, (j + 350) / 162.5);
            glVertex3f(i, gHeight, j);
            glTexCoord2f((i + 575) / 287.5, (j + 355) / 162.5);
            glVertex3f(i, gHeight, j + 5);
            glTexCoord2f((i + 580) / 287.5, (j + 355) / 162.5);
            glVertex3f(i + 5, gHeight, j + 5);
            glTexCoord2f((i + 580) / 287.5, (j + 350) / 162.5);
            glVertex3f(i + 5, gHeight, j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void drawRoof()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[12]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-575, gHeight + 200, 300);
    glTexCoord2f(3, 0);
    glVertex3f(575, gHeight + 200, 300);
    glTexCoord2f(3, 1);
    glVertex3f(575, gHeight + 400, -25);
    glTexCoord2f(0, 1);
    glVertex3f(-575, gHeight + 400, -25);

    glTexCoord2f(0, 0);
    glVertex3f(575, gHeight + 200, -350);
    glTexCoord2f(3, 0);
    glVertex3f(-575, gHeight + 200, -350);
    glTexCoord2f(3, 1);
    glVertex3f(-575, gHeight + 400, -25);
    glTexCoord2f(0, 1);
    glVertex3f(575, gHeight + 400, -25);
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0);
    glVertex3f(-125, gHeight + 200, 300);
    glTexCoord2f(0, 1);
    glVertex3f(0, gHeight + 400, 300);
    glTexCoord2f(1, 1);
    glVertex3f(0, gHeight + 400, -25);

    glTexCoord2f(1, 1);
    glVertex3f(0, gHeight + 400, -25);
    glTexCoord2f(0, 1);
    glVertex3f(0, gHeight + 400, 300);
    glTexCoord2f(0, 0);
    glVertex3f(125, gHeight + 200, 300);
    glEnd();

    glColor4f(1, 1, 1, 0.8);
    glBindTexture(GL_TEXTURE_2D, texId[13]);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0);
    glVertex3f(-575, gHeight + 200, 300);
    glTexCoord2f(1.5, 1);
    glVertex3f(-575, gHeight + 400, -25);
    glTexCoord2f(3, 0);
    glVertex3f(-575, gHeight + 200, -350);

    glTexCoord2f(0, 0);
    glVertex3f(575, gHeight + 200, 300);
    glTexCoord2f(1.5, 1);
    glVertex3f(575, gHeight + 400, -25);
    glTexCoord2f(3, 0);
    glVertex3f(575, gHeight + 200, -350);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//---------------------------------------------------------------------
void drawStation()
{

    //Entrance
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[6]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.7, 0);
    glVertex3f(50, gHeight, 300);
    glTexCoord2f(1, 0);
    glVertex3f(125, gHeight, 300);
    glTexCoord2f(1, 0.5466);
    glVertex3f(125, gHeight + 200, 300);
    glTexCoord2f(0.7, 0.5466);
    glVertex3f(50, gHeight + 200, 300);


    glTexCoord2f(0.3, 0.5466);
    glVertex3f(-50, gHeight + 200, 300);
    glTexCoord2f(0, 0.5466);
    glVertex3f(-125, gHeight + 200, 300);
    glTexCoord2f(0, 0);
    glVertex3f(-125, gHeight, 300);
    glTexCoord2f(0.3, 0);
    glVertex3f(-50, gHeight, 300);

    glTexCoord2f(0.7, 0.3416);
    glVertex3f(50, gHeight + 125, 300);
    glTexCoord2f(0.7, 0.5466);
    glVertex3f(50, gHeight + 200, 300);
    glTexCoord2f(0.3, 0.5466);
    glVertex3f(-50, gHeight + 200, 300);
    glTexCoord2f(0.3, 0.3416);
    glVertex3f(-50, gHeight + 125, 300);
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(1, 0.5466);
    glVertex3f(125, gHeight + 200, 300);
    glTexCoord2f(0.5, 1);
    glVertex3f(0, gHeight + 400, 300);
    glTexCoord2f(0, 0.5466);
    glVertex3f(-125, gHeight + 200, 300);
    glEnd();

    // Station front
    glBindTexture(GL_TEXTURE_2D, texId[7]);
    glBegin(GL_QUADS);
    glTexCoord2f(9, 0);
    glVertex3f(125, gHeight, 300);
    glTexCoord2f(0, 0);
    glVertex3f(575, gHeight, 300);
    glTexCoord2f(0, 1);
    glVertex3f(575, gHeight + 200, 300);
    glTexCoord2f(9, 1);
    glVertex3f(125, gHeight + 200, 300);

    glTexCoord2f(0, 1);
    glVertex3f(-125, gHeight + 200, 300);
    glTexCoord2f(9, 1);
    glVertex3f(-575, gHeight + 200, 300);
    glTexCoord2f(9, 0);
    glVertex3f(-575, gHeight, 300);
    glTexCoord2f(0, 0);
    glVertex3f(-125, gHeight, 300);

    // Station left side
    glNormal3f(-1, 0, 0);
    for(int i = gHeight; i < gHeight + 200; i += 5) {
        for(int j = -350; j  < 300; j += 5) {
            if(i >= gHeight + 130 || j <= -255 || j >= -150) {
                glTexCoord2f(9 * ((j + 350) / 650.0), (i - gHeight) / 200.0);
                glVertex3f(-575, i, j);
                glTexCoord2f(9 * ((j + 350) / 650.0), (i - gHeight + 5) / 200.0);
                glVertex3f(-575, i + 5, j);
                glTexCoord2f(9 * (j + 355) / 650.0, (i - gHeight + 5) / 200.0);
                glVertex3f(-575, i + 5, j + 5);
                glTexCoord2f(9 * (j + 355) / 650.0, (i - gHeight) / 200.0);
                glVertex3f(-575, i, j + 5);
            }
        }
    }

    // Station right side
    glNormal3f(1, 0, 0);
    for(int i = gHeight; i < gHeight + 200; i += 5) {
        for(int j = -350; j  < 300; j += 5) {
            if(i >= gHeight + 130 || j <= -255 || j >= -150) {
                glTexCoord2f(9 * ((j + 350) / 650.0), (i - gHeight) / 200.0);
                glVertex3f(575, i, j);
                glTexCoord2f(9 * ((j + 350) / 650.0), (i - gHeight + 5) / 200.0);
                glVertex3f(575, i + 5, j);
                glTexCoord2f(9 * (j + 355) / 650.0, (i - gHeight + 5) / 200.0);
                glVertex3f(575, i + 5, j + 5);
                glTexCoord2f(9 * (j + 355) / 650.0, (i - gHeight) / 200.0);
                glVertex3f(575, i, j + 5);
            }
        }
    }

    // Station back
    glNormal3f(-1, 0, 0);

    for(int i = -575; i < 575; i += 5) {
        for(int j = gHeight; j < gHeight + 200; j += 5) {
            glTexCoord2f(((i + 575) / 1150.0) * 23, (j - gHeight) / 200.0);
            glVertex3f(i, j, -350);
            glTexCoord2f(((i + 575) / 1150.0) * 23, (j - gHeight + 5) / 200.0);
            glVertex3f(i, j + 5, -350);
            glTexCoord2f(((i + 580) / 1150.0) * 23, (j - gHeight + 5) / 200.0);
            glVertex3f(i + 5, j + 5, -350);
            glTexCoord2f(((i + 580) / 1150.0) * 23, (j - gHeight) / 200.0);
            glVertex3f(i + 5, j, -350);
        }
    }

    glEnd();
    glDisable(GL_TEXTURE_2D);

}

//---------------------------------------------------------------------
void drawFountain() {
    float wx[30], wy[30], wz[30];
    float angStep = 10.0*cdr;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    glColor3f(1, 1, 1);
    float x;
    float y;
    float z;

    for(int j = 0; j < 36; j++) {
        glBegin(GL_TRIANGLE_STRIP);

        for(int i = 0; i < N; i++) {
            wx[i] = vx[i] * cos(angStep) + vz[i] * sin(angStep);
            wy[i] = vy[i];
            wz[i] = -vx[i] * sin(angStep) + vz[i] * cos(angStep);

            if(i > 0) normal( wx[i - 1], wy[i - 1] + gHeight, wz[i - 1],
                                  x, y + gHeight, z,
                                  vx[i], vy[i] + gHeight, vz[i],
                                  true
                                );
            else glNormal3f(1, 0, 0);
            glTexCoord2f(j / 36.0, i / (float) N);
            glVertex3f(vx[i], vy[i] + gHeight, vz[i]);

            if(i > 0) normal( wx[i - 1], wy[i - 1] + gHeight, wz[i  -1],
                                  vx[i], vy[i] + gHeight, vz[i],
                                  wx[i], wy[i] + gHeight, wz[i],
                                  true
                                );
            else glNormal3f(1, 0, 0);
            glTexCoord2f((j + 1) / 36.0, i / (float) N);
            glVertex3f(wx[i], wy[i] + gHeight, wz[i]);
            printf("\n");
            x = vx[i];
            y = vy[i];
            z = vz[i];
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);

    // Water
    glDisable(GL_LIGHTING);
    glColor4f(0, 0.3, 1.0, 0.8);
    glPushMatrix();
    glTranslatef(0, gHeight + 8, 0);
    glRotatef(90, 1, 0, 0);
    gluDisk(q, 0, 67, 20, 5);
    glPopMatrix();

    angStep = 10*cdr;
    for(int j = 0; j < 36; j++) {
        glBegin(GL_TRIANGLE_STRIP);

        for(int i = 0; i < 30; i++) {
            if(i >= 29) glColor4f(0, 0.3, 1.0, 0.8);
            else glColor4f(0, 0.3, 1.0, 0.3 * cos(12 * (-1 * theta + 4 * i) * cdr) + 0.5);
            wx[i] = lx[i] * cos(angStep) + lz[i] * sin(angStep);
            wy[i] = ly[i];
            wz[i] = -lx[i] * sin(angStep) + lz[i] * cos(angStep);
            glVertex3f(lx[i], ly[i], lz[i]);
            glVertex3f(wx[i], wy[i], wz[i]);

            lx[i] = wx[i];
            ly[i] = wy[i];
            lz[i] = wz[i];
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);

}

//---------------------------------------------------------------------
void drawTrain()
{
    float tHeight = gHeight + 2;
    // Front Bit
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 20);
    glPushMatrix();
    glTranslatef(trainx,tHeight + 50, trainz);
    glRotatef(90, 0, 1, 0);
    glColor3f(0.05, 0.25, 0.05);
    gluCylinder(q, 30, 30, 100, 20, 5);
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

    glPushMatrix();
    glTranslatef(trainx + 100,tHeight + 50, trainz);
    glScalef(0.2, 1, 1);
    glColor3f(0.1, 0.1, 0.1);
    glutSolidSphere(30, 20, 20);
    glPopMatrix();

    // Light
    float lgt1_pos[] = {0, 0, 0, 1};
    float lgt_dir[] = {1, -1, 0, 1};
    glPushMatrix();
    glTranslatef(trainx + 106,tHeight + 50, trainz);
    glColor3f(1, 1, 0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt_dir);
    glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);
    glRotatef(90, 0, 1, 0);
    gluDisk(q, 0, 10, 20, 5);
    glPopMatrix();

    // Driver Bit
    glPushMatrix();
    glTranslatef(trainx - 30, tHeight + 70, trainz);
    glScalef(60, 100, 60);
    glColor3f(0.7, 0.5, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    // Windows
    glPushMatrix();
    glTranslatef(trainx - 30, tHeight + 100, trainz);
    glScalef(65, 25, 50);
    glColor3f(0.2, 0.8, 1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(trainx - 30, tHeight + 100, trainz);
    glScalef(50, 25, 65);
    glColor3f(0.2, 0.8, 1);
    glutSolidCube(1);
    glPopMatrix();

    // Top
    glPushMatrix();
    glTranslatef(trainx - 30, tHeight + 120, trainz);
    glScalef(70, 5, 70);
    glColor3f(0.4, 0.4, 0.4);
    glutSolidCube(1);
    glPopMatrix();

    // Chimney
    glPushMatrix();
    glTranslatef(trainx + 60, tHeight + 100, trainz);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.1, 0.1, 0.1);
    gluCylinder(q, 10, 10, 30, 20, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(trainx + 60, tHeight + 110, trainz);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.1, 0.1, 0.1);
    glutSolidCone(20, 20, 20, 5);
    glPopMatrix();

    // Front wheels
    glPushMatrix();
    glTranslatef(trainx + 80, tHeight + 20, trainz + 31);
    glColor3f(0.1, 0.1, 0.1);
    gluDisk(q, 0, 20, 20, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(trainx + 80, tHeight + 20, trainz - 31);
    glColor3f(0.1, 0.1, 0.1);
    gluDisk(q, 0, 20, 20, 5);
    glPopMatrix();

    // Back Wheels
    glPushMatrix();
    glTranslatef(trainx - 35, tHeight + 20, trainz + 31);
    glColor3f(0.1, 0.1, 0.1);
    gluDisk(q, 0, 20, 20, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(trainx - 35, tHeight + 20, trainz - 31);
    glColor3f(0.1, 0.1, 0.1);
    gluDisk(q, 0, 20, 20, 5);
    glPopMatrix();

    // Wheel connectors
    glPushMatrix();
    glTranslatef(trainx + 23 + 11 * cos(-10 * theta * cdr), tHeight + 20 + 11 * sin(-10 * theta * cdr), trainz + 31);
    glScalef(115, 2, 2);
    glColor3f(0.9, 0.9, 0.9);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(trainx + 23 + 11 * cos(-10 * theta * cdr), tHeight + 20 + 11 * sin(-10 * theta * cdr), trainz - 31);
    glScalef(115, 2, 2);
    glColor3f(0.9, 0.9, 0.9);
    glutSolidCube(1);
    glPopMatrix();
}

//---------------------------------------------------------------------
void drawTracks()
{
    for(int i = -1000; i < 1000; i += 50) {
        glPushMatrix();
        glTranslatef(i, gHeight, trainz);
        glScalef(20, 3, 100);
        glColor3f(0.5, 0.3, 0);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 20);
    for(int i = -1000; i < 1000; i += 5) {
        glPushMatrix();
        glTranslatef(i, gHeight + 3, trainz - 29);
        glScalef(5, 3, 2);
        glColor3f(0.5, 0.5, 0.5);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(i, gHeight + 3, trainz + 29);
        glScalef(5, 3, 2);
        glColor3f(0.5, 0.5, 0.5);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
}

//---------------------------------------------------------------------
void drawClock()
{

    // Clock face
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[9]);
    gluQuadricTexture(q, true);
    glPushMatrix();
    glTranslatef(0, gHeight + 140, -340);
    gluDisk(q, 0, 50, 20, 5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0, gHeight + 140, -350);
    glColor3f(0.1, 0.1, 0.1);
    gluCylinder(q, 50, 50, 10, 20, 1);
    glPopMatrix();

    // Pendulum
    glPushMatrix();
    glTranslatef(0, gHeight + 140, -345);
    glRotatef(15 * cos(3.6 * theta * cdr), 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.5, 0.3, 0);
    gluCylinder(q, 3, 3, 120, 20, 5);
    glPopMatrix();

    // Weight
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 20);
    glPushMatrix();
    glTranslatef(0, gHeight + 140, -345);
    glRotatef(15 * cos(3.6 * theta * cdr), 0, 0, 1);
    glTranslatef(0, -120, 0);
    glScalef(1, 1, 0.3);
    glColor3f(0.9, 0.7, 0.1);
    gluSphere(q, 15, 20, 20);
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

    // Hour hand
    glPushMatrix();
    glTranslatef(0, gHeight + 140, -339);
    glRotatef(-30 * (minutes / 60.0), 0, 0, 1);
    glTranslatef(0, 8, 0);
    glScalef(3, 30, 1);
    glColor3f(0, 0, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Minute hand
    glPushMatrix();
    glTranslatef(0, gHeight + 140, -339);
    glRotatef(-6 * minutes, 0, 0, 1);
    glTranslatef(0, 15, 0);
    glScalef(3, 45, 1);
    glColor3f(0, 0, 0);
    glutSolidCube(1.0);
    glPopMatrix();

}

void drawController()
{
    float walk = 45 * sin(ctx / 9.0);
    // Torso
    glPushMatrix();
    glTranslatef(ctx, gHeight + 36, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glRotatef(-15, 0, 0, 1);
    glScalef(10, 20, 16);
    glColor3f(0.1, 0.1, 0.1);
    gluSphere(q, 1, 30, 30);
    glPopMatrix();

    // Legs
    glPushMatrix();
    glTranslatef(ctx, gHeight + 24, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(-1, 0, -5);
    glRotatef(walk, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.6, 0.6, 0.6);
    gluCylinder(q, 7, 4, 18, 20, 5);
    glTranslatef(0, 0, 20);
    glRotatef(180, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.1, 0.1, 0.1);
    glutSolidTeapot(5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ctx, gHeight + 24, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(-1, 0, 5);
    glRotatef(-walk, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.6, 0.6, 0.6);
    gluCylinder(q, 7, 4, 18, 20, 5);
    glTranslatef(0, 0, 20);
    glRotatef(180, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.1, 0.1, 0.1);
    glutSolidTeapot(5);
    glPopMatrix();

    // Arms
    glPushMatrix();
    glTranslatef(ctx, gHeight + 46, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(2, 0, -7);
    glRotatef(-walk, 0, 0, 1);
    glRotatef(120, 1, 0, 0);
    glColor3f(0.1, 0.1, 0.1);
    gluCylinder(q, 5, 3, 25, 20, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ctx, gHeight + 46, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(2, 0, 7);
    glRotatef(walk, 0, 0, 1);
    glRotatef(60, 1, 0, 0);
    glColor3f(0.1, 0.1, 0.1);
    gluCylinder(q, 5, 3, 25, 20, 5);
    glPopMatrix();

    // Hands
    glPushMatrix();
    glTranslatef(ctx, gHeight + 46, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(2, -3, -20.5);
    glRotatef(-walk, 0, 0, 1);
    glTranslatef(0, -20, 0);
    glColor3f(1, 0.85, 0.75);
    glutSolidSphere(3, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ctx, gHeight + 46, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(2, -3, 20.5);
    glRotatef(walk, 0, 0, 1);
    glTranslatef(0, -20, 0);
    glColor3f(1, 0.85, 0.75);
    glutSolidSphere(3, 10, 10);
    glPopMatrix();

    // Head
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[14]);
    glPushMatrix();
    glTranslatef(ctx, gHeight + 63, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(-2, 0, 0);
    glRotatef(-20, 0, 0, 1);
    glScalef(6, 8, 9);
    glColor3f(1, 1, 1);
    gluSphere(q, 1, 30, 30);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // Neck
    glPushMatrix();
    glTranslatef(ctx, gHeight + 59, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(-1, 0, 0);
    glRotatef(15, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glColor3f(1, 0.85, 0.75);
    gluCylinder(q, 4, 4, 15, 30, 30);
    glPopMatrix();

    // Hat
    glPushMatrix();
    glTranslatef(ctx, gHeight + 66, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(-0.5, 0, 0);
    glRotatef(-20, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glScalef(1, 1.2, 1);
    glColor3f(0.1, 0.1, 0.1);
    gluDisk(q, 0, 9.5, 20, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ctx, gHeight + 75.5, trainz + 100);
    glRotatef(ct_angle, 0, 1, 0);
    glTranslatef(2.6, 0, 0);
    glRotatef(-20, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glScalef(1, 1.2, 1);
    glColor3f(0.1, 0.1, 0.1);
    gluCylinder(q, 8, 7, 10, 20, 5);
    glPopMatrix();
}

//---------------------------------------------------------------------
void drawTimetable()
{
    // Board
    glPushMatrix();
    glTranslatef(200, gHeight + 65, 297);
    glScalef(100, 100, 5);
    glColor3f(0.1, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(252, gHeight + 65, 295);
    glScalef(4, 108, 8);
    glColor3f(0.4, 0.4, 0.4);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(148, gHeight + 65, 295);
    glScalef(4, 108, 8);
    glColor3f(0.4, 0.4, 0.4);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(200, gHeight + 117, 295);
    glScalef(100, 4, 8);
    glColor3f(0.4, 0.4, 0.4);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(200, gHeight + 13, 295);
    glScalef(100, 4, 8);
    glColor3f(0.4, 0.4, 0.4);
    glutSolidCube(1);
    glPopMatrix();

    // Text
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(248, gHeight + 110, 294);
    glRotatef(180, 0, 1, 0);
    glScalef(0.025, 0.03, 1);
    glColor3f(1, 0, 0);
    glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) "Destination   Plat. Time  Status\n");
    glPopMatrix();

    glRasterPos2f(0, 0);
    glPushMatrix();
    glTranslatef(248, gHeight + 103 - textMove / 10.0, 294);
    glRotatef(180, 0, 1, 0);
    glScalef(0.025, 0.03, 1);
    glColor3f(1, 0.6, 0);
    for(int i = 0; i < 20; i++) {
        glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) text[(i + textStart) % nText]);
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);

}

//---------------------------------------------------------------------
void initialise(void)
{

    loadGLTextures();
    q = gluNewQuadric();
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, offWhite);
    glLightfv(GL_LIGHT0, GL_SPECULAR, offWhite);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT1, GL_SPECULAR, yellow);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
    glEnable(GL_SMOOTH);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor (1.0, 1.0, 1.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(70.0, 1.0, 1, 5000.0);   //Perspective projection

    makeWater();
}

//---------------------------------------------------------------------
void display(void)
{
    float lgt_pos[] = {0, gHeight + 500, 500, 1.0f};  //light0 position (directly above the origin)

    float xlook, zlook;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    xlook = -100.0*sin(lookAngle*cdr) + camx;
    zlook = -100.0*cos(lookAngle*cdr) + camz;
    if(!ct_view) gluLookAt (camx, camy, camz, xlook, camy, zlook, 0, 1, 0);  //camera rotation
    else gluLookAt(ctx - 10 * cos(ct_angle * cdr), gHeight + 65, trainz + 100 + 10 * sin(ct_angle * cdr), ctx - 11 * cos(ct_angle * cdr), gHeight + 65, trainz + 100 + 11 * sin(ct_angle * cdr), 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

    skybox();
    drawFloor();
    drawRoof();
    drawStation();
    drawTimetable();
    drawTrain();
    drawTracks();
    drawClock();
    drawController();
    drawFountain();

    glFlush();
    glutSwapBuffers();
}

//--------------------------------------------------------------
void special(int key, int x, int y)
{
    if(!ct_view) {
        if(key == GLUT_KEY_UP) {
            camSpeed = 4;
        }
        else if(key == GLUT_KEY_DOWN) {
            camSpeed = -4;
        }
    }
    if(key==GLUT_KEY_LEFT) camaSpeed = 2;		 //Turn left
    else if(key==GLUT_KEY_RIGHT) camaSpeed = -2;   //Turn right
    if(key==GLUT_KEY_F1) {
        ct_view = !ct_view;
    }

    glutPostRedisplay();
}

//---------------------------------------------------------------------
void specialUp (int key, int x, int y)
{
    if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
        camSpeed = 0;
    }
    else if(key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
        camaSpeed = 0;

    }
}

//---------------------------------------------------------------------
void myTimer(int value)
{
    theta += 1;
    glutPostRedisplay();
    glutTimerFunc(20, myTimer, 0);
    minutes += 0.02 / 60;

    trainx = (fmod((trainx + 1000 + trainSpeed), 2200)) - 1000;

    camx -= sin(lookAngle * cdr) * camSpeed;
    camz -= cos(lookAngle * cdr) * camSpeed;
    lookAngle += camaSpeed;

    if(abs(ctx) > 530)
    {
        ctx -= ct_speed;
        ct_speed *= -1;
        ct_turning = true;
    }

    if(!ct_turning) ctx += ct_speed;
    else {
        ct_angle += 3;
        if(ct_angle == 180) ct_turning = false;
        else if(ct_angle == 360) ct_angle = ct_turning = false;
    }

    if(textMove) {
        textMove = (textMove + 1) % 45;
        if(!textMove % 45) {
            textStart = (textStart + nText - 1) % nText;
        }
    }
    else {
        iter += 1;
        if(!(iter %= 100)) {
            textMove = 1;
        }
    }
}
//-------------------------------------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize (700, 700);
    glutInitWindowPosition (50, 50);

    glutCreateWindow ("Assignment");
    initialise();
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutTimerFunc(20, myTimer, 0);

    glutMainLoop();
    return 0;
}
