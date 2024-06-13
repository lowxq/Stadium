#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <GL/glut.h>
#include <gl/glut.h> 
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <codecvt>
#include <cmath>
using namespace std;

int cw;
int ch;
int cTurn = 180; //current facing

bool isStand = true;
float curDistanceX = 0, curDistanceZ = 0;//current dist
float legDis = 0;

// Camera variables
float cameraX = 0.0f, cameraY = 10.0f, cameraZ = -145.0f;
float cameraYaw = 0.0f, cameraPitch = 0.0f;
float cameraDistance = 15.0f; // Distance between the camera and the target
bool isMouseDragging = false;
int mouseX, mouseY;
GLfloat angle = 0.0;

float carX = -70.0f; // Car's X position
float carZ = -137.0f; // Car's Z position
float carSpeed = 0.08f; // Car's speed
float car3Z = -160.0f;
float car3X = -70.0f; // Car's X position
float car3Speed = 0.08f;  // Speed of the third car

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint texture[50]; //the array for our texture
GLfloat DarkGrey[] = { 0.663, 0.663, 0.663, 1.0 };
GLfloat GrassGreen[] = { 0.0, 0.7, 0.0 };
GLfloat WindowBlue[] = { 0.529f, 0.808f, 0.922f, 1.0f };
GLfloat Peach[] = { 0.9f, 0.6f, 0.6f, 1.0f };
GLfloat LightBrown[] = { 0.8f, 0.6f, 0.2f, 1.0f };
GLfloat LightLightBrown[] = { 0.8f, 0.6f, 0.2f, 1.0f };
GLfloat StageBrown[] = { 0.95f, 0.87f, 0.6f, 1.0f };
GLfloat DarkBlue[] = { 0.0f, 0.0f, 0.4f, 1.0f };
GLfloat White[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightYellow[] = { 1.0f, 1.0f, 0.8f, 1.0f };

GLfloat vertices[][3] = { { -1.0, -1.0, -1.0 },{ 1.0, -1.0, -1.0 },
{ 1.0, 1.0, -1.0 },{ -1.0, 1.0, -1.0 },{ -1.0, -1.0, 1.0 },
{ 1.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ -1.0, 1.0, 1.0 } };

GLuint LoadTexture(const char* filename, int width, int height) {
    GLuint texture;
    unsigned char* data;
    FILE* file;
    //The following code will read in our RAW file
    file = fopen(filename, "rb");
    if (file == NULL) return 0;
    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);
    glGenTextures(1, &texture); //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to its array
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Generate the texture
    //Please be informed to switch from GL_RGB to GL_BGR_EXT for bitmap image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    free(data); //free the texture
    return texture; //return whether it was successful
}

void FreeTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}

void playBackgroundMusic(const std::string& bgm) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wideFilename = converter.from_bytes(bgm);
    PlaySound(wideFilename.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}


GLfloat vertice3[8][3] = {
  {-0.5, -0.5, 0.5},
  {0.5, -0.5, 0.5},
  {0.5, 0.5, 0.5},
  {-0.5, 0.5, 0.5},
  {-0.5, -0.5, -0.5},
  {0.5, -0.5, -0.5},
  {0.5, 0.5, -0.5},
  {-0.5, 0.5, -0.5}
};
GLfloat vertice1[8][3] = {
  {-0.6, -0.3, 0.6},
  {0.6, -0.3, 0.6},
  {0.6, 0.3, 0.6},
  {-0.6, 0.3, 0.6},
  {-0.6, -0.3, -0.6},
  {0.6, -0.3, -0.6},
  {0.6, 0.3, -0.6},
  {-0.6, 0.3, -0.6}
};
void cylinder(int segments, float radius, float height) {
    int k;
    float angles;
    float x, z;

    glPushMatrix();
    glTranslatef(0.0, height / 2, 0.0);
    glBegin(GL_POLYGON);
    for (k = 0; k < segments; k++) {
        angles = 2.0 * 3.141592654f * k / segments;
        x = radius * cos(angles);
        z = radius * sin(angles);
        glVertex3f(x, 0, z);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -height / 2, 0.0);
    glBegin(GL_POLYGON);
    for (k = 0; k < segments; k++) {
        angles = 2.0 * 3.141592654f * k / segments;
        x = radius * cos(angles);
        z = radius * sin(angles);
        glVertex3f(x, 0, z);
    }
    glEnd();
    glPopMatrix();


    glBegin(GL_QUAD_STRIP);
    for (k = 0; k <= segments; k++) {
        angles = 2.0 * 3.141592654f * k / segments;
        x = radius * cos(angles);
        z = radius * sin(angles);
        glVertex3f(x, -height / 2, z);
        glVertex3f(x, height / 2, z);
    }
    glEnd();
}

void polygon(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    glVertex3fv(vertice3[a]);
    glVertex3fv(vertice3[b]);
    glVertex3fv(vertice3[c]);
    glVertex3fv(vertice3[d]);
    glEnd();
}

void polygon1(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    glVertex3fv(vertice1[a]);
    glVertex3fv(vertice1[b]);
    glVertex3fv(vertice1[c]);
    glVertex3fv(vertice1[d]);
    glEnd();
}

void cube()
{
    polygon(0, 1, 2, 3);
    polygon(4, 5, 6, 7);
    polygon(0, 4, 7, 3);
    polygon(1, 5, 6, 2);
    polygon(3, 2, 6, 7);
    polygon(0, 1, 5, 4);
}

void cube1()
{
    polygon1(0, 1, 2, 3);
    polygon1(4, 5, 6, 7);
    polygon1(0, 4, 7, 3);
    polygon1(1, 5, 6, 2);
    polygon1(3, 2, 6, 7);
    polygon1(0, 1, 5, 4);
}

void square()
{
    glBegin(GL_POLYGON);
    glVertex3f(-0.3, 0.0, 0.5); // bottom left
    glVertex3f(-0.1, 0, 0.5); // bottom right
    glVertex3f(-0.1, 0.2, 0.5); // top right
    glVertex3f(-0.3, 0.2, 0.5); // top left
    glEnd();
}

void mouth()
{
    glBegin(GL_POLYGON);
    glVertex3f(-0.35, -0.1, 0.5); // bottom left
    glVertex3f(-0.2, -0.25, 0.5); // bottom right
    glVertex3f(0.1, -0.25, 0.5); // top right
    glVertex3f(0.25, -0.1, 0.5); // top left
    glVertex3f(0.2, -0.1, 0.5); // bottom left
    glVertex3f(0.1, -0.2, 0.5); // bottom right
    glVertex3f(-0.2, -0.2, 0.5); // bottom left
    glVertex3f(-0.3, -0.1, 0.5); // bottom right

    glEnd();
}

void ManniCharacter() {

    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);

    glPushMatrix();

    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0); //turn

    glTranslatef(0.375, 0.0, 0.0);

    //rl
    glTranslatef(0.0, 0.7, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    //rl
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    //ll
    glPushMatrix();
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0); //turn

    //ll
    glTranslatef(-0.375, 0.0, 0.0);
    glTranslatef(0.0, 0.7, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //body
    glTranslatef(0.0, 2.0, 0.0);
    glColor3f(0.0, 0.0, 0.5);
    glPushMatrix();
    glScalef(1.4, 1.75, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    //head
    glTranslatef(0.0, 1.55, 0.0);
    glColor3f(1.0, 0.75, 0.0);
    glPushMatrix();
    glScalef(0.8, 0.8, 0.8);
    glutSolidSphere(0.75, 20, 20);
    glPopMatrix();

    //eyes
    glTranslatef(0.125, 0.1, -0.6);
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glScalef(0.1, 0.1, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(-0.25, 0.0, 0.0);
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glScalef(0.1, 0.1, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();

    //mouth
    glTranslatef(0.125, -0.25, 0.02);
    glColor3f(0.8, 0.2, 0.2);
    glPushMatrix();
    glScalef(0.3, 0.05, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    //ra
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //ra
    glTranslatef(0.85, 1.80, 0.0);

    glTranslatef(0.0, 0.5, 0.0);
    glColor3f(1.0, 0.75, 0.0);
    glPushMatrix();
    glScalef(0.35, 1.0, 0.45);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    //la
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //la
    glTranslatef(-0.85, 1.8, 0.0);

    glTranslatef(0.0, 0.5, 0.0);
    glColor3f(1.0, 0.75, 0.0);
    glPushMatrix();
    glScalef(0.35, 1.0, 0.45);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    //legs
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //rl
    glTranslatef(0.375, 0.0, 0.0);
    glTranslatef(0.0, 0.5, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    //rl
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);
    //  if (walk) {
          // Adjust leg angle for walking animation
     //     glRotatef(walkingLegAngle / 3, 1.0, 0.0, 0.0);
    //  }
    glutSolidCube(1.0);
    glPopMatrix();
    //ll
    glTranslatef(-0.75, 0.0, 0.0);
    // if (walk) {
         // Adjust leg angle for walking animation
  //       glRotatef(-walkingLegAngle / 3, 1.0, 0.0, 0.0);
   //  }
    glTranslatef(0.0, -0.05, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);

    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

void drawManniCharacter() {
    ManniCharacter();
}
void hand()
{
    glPushMatrix();//right hand 1
    glTranslatef(0.3, 0.6, 0.1);
    glScalef(0.17, 0.17, 0.17);

    glColor3f(1.0, 1.0, 0.0); // yellow
    cube();
    glPopMatrix();

    glPushMatrix();//right hand 2
    glTranslatef(0.3, 0.45, 0.125);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(1.0, 1.0, 0.0); // yellow
    glRotatef(22.5, 1.0, 0.0, 0.0);
    cube();
    glPopMatrix();


}
void drawLowCharacter() {

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0); // Yellow
    glTranslatef(0.0, 1.0, 0.10);
    glScalef(0.1, 0.1, 0.1);
    cylinder(24, 1.0, 2.0);
    glPopMatrix();

    glPushMatrix(); //body
    glTranslatef(0, 0.6, 0.1);
    glScalef(0.45, 0.45, 0.45);
    glColor3f(0.0, 0.0, 1.0); // blue
    cube();
    glPopMatrix();

    glPushMatrix(); //hand
    hand();
    glPopMatrix();

    glPushMatrix();//left hand 1
    glTranslatef(-0.3, 0.6, 0.1);
    glScalef(0.17, 0.17, 0.17);

    glColor3f(1.0, 1.0, 0.0); // yellow
    cube();
    glPopMatrix();

    glPushMatrix();//left hand 2
    glTranslatef(-0.3, 0.45, 0.125);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(1.0, 1.0, 0.0); // yellow
    glRotatef(22.5, -1.0, 0.0, 0.0);
    cube();
    glPopMatrix();

    glPushMatrix();//right leg 1
    glTranslatef(0.14, 0.3, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green

    cube();
    glPopMatrix();

    glPushMatrix();//right leg 3
    glTranslatef(0.14, 0.15, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green
    cube();
    glPopMatrix();

    glPushMatrix();//right leg 2
    glTranslatef(0.14, 0.00, 0.05);
    glScalef(0.19, 0.19, 0.19);
    glColor3f(0.5f, 1.0f, 1.0f); // green
    //   if (walk) {
           // Adjust leg angle for walking animation
      //     glRotatef(walkingLegAngle, 1.0, 0.0, 0.0);
      // }
    cube1();
    glPopMatrix();

    glPushMatrix();//left leg 1
    glTranslatef(-0.14, 0.3, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green
    cube();
    glPopMatrix();

    glPushMatrix();//left leg 3
    glTranslatef(-0.14, 0.15, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green
    cube();
    glPopMatrix();

    glPushMatrix();//left leg 2
    glTranslatef(-0.14, 0.0, 0.05);
    glScalef(0.19, 0.19, 0.19);
    glColor3f(0.5f, 1.0f, 1.0f); // green
    // if (walk) {
         // Adjust leg angle for walking animation
      //   glRotatef(-walkingLegAngle, 1.0, 0.0, 0.0);
    // }
    cube1();
    glPopMatrix();

    glPushMatrix();//eye 1
    glColor3f(0, 0, 0.0); // black
    glTranslatef(0.0, 1, 0.12);
    glScalef(0.17, 0.17, 0.17);
    square();
    glPopMatrix();

    glPushMatrix();//eye 2
    glColor3f(0, 0, 0.0); // black
    glTranslatef(0.05, 1, 0.12);
    glScalef(0.17, 0.17, 0.17);
    square();
    glPopMatrix();

    glPushMatrix();//mouth
    glColor3f(0, 0, 0.0); // black
    glTranslatef(0.00, 1, 0.12);
    glScalef(0.17, 0.17, 0.17);
    mouth();
    glPopMatrix();

}

void body3(void)
{
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.2);
    glScalef(2.5, 3, 1.5);
    glColor3f(0.0, 0.75, 0.75);
    glutSolidCube(0.3);
    glPopMatrix();
}

void head3() {
    //neck
    glPushMatrix();
    glTranslatef(0.0, 0.525, 0.24);
    glScalef(0.5, 0.5, 0.5);
    glColor3f(1, 0.68, 0.68);
    glutSolidCube(0.3);
    glPopMatrix();

    //head
    glPushMatrix();
    glTranslatef(0.0, 0.75, 0.24);
    glScalef(1.5, 1.0, 1.35);
    glColor3f(1, 0.68, 0.68);
    glutSolidCube(0.3);
    glPopMatrix();

    //right eye
    glPushMatrix();
    glTranslatef(0.15, (3 + 1 + 2 * 2 / 3) * 0.3 * 0.5, 0.45);
    glScalef(0.15, 0.15, 0.05);
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    //left eye
    glPushMatrix();
    glTranslatef(-0.15, (3 + 1 + 2 * 2 / 3) * 0.3 * 0.5, 0.45);
    glScalef(0.15, 0.15, 0.05);
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    //mouth n nose
    glPushMatrix();
    glTranslatef(0, 0.735, 0.45);
    glScalef(0.15, 0.15, 0);
    glColor3f(0, 0, 0);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.66, 0.45);
    glScalef(0.85, 0.15, 0);
    glColor3f(0, 0, 0);
    glutSolidCube(0.3);
    glPopMatrix();
}

void leftArm3()
{
    //left
    glPushMatrix();
    glTranslatef(-0.45, 0.2625, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(0.0, 0.85, 0.85);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.45, -0.15, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.45, 0.0375, 0.225);
    glScalef(0.5, 0.25, 0.5);
    glColor3f(1, 0.5, 0.5);
    glutSolidCube(0.3);
    glPopMatrix();
}

void rightArm3()
{
    glPushMatrix();
    glTranslatef(0.45, 0.2625, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(0.0, 0.85, 0.85);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.45, -0.15, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.45, 0.0375, 0.225);
    glScalef(0.5, 0.25, 0.5);
    glColor3f(1, 0.5, 0.5);
    glutSolidCube(0.3);
    glPopMatrix();
}

void leftLeg3()
{
    //left
    glPushMatrix();
    glTranslatef(-0.2625, -0.675, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(0, 0, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2625, -1.05, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);

    glScalef(1.25, 0.25, 1.0);
    glTranslatef(0, -0.75, 0);
    glColor3f(0.3, 0, 0);
    //if (walk) {
        // Adjust leg angle for walking animation
    //    glRotatef(-walkingLegAngle, 1.0, 0.0, 0.0);
   // }
    glutSolidCube(0.3);
    glPopMatrix();
}

void rightLeg3()
{
    glPushMatrix();
    glTranslatef(0.2625, -0.675, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(0, 0, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2625, -1.05, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);

    glScalef(1.25, 0.25, 1.0);
    glTranslatef(0, -0.75, 0);
    glColor3f(0.3, 0, 0);
    //   if (walk) {
           // Adjust leg angle for walking animation
        //   glRotatef(walkingLegAngle, 1.0, 0.0, 0.0);
    //   }
    glutSolidCube(0.3);
    glPopMatrix();
}

void drawLimCharacter()
{
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    body3();
    head3();
    leftArm3();
    rightArm3();
    leftLeg3();
    rightLeg3();
    glPopMatrix();
}

void drawBus() {
    // Bus body
    glColor3f(0.7, 0.7, 0.7); // Light grey color for the bus body
    glPushMatrix();
    glScalef(3.0, 1.0, 1.0);
    glutSolidCube(2.0);
    glPopMatrix();

    // Bus roof
    glColor3f(0.6, 0.6, 0.6); // Slightly darker grey for the roof
    glPushMatrix();
    glTranslatef(0.0, 1.25, 0.0);
    glScalef(2.5, 0.5, 0.9);
    glutSolidCube(2.0);
    glPopMatrix();

    // Windows
    glColor3f(0.6, 0.8, 1.0);

    // Front windows
    glPushMatrix();
    glTranslatef(3.0, 0.25, 0.0); // Adjusted position for front window
    glRotatef(90, 0.0, 1.0, 0.0); // Rotate 90 degrees around the y-axis
    glScalef(1.5, 1.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.0, 0.25, 0.0); // Adjusted position for front window
    glRotatef(90, 0.0, 1.0, 0.0); // Rotate 90 degrees around the y-axis
    glScalef(1.5, 1.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Rear windows
    glPushMatrix();
    glTranslatef(-3.0, 0.25, 0.0); // Adjusted position for rear window
    glRotatef(-90, 0.0, 1.0, 0.0); // Rotate -90 degrees around the y-axis
    glScalef(1.5, 1.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.0, 0.25, 0.0); // Adjusted position for rear window
    glRotatef(-90, 0.0, 1.0, 0.0); // Rotate -90 degrees around the y-axis
    glScalef(1.5, 1.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Side windows
    for (float x = 1.5; x >= -1.5; x -= 1.0) {
        glPushMatrix();
        glTranslatef(x, 0.5, 1.0);
        glScalef(0.7, 0.5, 0.1);
        glutSolidCube(1.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(x, 0.5, -1.0);
        glScalef(0.7, 0.5, 0.1);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Wheels
    glColor3f(0.2, 0.2, 0.2); // Dark grey color for the wheels
    glPushMatrix();
    glTranslatef(2.0, -1.0, 1.0);
    glutSolidTorus(0.2, 0.4, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.0, -1.0, -1.0);
    glutSolidTorus(0.2, 0.4, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.0, -1.0, 1.0);
    glutSolidTorus(0.2, 0.4, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.0, -1.0, -1.0);
    glutSolidTorus(0.2, 0.4, 10, 10);
    glPopMatrix();
}

void drawCar1() {
    // Car body
    glColor3f(0.8, 0.0, 0.0); // Red color for the car body
    glPushMatrix();
    glScalef(2.0, 0.5, 1.0);
    glutSolidCube(2.0);
    glPopMatrix();

    // Car roof
    glColor3f(0.9, 0.0, 0.0); // Slightly different red color for the roof
    glPushMatrix();
    glTranslatef(0.0, 0.75, 0.0);
    glScalef(1.2, 0.5, 0.8);
    glutSolidCube(2.0);
    glPopMatrix();

    // Windows
    glColor3f(0.8, 0.9, 1.0); // Very light blue color for windows
    // Front windshield
    glPushMatrix();
    glTranslatef(1.1, 0.6, 0.0);
    glRotatef(30, 0.0, 0.0, 1.0);
    glScalef(0.6, 0.4, 0.81);
    glutSolidCube(1.0);
    glPopMatrix();
    // Back windshield
    glPushMatrix();
    glTranslatef(-1.1, 0.6, 0.0);
    glRotatef(-30, 0.0, 0.0, 1.0);
    glScalef(0.6, 0.4, 0.81);
    glutSolidCube(1.0);
    glPopMatrix();

    // Side windows 
    glPushMatrix();
    glTranslatef(0.0, 0.75, 0.79);
    glScalef(0.9, 0.4, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.75, -0.79);
    glScalef(0.9, 0.4, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Wheels
    glColor3f(0.1, 0.1, 0.1); // Dark gray color for the wheels
    glPushMatrix();
    glTranslatef(1.0, -0.5, 1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.0, -0.5, 1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0, -0.5, -1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.0, -0.5, -1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
}

void drawCar2() {
    // Car body
    glColor3f(0.337f, 0.512f, 0.941f); // Royal blue
    glPushMatrix();
    glScalef(2.0, 0.5, 1.0);
    glutSolidCube(2.0);
    glPopMatrix();

    // Car roof and cabin
    glColor3f(0.486f, 0.633f, 0.933f); // Lighter blue for the roof
    glPushMatrix();
    glTranslatef(0.0, 0.75, 0.0);
    glScalef(1.2, 0.5, 0.8);
    glutSolidCube(2.0);
    glPopMatrix();

    // Windows
    glColor3f(0.8, 0.9, 1.0); // Very light blue color for windows
    // Front windshield
    glPushMatrix();
    glTranslatef(1.1, 0.6, 0.0);
    glRotatef(30, 0.0, 0.0, 1.0);
    glScalef(0.6, 0.4, 0.81);
    glutSolidCube(1.0);
    glPopMatrix();
    // Back windshield
    glPushMatrix();
    glTranslatef(-1.1, 0.6, 0.0);
    glRotatef(-30, 0.0, 0.0, 1.0);
    glScalef(0.6, 0.4, 0.81);
    glutSolidCube(1.0);
    glPopMatrix();

    // Side windows 
    glPushMatrix();
    glTranslatef(0.0, 0.75, 0.79);
    glScalef(0.9, 0.4, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.75, -0.79);
    glScalef(0.9, 0.4, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Wheels
    glColor3f(0.1, 0.1, 0.1); // Dark gray color for the wheels
    glPushMatrix();
    glTranslatef(1.0, -0.5, 1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.0, -0.5, 1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0, -0.5, -1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.0, -0.5, -1.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glutSolidTorus(0.125, 0.25, 10, 10);
    glPopMatrix();
}

void stadium()
{
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-left corner
    glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-right corner
    glVertex3f(9.1f, 0.0f, 7.0f);   // Top-right corner
    glVertex3f(-10.0f, 0.0f, 7.0f);  // Top-left corner
    glEnd();



    // Draw the back face
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-left corner
    glVertex3f(-9.0f, 0.0f, -10.0f);
    glVertex3f(-9.0f, -1.0f, -10.0f);
    glVertex3f(-10.0f, -1.0f, -10.0f); // Top-left corner
    glEnd();

    // Draw the middle quad
    glBegin(GL_QUADS);
    glVertex3f(-9.0f, 0.0f, -10.0f);
    glVertex3f(8.0f, 0.0f, -10.0f);
    glVertex3f(8.0f, -1.0f, -10.0f);
    glVertex3f(-9.0f, -1.0f, -10.0f);
    glEnd();

    // Draw the right quad
    glBegin(GL_QUADS);
    glVertex3f(8.0f, 0.0f, -10.0f);
    glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-right corner
    glVertex3f(9.1f, -2.0f, -10.0f); // Top-right corner
    glVertex3f(8.0f, -2.0f, -10.0f);
    glEnd();



    // Draw the left face
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-front corner
    glVertex3f(-10.0f, 0.0f, 7.0f);  // Bottom-back corner
    glVertex3f(-10.0f, -2.0f, 7.0f);// Top-back corner
    glVertex3f(-10.0f, -2.0f, -10.0f);// Top-front corner
    glEnd();

    // Draw the right face
    glBegin(GL_QUADS);
    glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-front corner
    glVertex3f(9.1f, 0.0f, 7.0f);   // Bottom-back corner
    glVertex3f(9.1f, -2.0f, 7.0f); // Top-back corner
    glVertex3f(9.1f, -2.0f, -10.0f);// Top-front corner
    glEnd();
    glEnable(GL_TEXTURE_2D);


}
void bumbung()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[38]);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-left corner
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(9.1f, 0.0f, -10.0f);   // Bottom-right corner
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(9.1f, 0.0f, 7.0f);     // Top-right corner
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-10.0f, 0.0f, 7.0f);   // Top-left corner
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void navyBlueWall()
{
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    // Front Face
    glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glVertex3f(1.0f, 1.0f, 1.0f);// Top Right Of The Texture and Quad
    glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    // Top Face
    glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

    // Right face
    glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
    glEnable(GL_TEXTURE_2D);
}
void road()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[33]);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    // Draw the front face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom Right
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(1.0f, 1.0f, 1.0f);   // Top Right
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);  // Top Left
    glEnd();

    // Draw the back face
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Right
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);   // Top Left
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Left
    glEnd();

    // Draw the top face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);   // Bottom Left
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Right
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(1.0f, 1.0f, -1.0f);   // Top Right
    glEnd();

    // Draw the bottom face
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Top Left
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);   // Bottom Left
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Right
    glEnd();

    // Draw the right face
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Right
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(1.0f, 1.0f, -1.0f);   // Top Right
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);    // Top Left
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);   // Bottom Left
    glEnd();

    // Draw the left face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Right
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(-1.0f, 1.0f, 1.0f);   // Top Right
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void RedRectangle()
{
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-left corner
    glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-right corner
    glVertex3f(9.1f, 0.0f, 7.0f);   // Top-right corner
    glVertex3f(-10.0f, 0.0f, 7.0f);  // Top-left corner
    glEnd();

    // Draw the front face
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, 7.0f);  // Bottom-left corner
    glVertex3f(9.1f, 0.0f, 7.0f);   // Bottom-right corner
    glVertex3f(9.1f, -2.0f, 7.0f); // Top-right corner
    glVertex3f(-10.0f, -2.0f, 7.0f);// Top-left corner
    glEnd();

    // Draw the back face
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-left corner
    glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-right corner
    glVertex3f(9.1f, -2.0f, -10.0f);// Top-right corner
    glVertex3f(-10.0f, -2.0f, -10.0f);// Top-left corner
    glEnd();

    // Draw the left face
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-front corner
    glVertex3f(-10.0f, 0.0f, 7.0f);  // Bottom-back corner
    glVertex3f(-10.0f, -2.0f, 7.0f);// Top-back corner
    glVertex3f(-10.0f, -2.0f, -10.0f);// Top-front corner
    glEnd();

    // Draw the right face
    glBegin(GL_QUADS);
    glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-front corner
    glVertex3f(9.1f, 0.0f, 7.0f);   // Bottom-back corner
    glVertex3f(9.1f, -2.0f, 7.0f); // Top-back corner
    glVertex3f(9.1f, -2.0f, -10.0f);// Top-front corner
    glEnd();
    glEnable(GL_TEXTURE_2D);
}
void rectangle()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[27]);
    glColor3f(1.0, 1.0, 1.0);
    // Draw the top face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-left corner
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-right corner
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(9.1f, 0.0f, 7.0f);   // Top-right corner
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-10.0f, 0.0f, 7.0f);  // Top-left corner
    glEnd();

    // Draw the front face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-10.0f, 0.0f, 7.0f);  // Bottom-left corner
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(9.1f, 0.0f, 7.0f);   // Bottom-right corner
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(9.1f, -2.0f, 7.0f);  // Top-right corner
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-10.0f, -2.0f, 7.0f); // Top-left corner
    glEnd();

    // Draw the back face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-left corner
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-right corner
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(9.1f, -2.0f, -10.0f); // Top-right corner
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-10.0f, -2.0f, -10.0f); // Top-left corner
    glEnd();

    // Draw the left face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom-front corner
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(-10.0f, 0.0f, 7.0f);  // Bottom-back corner
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(-10.0f, -2.0f, 7.0f); // Top-back corner
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-10.0f, -2.0f, -10.0f); // Top-front corner
    glEnd();

    // Draw the right face
    glBegin(GL_QUADS);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(9.1f, 0.0f, -10.0f);  // Bottom-front corner
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(9.1f, 0.0f, 7.0f);   // Bottom-back corner
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(9.1f, -2.0f, 7.0f);  // Top-back corner
    glTexCoord2f(0.06f, 1.0f); glVertex3f(9.1f, -2.0f, -10.0f); // Top-front corner
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void stair()
{
    glDisable(GL_TEXTURE_2D);
    for (float i = 0; i < 3; i += 0.5)
    {
        glColor3f(1, 1, 1);
        glColor3f(0.74902, 0.847059, 1);
        glBegin(GL_QUADS);
        glVertex3f(10, 0.5 + i, -21 - i);
        glVertex3f(10, 0 + i, -21 - i);
        glVertex3f(5, 0 + i, -21 - i);
        glVertex3f(5, 0.5 + i, -21 - i);
        glEnd();

        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glVertex3f(10, 0.5 + i, -21 - i);
        glVertex3f(10, 0.5 + i, -21.5 - i);
        glVertex3f(5, 0.5 + i, -21.5 - i);
        glVertex3f(5, 0.5 + i, -21 - i);
        glEnd();
        // Draw left face of the step
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glVertex3f(5, 0.5 + i, -21 - i);
        glVertex3f(5, 0, -21 - i);
        glVertex3f(5, 0, -21.5 - i);
        glVertex3f(5, 0.5 + i, -21.5 - i);
        glEnd();

        // Draw right face of the step
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glVertex3f(10, 0.5 + i, -21 - i);
        glVertex3f(10, 0, -21 - i);
        glVertex3f(10, 0, -21.5 - i);
        glVertex3f(10, 0.5 + i, -21.5 - i);
        glEnd();

        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glVertex3f(10, 0.5 + i, -24);
        glVertex3f(10, 0 + i, -24);
        glVertex3f(5, 0 + i, -24);
        glVertex3f(5, 0.5 + i, -24);
        glEnd();

    }


    glEnable(GL_TEXTURE_2D);
}
void LeftNRightStair()
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.658824, 0.658824, 0.658824);
    float k = 0;
    for (float i = 0; i < 5; i += 0.5)
    {
        glColor3f(0.658824, 0.658824, 0.658824);
        glBegin(GL_QUADS);
        glVertex3f(0, 1 + i, -21 - k);
        glVertex3f(0, 0.5 + i, -21 - k);
        glVertex3f(5, 0.5 + i, -21 - k);
        glVertex3f(5, 1 + i, -21 - k);
        glEnd();

        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex3f(0, 1 + i, -21 - k);
        glVertex3f(0, 1 + i, -23 - k);
        glVertex3f(5, 1 + i, -23 - k);
        glVertex3f(5, 1 + i, -21 - k);
        glEnd();

        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glVertex3f(0, 1 + i, -40);
        glVertex3f(0, 1.5 + i, -40);
        glVertex3f(5, 1.5 + i, -40);
        glVertex3f(5, 1 + i, -40);
        glEnd();

        // Left face
        glBegin(GL_QUADS);
        glVertex3f(0, 0.5 + i, -21 - k);
        glVertex3f(0, 1 + i, -21 - k);
        glVertex3f(0, 1 + i, -40);
        glVertex3f(0, 0.5 + i, -40);
        glEnd();

        // Right face
        glBegin(GL_QUADS);
        glVertex3f(5, 0.5 + i, -21 - k);
        glVertex3f(5, 1 + i, -21 - k);
        glVertex3f(5, 1 + i, -40);
        glVertex3f(5, 0.5 + i, -40);
        glEnd();
        k += 2;
    }
    glEnable(GL_TEXTURE_2D);
}
void chair()
{
    glDisable(GL_TEXTURE_2D);

    glLineWidth(2.0f);

    glBegin(GL_QUADS);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 1.5, 0);
    glVertex3f(0.5, 1.5, 0);
    glVertex3f(0.5, 1, 0);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0, 1, 0.1);
    glVertex3f(0, 1.5, 0.1);
    glVertex3f(0.5, 1.5, 0.1);
    glVertex3f(0.5, 1, 0.1);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0, 1.5, 0);
    glVertex3f(0.5, 1.5, 0);
    glVertex3f(0.5, 1.5, 0.1);
    glVertex3f(0, 1.5, 0.1);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 1.5, 0);
    glVertex3f(0, 1.5, 0.1);
    glVertex3f(0, 1, 0.1);
    glEnd();


    glBegin(GL_QUADS);
    glVertex3f(0.5, 1, 0);
    glVertex3f(0.5, 1.5, 0);
    glVertex3f(0.5, 1.5, 0.1);
    glVertex3f(0.5, 1, 0.1);
    glEnd();


    glBegin(GL_QUADS);
    glVertex3f(0, 1.07, 0.5);
    glVertex3f(0, 1.05, 0);
    glVertex3f(0.5, 1.05, 0);
    glVertex3f(0.5, 1.07, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0, 1, 0.5);
    glVertex3f(0, 1.05, 0.5);
    glVertex3f(0.5, 1.05, 0.5);
    glVertex3f(0.5, 1, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0, 1.05, 0.5);
    glVertex3f(0, 1, 0);
    glVertex3f(0.5, 1, 0);
    glVertex3f(0.5, 1.05, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 1.05, 0.5);
    glVertex3f(0, 1, 0.5);
    glEnd();


    glBegin(GL_QUADS);
    glVertex3f(0.5, 1, 0);
    glVertex3f(0.5, 1.1, 0);
    glVertex3f(0.5, 1.05, 0.5);
    glVertex3f(0.5, 1, 0.5);
    glEnd();

    glEnable(GL_TEXTURE_2D);
}
void chairColor()
{
    float redThreshold = 0.75;  // Initial threshold for red color
    float cyanThreshold = 0.75; // Initial threshold for cyan color

    for (float k = 0; k < 15; k++) {
        float currentRedThreshold = redThreshold;
        float currentCyanThreshold = cyanThreshold;

        for (float i = 0; i < 7.5; i += 0.75) {
            if (k > 2 && k < 7) {
                // Rows 3 to 6
                if (i <= currentRedThreshold) {
                    glColor3f(1.0, 0, 0); // Red color
                }
                else {
                    glColor3f(1, 1, 0); // Yellow color
                }
            }
            else if (k <= 2) {
                // Rows 0 to 2
                glColor3f(1, 1, 0); // Yellow color
            }
            else if (k >= 7) {
                // Rows 7 and above
                if (i <= currentCyanThreshold) {
                    glColor3f(0.137255, 0.137255, 0.556863); // blue color
                }
                else {
                    glColor3f(1.0, 0, 0); // Red color
                }
            }

            glPushMatrix();
            glTranslated(2 - i, 1.65 + k, -21.9 - k);
            glScaled(1, 1, 1);
            chair();
            glPopMatrix();
        }

        // Increment the thresholds for the next row
        if (k < 7) {
            redThreshold += 0.75;
        }
        else {
            cyanThreshold += 0.75;
        }
    }
}
void chairColor2()
{
    float redThreshold = 0.75;  // Initial threshold for red color
    float cyanThreshold = 0.75; // Initial threshold for cyan color

    for (float k = 0; k < 15; k++) {
        float currentRedThreshold = redThreshold;
        float currentCyanThreshold = cyanThreshold;

        for (float i = 0; i < 7.5; i += 0.75) {
            // Skip columns 4 to 10 in rows 5 to 8
            if (k >= 4 && k <= 7 && i >= 0 && i <= 4.5) {
                continue;
            }

            if (k > 2 && k < 7) {
                // Rows 3 to 6
                if (i <= currentRedThreshold) {
                    glColor3f(1.0, 0, 0); // Red color
                }
                else {
                    glColor3f(1, 1, 0); // Yellow color
                }
            }
            else if (k <= 2) {
                // Rows 0 to 2
                glColor3f(1, 1, 0); // Yellow color
            }
            else if (k >= 7) {
                // Rows 7 and above
                if (i <= currentCyanThreshold) {
                    glColor3f(0.137255, 0.137255, 0.556863); // blue color
                }
                else {
                    glColor3f(1.0, 0, 0); // Red color
                }
            }

            glPushMatrix();
            glTranslated(-15 + i, 1.65 + k, -21.9 - k);
            glScaled(1, 1, 1);
            chair();
            glPopMatrix();
        }

        // Increment the thresholds for the next row
        if (k >= 3 && k < 7) {
            redThreshold += 0.75;
        }
        else if (k >= 7) {
            cyanThreshold += 0.75;
        }
    }
}
void chairColor3()
{
    float redThreshold = 0.75;  // Initial threshold for red color
    float cyanThreshold = 0.75; // Initial threshold for cyan color

    for (float k = 0; k < 15; k++) {
        float currentRedThreshold = redThreshold;
        float currentCyanThreshold = cyanThreshold;

        for (float i = 0; i < 7.5; i += 0.75) {
            // Skip columns 1 to 6 in rows 5 to 8
            if (k >= 4 && k <= 7 && i >= 0 && i <= 4.5) {
                continue;
            }

            if (k > 2 && k < 7) {
                // Rows 3 to 6
                if (i <= currentRedThreshold) {
                    glColor3f(1.0, 0, 0); // Red color
                }
                else {
                    glColor3f(1, 1, 0); // Yellow color
                }
            }
            else if (k <= 2) {
                // Rows 0 to 2
                glColor3f(1, 1, 0); // Yellow color
            }
            else if (k >= 7) {
                // Rows 7 and above
                if (i <= currentCyanThreshold) {
                    glColor3f(0.137255, 0.137255, 0.556863); // blue color
                }
                else {
                    glColor3f(1.0, 0, 0); // Red color
                }
            }

            glPushMatrix();
            // Adjust the translation to mirror along the y-axis
            glTranslated(15 - i, 1.65 + k, -21.9 - k);
            glScaled(1, 1, 1);
            chair();
            glPopMatrix();
        }

        // Increment the thresholds for the next row
        if (k >= 3 && k < 7) {
            redThreshold += 0.75;
        }
        else if (k >= 7) {
            cyanThreshold += 0.75;
        }
    }
}
void chairColor4()
{

    float redThreshold = 0.75;  // Initial threshold for red color
    float cyanThreshold = 0.75; // Initial threshold for cyan color

    for (float k = 0; k < 15; k++) {
        float currentRedThreshold = redThreshold;
        float currentCyanThreshold = cyanThreshold;

        for (float i = 0; i < 7.5; i += 0.75) {

            if (k > 2 && k < 7) {
                // Rows 3 to 6
                if (i <= currentRedThreshold) {
                    glColor3f(1.0, 0, 0); // Red color
                }
                else {
                    glColor3f(1, 1, 0); // Yellow color
                }
            }
            else if (k <= 2) {
                // Rows 0 to 2
                glColor3f(1, 1, 0); // Yellow color
            }
            else if (k >= 7) {
                // Rows 7 and above
                if (i <= currentCyanThreshold) {
                    glColor3f(0.137255, 0.137255, 0.556863); // blue color
                }
                else {
                    glColor3f(1.0, 0, 0); // Red color
                }
            }

            glPushMatrix();
            glTranslated(-15 + i, 1.65 + k, -21.9 - k);
            glScaled(1, 1, 1);
            chair();
            glPopMatrix();
        }

        // Increment the thresholds for the next row
        if (k >= 3 && k < 7) {
            redThreshold += 0.75;
        }
        else if (k >= 7) {
            cyanThreshold += 0.75;
        }
    }
}
void fence()
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.80, 0.80, 0.80);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 1, 2.5);
    glVertex3f(0, 1.8, 2.5);
    glVertex3f(1, 1.8, 2.5);
    glVertex3f(1, 1, 2.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0, 1.4, 2.5);
    glVertex3f(1, 1.4, 2.5);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}
void SpectatorSeats()
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(-30.9, 1.5, -21);
    glVertex3f(-30.9, 0.5, -21);
    glVertex3f(5, 0.5, -21);
    glVertex3f(5, 1.5, -21);
    glEnd();

    glColor3f(0.74902, 0.847059, 1);

    for (float i = 0; i < 17; i += 1)
    {
        if (i >= 5 && i <= 8) {
            //front Horizontal 
            glColor3f(0.74902, 0.847059, 1);
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 2.5 + i, -21 - i);
            glVertex3f(-15, 2.5 + i, -21 - i);
            glVertex3f(-15, 1.5 + i, -21 - i);
            glVertex3f(-30.9, 1.5 + i, -21 - i);
            glEnd();
            glBegin(GL_QUADS);
            glVertex3f(5, 1.5 + i, -21 - i);
            glVertex3f(5, 2.5 + i, -21 - i);
            glVertex3f(-10, 2.5 + i, -21 - i);
            glVertex3f(-10, 1.5 + i, -21 - i);
            glEnd();
            glColor3f(0.560784, 0.560784, 0.737255);
            // front Vertical 
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 2.5 + i, -21 - i);
            glVertex3f(-30.9, 2.7 + i, -22 - i);
            glVertex3f(-15, 2.7 + i, -22 - i);
            glVertex3f(-15, 2.5 + i, -21 - i);
            glEnd();
            glBegin(GL_QUADS);
            glVertex3f(5, 2.5 + i, -21 - i);
            glVertex3f(5, 2.7 + i, -22 - i);
            glVertex3f(-10, 2.7 + i, -22 - i);
            glVertex3f(-10, 2.5 + i, -21 - i);
            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.137255, 0.137255, 0.556863);
            glVertex3f(-10, 6.5, -24 - i);
            glVertex3f(-10, 10.5, -24 - i);
            glVertex3f(-10, 10.5, -32 - i);
            glVertex3f(-10, 6.5, -32 - i);
            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.137255, 0.137255, 0.556863);
            glVertex3f(-15, 6.5, -24 - i);
            glVertex3f(-15, 10.5, -24 - i);
            glVertex3f(-15, 10.5, -32 - i);
            glVertex3f(-15, 6.5, -32 - i);
            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.137255, 0.137255, 0.556863);
            glVertex3f(-15, 6.5, -25);
            glVertex3f(-15, 8.5, -25);
            glVertex3f(-15, 12.5, -29);
            glVertex3f(-15, 6.5, -29);
            glEnd();
            glBegin(GL_QUADS);
            glColor3f(0.137255, 0.137255, 0.556863);
            glVertex3f(-10, 6.5, -25);
            glVertex3f(-10, 8.5, -25);
            glVertex3f(-10, 12.5, -29);
            glVertex3f(-10, 6.5, -29);
            glEnd();
            glBegin(GL_QUADS);
            glColor3f(0.137255, 0.137255, 0.556863);
            glVertex3f(-10, 10.5, -29);
            glVertex3f(-15, 10.5, -29);
            glVertex3f(-15, 12.5, -29);
            glVertex3f(-10, 12.5, -29);
            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.137255, 0.137255, 0.556863);
            glVertex3f(-10, 6.5, -24 - i);
            glVertex3f(-15, 6.5, -24 - i);
            glVertex3f(-15, 6.5, -32 - i);
            glVertex3f(-10, 6.5, -32 - i);
            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.137255, 0.137255, 0.556863);
            glVertex3f(-10, 10.5, -24 - i);
            glVertex3f(-15, 10.5, -24 - i);
            glVertex3f(-15, 10.5, -32 - i);
            glVertex3f(-10, 10.5, -32 - i);
            glEnd();

            // back face
            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            glVertex3f(-10, 2.5 + i, -40);
            glVertex3f(5, 2.5 + i, -40);
            glVertex3f(5, 1.5 + i, -40);
            glVertex3f(-10, 1.5 + i, -40);
            glEnd();

            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 2.5 + i, -40);
            glVertex3f(-30.9, 1.5 + i, -40);
            glVertex3f(-15, 1.5 + i, -40);
            glVertex3f(-15, 2.5 + i, -40);
            glEnd();

            // Left face
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 0.5 + i, -21 - i);
            glVertex3f(-30.9, 2.5 + i, -21 - i);
            glVertex3f(-30.9, 2.7 + i, -40);
            glVertex3f(-30.9, 0.5 + i, -40);
            glEnd();

            // Right face
            glBegin(GL_QUADS);
            glVertex3f(5, 0.5 + i, -21 - i);
            glVertex3f(5, 2.5 + i, -21 - i);
            glVertex3f(5, 2.7 + i, -40);
            glVertex3f(5, 0.5 + i, -40);
            glEnd();
        }
        else {
            glColor3f(0.74902, 0.847059, 1);
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 2.5 + i, -21 - i);
            glVertex3f(-30.9, 1.5 + i, -21 - i);
            glVertex3f(5, 1.5 + i, -21 - i);
            glVertex3f(5, 2.5 + i, -21 - i);
            glEnd();

            glColor3f(0.560784, 0.560784, 0.737255);
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 2.5 + i, -21 - i);
            glVertex3f(-30.9, 2.7 + i, -22 - i);
            glVertex3f(5, 2.7 + i, -22 - i);
            glVertex3f(5, 2.5 + i, -21 - i);
            glEnd();

            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 2.5 + i, -40);
            glVertex3f(-30.9, 1.5 + i, -40);
            glVertex3f(5, 1.5 + i, -40);
            glVertex3f(5, 2.5 + i, -40);
            glEnd();

            // Left face
            glBegin(GL_QUADS);
            glVertex3f(-30.9, 0.5 + i, -21 - i);
            glVertex3f(-30.9, 2.5 + i, -21 - i);
            glVertex3f(-30.9, 2.7 + i, -40);
            glVertex3f(-30.9, 0.5 + i, -40);
            glEnd();

            // Right face
            glBegin(GL_QUADS);
            glVertex3f(5, 0.5 + i, -21 - i);
            glVertex3f(5, 2.5 + i, -21 - i);
            glVertex3f(5, 2.7 + i, -40);
            glVertex3f(5, 0.5 + i, -40);
            glEnd();

        }
    }

    glEnable(GL_TEXTURE_2D);
}
void Cylinder(float radius, float height, int segments) {
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);

        glNormal3f(x, 0.0f, z);
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, height, z);
    }
    glEnd();


}
void TrackLine(float centerX, float centerz, float radiusX, float radiusz, int num)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num; ++i)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num);
        float x = radiusX * cosf(theta);
        float z = radiusz * sinf(theta);
        glVertex3f(x + centerX, 2.0f, z + centerz);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}
void runningTrack(float centerX, float centerz, float radiusX, float radiusz, int num)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.556863, 0.137255, 0.137255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(centerX, 2.0f, centerz); // Center of the oval
    for (int i = 0; i <= num; ++i)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num);
        float x = radiusX * cosf(theta);
        float z = radiusz * sinf(theta);
        glVertex3f(x + centerX, 2.0f, z + centerz);
    }
    glEnd();
    int x = 15, z = 30;
    for (int i = 0; i < 9; i++)
    {
        glPushMatrix();
        glTranslatef(0, 0.0, 0);
        glRotatef(180, 0, 1, 0);
        TrackLine(0.0f, 0.0f, x, z, 100);
        x -= 0.5;
        z -= 0.5;
        glPopMatrix();
    }

    glEnable(GL_TEXTURE_2D);
}
void goalLine()
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    for (float x = -0.5; x > -1.1; x -= 0.1)
    {
        glVertex3f(x, 2.5, 2.5);
        glVertex3f(x, 2.5f, 6.0f);
    }
    for (float z = 2.5; z < 6.0; z += 0.1)
    {
        glVertex3f(-0.5, 2.5, z);
        glVertex3f(-1.0, 2.5f, z);
    }

    float x = -1.0;
    for (float y = 2.5; y > 1; y -= 0.15)
    {

        glVertex3f(-0.5, y, 2.5);
        glVertex3f(x, y, 2.5);
        x -= 0.052;
    }
    float y = 2.5;
    for (float x = -0.5; x > -1.5; x -= 0.10)
    {

        if (x < -1.0) {
            glVertex3f(x, y, 2.5f);
            glVertex3f(x, 1.0f, 2.5f);
            if (y > 1)
                y -= 0.3;
        }
        else if (x >= -1.0 && x < -0.5) {
            glVertex3f(x, 2.5f, 2.5f);
            glVertex3f(x, 1.0f, 2.5f);
        }
    }
    float x1 = -1.0;
    for (float y = 2.5; y > 1; y -= 0.15)
    {

        glVertex3f(-0.5, y, 6.0);
        glVertex3f(x1, y, 6.0);
        x1 -= 0.052;
    }
    float y1 = 2.5;
    for (float x = -0.5; x > -1.5; x -= 0.10)
    {

        if (x < -1.0) {
            glVertex3f(x, y1, 6.0f);
            glVertex3f(x, 1.0f, 6.0f);
            if (y1 > 1)
                y1 -= 0.3;
        }
        else if (x >= -1.0 && x < -0.5) {
            glVertex3f(x, 2.5f, 6.0f);
            glVertex3f(x, 1.0f, 6.0f);
        }
    }
    float y3 = 2.5;
    for (float x = -1.0; x > -1.5; x -= 0.05)
    {

        glVertex3f(x, y3, 2.5);
        glVertex3f(x, y3, 6.0f);

        if (y3 > 1)
            y3 -= 0.15;
    }

    for (float z = 2.5; z < 6.0; z += 0.1)
    {

        glVertex3f(-1.0, 2.5, z);
        glVertex3f(-1.5, 1.0, z);


    }



    glEnd();
}
void goal()
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-0.5f, 2.5f, 2.5f);
    glVertex3f(-1.0f, 2.5f, 2.5f);
    glVertex3f(-1.5f, 1.0f, 2.5f);
    glVertex3f(-0.5f, 1.0f, 2.5f);
    glVertex3f(-0.5f, 1.0f, 6.0f);
    glVertex3f(-1.5f, 1.0f, 6.0f);
    glVertex3f(-1.0f, 2.5f, 6.0f);
    glVertex3f(-0.5f, 2.5f, 6.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(-0.5f, 2.5f, 2.5f);
    glVertex3f(-0.5f, 1.0f, 2.5f);
    glVertex3f(-0.5f, 1.0f, 6.0f);
    glVertex3f(-0.5f, 2.5f, 6.0f);
    glEnd();
    goalLine();

}
void circle(int num, int semi)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < num; i++) {
        float theta = semi * M_PI * float(i) / float(num); // Calculate the angle for this vertex
        float x = 1 * cosf(theta); // Calculate the x component
        float z = 1 * sinf(theta); // Calculate the y component

        glVertex3f(x, 0, z); // Output vertex
    }
    glEnd();
}
void halfRectangle()
{

    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-0.5f, 0.0f, 2.5f);
    glVertex3f(1.0f, 0.0f, 2.5f);
    glVertex3f(1.0f, 0.0f, 6.0f);
    glVertex3f(-0.5f, 0.0f, 6.0f);
    glEnd();
}
void line(void)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-0.5f, 0.0f, -0.5f); // Start point of the line
    glVertex3f(9.5f, 0.0f, -0.5f);
    glVertex3f(9.5f, 0.0f, 9.5f);
    glVertex3f(9.5f, 0.0f, -0.5f);
    glVertex3f(19.5f, 0.0f, -0.5f);
    glVertex3f(19.5f, 0.0f, 7.1f);
    glVertex3f(19.5f, 0.0f, 1.5f);
    glVertex3f(17.5f, 0.0f, 1.5f);
    glVertex3f(17.5f, 0.0f, 7.0f);
    glVertex3f(19.5f, 0.0f, 7.0f);
    glVertex3f(19.5f, 0.0f, 9.5f);
    glVertex3f(-0.5f, 0.0f, 9.5f);
    glVertex3f(-0.5f, 0.0f, 1.5f);
    glVertex3f(2.5f, 0.0f, 1.5f);
    glVertex3f(2.5f, 0.0f, 7.0f);
    glVertex3f(-0.5f, 0.0f, 7.0f);
    //	glVertex3f(-0.5f, 0.0f, 2.5f);
    glEnd();
    glPushMatrix();
    halfRectangle();
    glPopMatrix();
    glTranslated(9.5, 0, 4.5);
    circle(360, 2);
    glPushMatrix();
    glTranslated(-8.5, 0, -0.2);
    glRotated(90, 0, 1, 0);
    circle(180, 1);
    glPopMatrix();
    glTranslated(8.0, 0, -0.4);
    glRotated(270, 0, 1.0, 0);
    circle(180, 1);//left

    glEnable(GL_TEXTURE_2D);
}
void drawTexturedRectangle() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[14]);

    glBegin(GL_QUADS);
    // Bottom-left vertex
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-10.0f, 0.0f, 0.0f);

    // Bottom-right vertex
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);

    // Top-right vertex
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(10.0f, 10.0f, 0.0f);

    // Top-left vertex
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-10.0f, 10.0f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
void field(void) {
    glEnable(GL_TEXTURE_2D);
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture[15]); //bind our textureto our shape
    glColor3f(1.0, 1.0, 1.0); // reset color to white
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);// Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void logo(void) {
    glEnable(GL_TEXTURE_2D);
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture[13]); //bind our textureto our shape
    glColor3f(1.0, 1.0, 1.0); // reset color to white
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);// Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void wholefield()
{
    for (int z = 0; z < 10; z++) {


        for (int i = 0; i < 20; i++) {
            //field 
            glPushMatrix();
            glTranslatef(i, 0, z);
            field();
            glPopMatrix();
        }
    }


    glPushMatrix();
    glTranslatef(0, 1.025, 0);
    line();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, -1.1, 5);
    glRotatef(90, 0, 1, 0);
    runningTrack(0.0f, 0.0f, 15.0f, 30.0f, 100);
    glPopMatrix();
}
int currentTexture = 16;  // Starting with texture[16]
const int numTextures = 6;
void screen(void) {
    float scaleX = 5.0f;  // Scale factor for width
    float scaleY = 2.5f;  // Scale factor for height
    float scaleZ = 1.0f;  // Scale factor for depth

    glEnable(GL_TEXTURE_2D);
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture[currentTexture]); //bind our texture to our shape
    glColor3f(1.0, 1.0, 1.0); // reset color to white
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f * scaleX, -1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f * scaleX, -1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ);
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ);
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f * scaleX, -1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f * scaleX, -1.0f * scaleY, 1.0f * scaleZ);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f * scaleX, -1.0f * scaleY, 1.0f * scaleZ);
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ);
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f * scaleX, -1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ);
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void groupname(void) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[22]);

    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -19.1f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, -0.0f, 0.0f);  // Bottom-left corner
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-80.0f, 30.0f, 0.0f);   // Top-left corner
    glTexCoord2f(1.0f, 1.0f); glVertex3f(60.0f, 30.0f, 0.0f);    // Top-right corner
    glTexCoord2f(1.0f, 0.0f); glVertex3f(60.0f, -0.0f, 0.0f);   // Bottom-right corner
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void updateTexture(int value) {
    currentTexture = (currentTexture + 1) % numTextures + 16;
    glutPostRedisplay();  // Request a redraw
    glutTimerFunc(2000, updateTexture, 0);  // Set timer for next update
}
void spotlight(void)
{
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glColor3f(0.752941, 0.752941, 0.752941);
    Cylinder(0.1f, 45.0f, 10);
    glPopMatrix();


    float z1 = 0.0;
    float y1 = 45;
    for (int y = 0; y < 8; ++y) {

        for (int z = 0; z < 15; ++z) {

            glPushMatrix();
            glTranslatef(0.1, y1, z1 - 1);
            glColor3f(0.752941, 0.752941, 0.752941);
            Cylinder(0.1, 0.05, 10);
            z1 += 0.2;
            glPopMatrix();
        }
        y1 -= 0.4;
        z1 = 0.0;
    }

}
void grass(void) {
    glEnable(GL_TEXTURE_2D);
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture[3]); //bind our texture to our shape
    glColor3f(1.0, 1.0, 1.0); // reset color to white
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);// Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void ALLgrass() {
    for (int z = -20; z < 70; z++) {


        for (int i = -30; i < 170; i++) {
            //grass
            glPushMatrix();
            glTranslatef(i, -0.2, z);
            grass();
            glPopMatrix();
        }
    }
}
void updateCarPosition() {
    carX += carSpeed;

    // Loop the car back to the start if it goes too far
    if (carX > 20.0f) {
        carX = -20.0f;
    }

    car3Z += car3Speed;  // Increment Z, which is the car's forward direction
    // Loop the third car back to the start if it goes too far
    if (car3Z > -100.0f) {
        car3Z = -160.0f;  // Reset to its initial position
    }

    glutPostRedisplay();
}
void greyWall(void) {
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture[23]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[23]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[23]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void yellowWall(void) {
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture[24]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[24]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[24]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void blueRoof(void) {
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture[25]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[25]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[25]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void DewanSukan(void) {
    glBindTexture(GL_TEXTURE_2D, texture[26]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[26]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[26]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void greyEntrance(void) {
    glBindTexture(GL_TEXTURE_2D, texture[23]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[23]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[23]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void PusatSukan(void) {
    glBindTexture(GL_TEXTURE_2D, texture[28]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.04f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.04f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[28]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[28]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void blackglass(void) {
    glBindTexture(GL_TEXTURE_2D, texture[29]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[29]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[29]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void metal(void) {
    glBindTexture(GL_TEXTURE_2D, texture[30]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[30]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[30]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void aluminiumRoof(void) {
    glBindTexture(GL_TEXTURE_2D, texture[31]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[31]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[31]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void window(void) {
    glBindTexture(GL_TEXTURE_2D, texture[32]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[32]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[32]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void floor(void) {
    glBindTexture(GL_TEXTURE_2D, texture[33]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[33]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[33]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void rollerShutterDoor(void) {
    glBindTexture(GL_TEXTURE_2D, texture[34]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[34]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[34]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void rollerShutterWindow(void) {
    glBindTexture(GL_TEXTURE_2D, texture[35]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[35]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[35]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void whiteDoor(void) {
    glBindTexture(GL_TEXTURE_2D, texture[36]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[36]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[36]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void KompleksSukan(void) {
    glBindTexture(GL_TEXTURE_2D, texture[37]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.04f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.04f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[37]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[37]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void polygon(int a, int b, int c, int d, GLfloat color[])
{
    /* draw a polygon via list of vertices */
    glBegin(GL_POLYGON);
    glColor4fv(color);
    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
    glEnd();
}
void colorcube(GLfloat color[])
{
    polygon(0, 3, 2, 1, color);
    polygon(2, 3, 7, 6, color);
    polygon(0, 4, 7, 3, color);
    polygon(1, 2, 6, 5, color);
    polygon(4, 5, 6, 7, color);
    polygon(0, 1, 5, 4, color);
}
void roadKompleks(void) {
    glBindTexture(GL_TEXTURE_2D, texture[1]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.04f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.04f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[1]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[1]); //bind our textureto our shape
    glBegin(GL_QUADS);
    // Right face
    glTexCoord2f(1.08f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
    glTexCoord2f(0.06f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.06f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.08f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.08f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
    glTexCoord2f(0.06f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
    glEnd();
}
void WholeKompleksSukan() {
    glEnable(GL_TEXTURE_2D);
    //draw grey building
    for (int x = -3; x < 14; x++) {
        for (int y = 0; y < 10; y++) {
            for (int z = 1; z < 8; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    //draw yellow wall
    for (int x = 3; x < 8; x++) {
        for (int y = 0; y < 12; y++) {
            for (int z = 0; z < 10; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.20);
                glTranslatef(x, y, z + 0.5);
                yellowWall();
                glPopMatrix();
            }
        }
    }
    //draw blue roof top
    for (int x = -3; x < 14; x++) {
        for (int z = 1; z < 8; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 51, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw Dewan Sukan
    glPushMatrix();
    glScalef(1, 0.35, 0.1);
    glTranslatef(2.5, 7.3, -0.1);
    DewanSukan();
    glPopMatrix();
    //draw entrance
    glPushMatrix();
    glScalef(1, 0.9, 0.1);
    glTranslatef(2.5, 0.7, -0.2);
    greyEntrance();
    glPopMatrix();
    //draw Pusat Sukan
    glPushMatrix();
    glScalef(0.469, 0.186, 0.1);
    glTranslatef(5.15, 4, -0.3);
    PusatSukan();
    glPopMatrix();
    //draw right door 
    glPushMatrix();
    glScalef(0.186, 0.4, 0.1);
    glTranslatef(10, 0.33, -0.3);
    blackglass();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.025, 0.4, 0.1);
    glTranslatef(84, 0.33, -0.3);
    blackglass();
    glPopMatrix();
    //draw middle right door 
    glPushMatrix();
    glScalef(0.186, 0.4, 0.1);
    glTranslatef(12.5, 0.33, -0.3);
    blackglass();
    glPopMatrix();
    //draw middle left door 
    glPushMatrix();
    glScalef(0.186, 0.4, 0.1);
    glTranslatef(14.5, 0.33, -0.3);
    blackglass();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.025, 0.4, 0.1);
    glTranslatef(117.5, 0.33, -0.3);
    blackglass();
    glPopMatrix();
    //draw left door 
    glPushMatrix();
    glScalef(0.186, 0.4, 0.1);
    glTranslatef(17, 0.33, -0.3);
    blackglass();
    glPopMatrix();
    //draw glass window
    glPushMatrix();
    glScalef(0.8, 0.15, 0.1);
    glTranslatef(3.1, 7.5, -0.3);
    blackglass();
    glPopMatrix();
    //draw door frame vertical
    glPushMatrix();
    glScalef(0.015, 0.41, 0.1);
    glTranslatef(111, 0.33, -0.3);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.015, 0.41, 0.1);
    glTranslatef(137, 0.33, -0.3);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.015, 0.41, 0.1);
    glTranslatef(142, 0.33, -0.3);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.015, 0.41, 0.1);
    glTranslatef(193, 0.33, -0.3);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.015, 0.41, 0.1);
    glTranslatef(198, 0.33, -0.3);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.015, 0.41, 0.1);
    glTranslatef(224, 0.33, -0.3);
    metal();
    glPopMatrix();
    //draw metal door frame horizontal
    glPushMatrix();
    glScalef(0.85, 0.015, 0.1);
    glTranslatef(2.94, 36.1, -0.3);
    metal();
    glPopMatrix();
    //draw grey building left
    for (int x = -7; x < 0; x++) {
        for (int y = 0; y < 5; y++) {
            for (int z = -10; z < 8; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    //draw blue roof top left 1
    for (int x = -7; x < -4; x++) {
        for (int z = -10; z < 8; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 26, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw blue roof top left 2
    for (int x = -4; x < 0; x++) {
        for (int z = -10; z < 0; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 26, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw grey building right
    for (int x = 11; x < 18; x++) {
        for (int y = 0; y < 5; y++) {
            for (int z = -10; z < 8; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    //draw blue roof top right 1
    for (int x = 11; x < 15; x++) {
        for (int z = -10; z < 0; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 26, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw blue roof top right 2
    for (int x = 15; x < 18; x++) {
        for (int z = -10; z < 8; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 26, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw aluminium roof
    glPushMatrix();
    glScalef(1.2, 0.02, 1.2);
    glTranslatef(2.1, 75, -1.1);
    aluminiumRoof();
    glPopMatrix();
    //draw left window
    glPushMatrix();
    glScalef(0.2, 0.2, 0.01);
    glTranslatef(22, 0.9, 0);
    window();
    glPopMatrix();
    //draw right window
    glPushMatrix();
    glScalef(0.2, 0.2, 0.01);
    glTranslatef(3, 0.9, 0);
    window();
    glPopMatrix();
    //draw floor
    glPushMatrix();
    glScalef(1, 0.02, 2);
    glTranslatef(2.5, -13, -1);
    floor();
    glPopMatrix();
    //draw yellow wall right
    glPushMatrix();
    glScalef(0.01, 0.45, 1.2);
    glTranslatef(-0.1, 0.45, -1.0);
    yellowWall();
    glPopMatrix();
    //draw yellow wall left
    glPushMatrix();
    glScalef(0.01, 0.45, 1.2);
    glTranslatef(500, 0.45, -1.0);
    yellowWall();
    glPopMatrix();
    //draw yellow wall right 1
    glPushMatrix();
    glScalef(0.7, 0.45, 0.01);
    glTranslatef(-1.5, 0.45, -275);
    yellowWall();
    glPopMatrix();
    //draw yellow wall right 2
    glPushMatrix();
    glScalef(0.7, 0.45, 0.01);
    glTranslatef(-4.3, 0.45, -275);
    yellowWall();
    glPopMatrix();
    //draw yellow wall left 1
    glPushMatrix();
    glScalef(0.5, 0.45, 0.01);
    glTranslatef(11.5, 0.45, -275);
    yellowWall();
    glPopMatrix();
    //draw yellow wall left 2
    glPushMatrix();
    glScalef(0.5, 0.45, 0.01);
    glTranslatef(14, 0.45, -275);
    yellowWall();
    glPopMatrix();
    //draw yellow wall left 3
    glPushMatrix();
    glScalef(0.5, 0.45, 0.01);
    glTranslatef(16.5, 0.45, -275);
    yellowWall();
    glPopMatrix();
    //draw left side yellow wall
    glPushMatrix();
    glScalef(0.01, 0.45, 0.5);
    glTranslatef(900, 0.45, -4.0);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.01, 0.35, 0.5);
    glTranslatef(900, 0.3, -1.0);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.01, 0.35, 0.5);
    glTranslatef(900, 0.3, 2.0);
    yellowWall();
    glPopMatrix();
    //draw right side yellow wall
    glPushMatrix();
    glScalef(0.01, 0.45, 0.5);
    glTranslatef(-400, 0.45, -4.0);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.01, 0.45, 0.5);
    glTranslatef(-400, 0.45, -1.0);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.01, 0.45, 0.5);
    glTranslatef(-400, 0.45, 2.0);
    yellowWall();
    glPopMatrix();
    //draw left side window
    glPushMatrix();
    glScalef(0.01, 0.25, 0.2);
    glTranslatef(-405, 1.1, -10.0);
    window();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.01, 0.25, 0.2);
    glTranslatef(-405, 1.1, -2.5);
    window();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.01, 0.25, 0.2);
    glTranslatef(-405, 1.1, 5.0);
    window();
    glPopMatrix();
    //draw left window 1
    glPushMatrix();
    glScalef(0.25, 0.25, 0.01);
    glTranslatef(23, 0.9, -277);
    window();
    glPopMatrix();
    //draw left window 2
    glPushMatrix();
    glScalef(0.25, 0.25, 0.01);
    glTranslatef(33, 0.9, -277);
    window();
    glPopMatrix();
    //draw right window 1
    glPushMatrix();
    glScalef(0.25, 0.25, 0.01);
    glTranslatef(-4, 0.9, -277);
    window();
    glPopMatrix();
    //draw right window 2
    glPushMatrix();
    glScalef(0.25, 0.25, 0.01);
    glTranslatef(-12, 0.9, -277);
    window();
    glPopMatrix();
    //draw left upper window 1
    glPushMatrix();
    glScalef(0.2, 0.2, 0.01);
    glTranslatef(22, 9, 0);
    window();
    glPopMatrix();
    //draw left upper window 2
    glPushMatrix();
    glScalef(0.2, 0.2, 0.01);
    glTranslatef(29, 9, 0);
    window();
    glPopMatrix();
    //draw right upper window 1
    glPushMatrix();
    glScalef(0.2, 0.2, 0.01);
    glTranslatef(3, 9, 0);
    window();
    glPopMatrix();
    //draw right upper window 2
    glPushMatrix();
    glScalef(0.2, 0.2, 0.01);
    glTranslatef(-5, 9, 0);
    window();
    glPopMatrix();
    //draw grey building middle
    for (int x = -1; x < 16; x++) {
        for (int y = 0; y < 7; y++) {
            for (int z = 9; z < 60; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    for (int x = -5; x < -1; x++) {
        for (int y = 4; y < 7; y++) {
            for (int z = 9; z < 60; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    for (int x = -5; x < -1; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 14; z < 57; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    //draw blue roof middle
    for (int x = -5; x < 16; x++) {
        for (int z = 9; z < 59; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 35, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw blue wall left
    glPushMatrix();
    glScalef(0.01, 1, 1.2);
    glTranslatef(800, 0.7, 7);
    blueRoof();
    glPopMatrix();
    //draw blue wall right
    glPushMatrix();
    glScalef(0.01, 1, 1.2);
    glTranslatef(-300, 0.7, 7);
    blueRoof();
    glPopMatrix();
    //draw right side window
    glPushMatrix();
    glScalef(0.25, 0.25, 0.01);
    glTranslatef(-12, 0.9, -277);
    window();
    glPopMatrix();
    //draw right yellow wall behind
    glPushMatrix();
    glScalef(1.4, 0.45, 0.01);
    glTranslatef(-1.5, 0.45, 200);
    yellowWall();
    glPopMatrix();
    //draw window right behind
    glPushMatrix();
    glScalef(0.2, 0.25, 0.01);
    glTranslatef(-9.5, 1.1, 203);
    window();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2, 0.25, 0.01);
    glTranslatef(-15.5, 1.1, 203);
    window();
    glPopMatrix();
    //draw yellow wall right side door entrance
    glPushMatrix();
    glScalef(0.01, 0.45, 0.65);
    glTranslatef(-100, 0.45, 4.1);
    yellowWall();
    glPopMatrix();
    //draw right side door
    glPushMatrix();
    glScalef(0.1, 0.4, 0.3);
    glTranslatef(-10, 0.4, 9);
    blackglass();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.4, 0.01);
    glTranslatef(-10.1, 0.4, 240);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.4, 0.01);
    glTranslatef(-10.1, 0.4, 270);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.4, 0.01);
    glTranslatef(-10.1, 0.4, 300);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.01, 0.32);
    glTranslatef(-10.1, 55, 8.4);
    metal();
    glPopMatrix();
    //draw right side middle yellow wall 1
    glPushMatrix();
    glScalef(0.1, 0.45, 0.6);
    glTranslatef(-29.1, 0.45, 6.7);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.6);
    glTranslatef(-29.1, 0.45, 9);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.5);
    glTranslatef(-29.1, 0.45, 13.4);
    yellowWall();
    glPopMatrix();
    //draw roller shutter door
    glPushMatrix();
    glScalef(0.1, 0.35, 0.35);
    glTranslatef(-29.2, 0.3, 14.7);
    rollerShutterDoor();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.35, 0.3);
    glTranslatef(-29.2, 0.3, 19);
    rollerShutterDoor();
    glPopMatrix();
    //draw side door right
    glPushMatrix();
    glScalef(0.1, 0.35, 0.2);
    glTranslatef(-29.2, 0.3, 21.3);
    whiteDoor();
    glPopMatrix();
    //draw right side middle yellow wall 1
    glPushMatrix();
    glScalef(0.1, 0.45, 0.6);
    glTranslatef(-29.1, 0.45, 17);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.6);
    glTranslatef(-29.1, 0.45, 19.5);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.6);
    glTranslatef(-29.1, 0.45, 22);
    yellowWall();
    glPopMatrix();
    //draw roller shutter door 
    glPushMatrix();
    glScalef(0.1, 0.35, 0.35);
    glTranslatef(-29.2, 0.3, 32.7);
    rollerShutterDoor();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.35, 0.3);
    glTranslatef(-29.2, 0.3, 40);
    rollerShutterDoor();
    glPopMatrix();
    //draw small window side wall
    glPushMatrix();
    glScalef(0.1, 0.05, 0.3);
    glTranslatef(-29.2, 10, 43.8);
    rollerShutterDoor();
    glPopMatrix();
    //draw right yellow wall behind 2
    glPushMatrix();
    glScalef(1.4, 0.45, 0.01);
    glTranslatef(-1, 0.45, 1425);
    yellowWall();
    glPopMatrix();
    //draw window right behind 2
    glPushMatrix();
    glScalef(0.2, 0.25, 0.01);
    glTranslatef(-9.5, 1.1, 1427);
    window();
    glPopMatrix();
    //draw yellow wall right side door entrance 2
    glPushMatrix();
    glScalef(0.01, 0.45, 0.65);
    glTranslatef(-100, 0.45, 22.2);
    yellowWall();
    glPopMatrix();
    //draw right yellow wall behind 3
    glPushMatrix();
    glScalef(1.4, 0.45, 0.01);
    glTranslatef(-1.5, 0.45, 1500);
    yellowWall();
    glPopMatrix();
    //draw right side door 2 
    glPushMatrix();
    glScalef(0.1, 0.4, 0.3);
    glTranslatef(-10, 0.4, 48.8);
    blackglass();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.4, 0.01);
    glTranslatef(-10.1, 0.4, 1495);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.4, 0.01);
    glTranslatef(-10.1, 0.4, 1435);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.4, 0.01);
    glTranslatef(-10.1, 0.4, 1465);
    metal();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.01, 0.3);
    glTranslatef(-10.1, 56, 48.8);
    metal();
    glPopMatrix();
    //draw grey building back left
    for (int x = 11; x < 18; x++) {
        for (int y = 0; y < 5; y++) {
            for (int z = 61; z < 68; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    //draw blue roof top back left
    for (int x = 11; x < 18; x++) {
        for (int z = 60; z < 67; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 26, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw grey building back right
    for (int x = -7; x < 0; x++) {
        for (int y = 0; y < 5; y++) {
            for (int z = 61; z < 68; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    //draw blue roof top back right
    for (int x = -7; x < 0; x++) {
        for (int z = 60; z < 67; z++) {
            glPushMatrix();
            glScalef(0.505, 0.05, 0.255);
            glTranslatef(x, 26, z);
            blueRoof();
            glPopMatrix();
        }
    }
    //draw yellow wall right last
    glPushMatrix();
    glScalef(0.1, 0.45, 0.6);
    glTranslatef(-39.3, 0.45, 26.7);
    yellowWall();
    glPopMatrix();
    //draw yellow wall behind 
    glPushMatrix();
    glScalef(0.7, 0.45, 0.1);
    glTranslatef(-4.2, 0.45, 169.3);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.7, 0.45, 0.1);
    glTranslatef(-1.7, 0.45, 169.3);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(2.5, 0.8, 0.1);
    glTranslatef(1, 0.65, 150);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.7, 0.45, 0.1);
    glTranslatef(11.5, 0.45, 169.3);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.7, 0.45, 0.1);
    glTranslatef(8.6, 0.45, 169.3);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(-0.7, 0.45, 23);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(50.8, 0.45, 23);
    yellowWall();
    glPopMatrix();

    //draw left side yellow wall
    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(79.2, 0.45, 4.0);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(79.2, 0.45, 6.5);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(79.2, 0.45, 9.0);
    yellowWall();
    glPopMatrix();
    //draw left side window
    glPushMatrix();
    glScalef(0.1, 0.25, 0.2);
    glTranslatef(79.3, 1.1, 14.0);
    window();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.25, 0.2);
    glTranslatef(79.3, 1.1, 31.0);
    window();
    glPopMatrix();
    //draw left side roller shutter door
    glPushMatrix();
    glScalef(0.1, 0.35, 0.35);
    glTranslatef(79.3, 0.3, 13.8);
    rollerShutterDoor();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.35, 0.3);
    glTranslatef(79.3, 0.3, 14);
    rollerShutterDoor();
    glPopMatrix();
    //draw left side yellow wall
    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(79.2, 0.45, 15.0);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(79.2, 0.45, 17.5);
    yellowWall();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.45, 0.7);
    glTranslatef(79.2, 0.45, 20.0);
    yellowWall();
    glPopMatrix();
    //draw left side window 2
    glPushMatrix();
    glScalef(0.1, 0.25, 0.2);
    glTranslatef(79.3, 1.1, 52.5);
    window();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.25, 0.2);
    glTranslatef(79.3, 1.1, 70.0);
    window();
    glPopMatrix();
    //draw left side roller shutter door 2
    glPushMatrix();
    glScalef(0.1, 0.35, 0.35);
    glTranslatef(79.3, 0.3, 35.8);
    rollerShutterDoor();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.35, 0.3);
    glTranslatef(79.3, 0.3, 39.6);
    rollerShutterDoor();
    glPopMatrix();
    //draw yellow wall left last
    glPushMatrix();
    glScalef(0.1, 0.45, 0.6);
    glTranslatef(89.1, 0.45, 26.7);
    yellowWall();
    glPopMatrix();
    //draw left side shutter window
    glPushMatrix();
    glScalef(0.1, 0.25, 0.15);
    glTranslatef(79.3, 0.85, 50);
    rollerShutterWindow();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.25, 0.15);
    glTranslatef(79.3, 0.85, 54);
    rollerShutterWindow();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.25, 0.15);
    glTranslatef(79.3, 0.85, 58);
    rollerShutterWindow();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.25, 0.15);
    glTranslatef(79.3, 0.85, 62);
    rollerShutterWindow();
    glPopMatrix();
    //draw back window right
    glPushMatrix();
    glScalef(0.2, 0.25, 0.1);
    glTranslatef(-15.8, 0.75, 169.35);
    window();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2, 0.25, 0.1);
    glTranslatef(-13.8, 0.75, 169.35);
    window();
    glPopMatrix();
    //draw back window left
    glPushMatrix();
    glScalef(0.2, 0.25, 0.1);
    glTranslatef(39.3, 0.75, 169.35);
    window();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2, 0.25, 0.1);
    glTranslatef(41.3, 0.75, 169.35);
    window();
    glPopMatrix();
    //draw back shutter door
    glPushMatrix();
    glScalef(0.35, 0.35, 0.1);
    glTranslatef(-4.2, 0.3, 169.35);
    rollerShutterDoor();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.3, 0.35, 0.1);
    glTranslatef(-2.8, 0.3, 169.35);
    rollerShutterDoor();
    glPopMatrix();
    //draw top building
    for (int x = -2; x < 13; x++) {
        for (int y = 7; y < 13; y++) {
            for (int z = 11; z < 58; z++) {
                glPushMatrix();
                glScalef(0.5, 0.25, 0.25);
                glTranslatef(x, y, z);
                greyWall();
                glPopMatrix();
            }
        }
    }
    //draw top roof
    glPushMatrix();
    glScalef(4.5, 0.05, 6.2);
    glTranslatef(0.55, 65, 1.365);
    metal();
    glPopMatrix();
    //draw Kompleks Sukan
    glPushMatrix();
    glScalef(1.7, 0.5, 0.1);
    glTranslatef(1.75, 4.8, 144.1);
    KompleksSukan();
    glPopMatrix();
    //draw road
    glPushMatrix();
    glScalef(12, 0.01, 12);
    glTranslatef(0.4, -30, 0.6);
    roadKompleks();
    glPopMatrix();
}

void display() {
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);

    GLUquadricObj* quadObj = gluNewQuadric();
    gluQuadricOrientation(quadObj, GLU_OUTSIDE);

    // Calculate the target position (where the camera is looking)
    float targetX = cameraX + cos(cameraYaw);
    float targetY = cameraY + sin(cameraPitch);
    float targetZ = cameraZ - sin(cameraYaw);

    // Set camera position and orientation (third-person view)
    gluLookAt(
        cameraX, cameraY, cameraZ, // Camera position
        targetX, targetY, targetZ, // Target position
        0.0, 1.0, 0.0              // Up vector
    );

    // Define the number of repetitions for the parking lot
    int numRepetitionsX = 6;
    int numRepetitionsZ = 0;

    // Draw the parking lot 1
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            // Bind the texture for the ground
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -140.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -140.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -150.0 + offsetZ);
            glEnd();
        }
    }

    //parking grass 1
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            float offsetX = i * 10.0f;
            float offsetZ = j * 10.0f;
            float height = 5.5f;

            // Top face
            glBindTexture(GL_TEXTURE_2D, texture[3]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, height, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, height, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -150.0 + offsetZ);
            glEnd();

            // Other faces
            glBindTexture(GL_TEXTURE_2D, texture[4]);
            glBegin(GL_QUADS);
            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -150.0 + offsetZ);

            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -150.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -155.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -155.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -155.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -155.0 + offsetZ);
            glEnd();
        }
    }

    // Draw trees on the grass
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            float offsetX = i * 10.0f;
            float offsetZ = j * 10.0f;
            float treeHeight = 15.0f; // Adjust the tree height as needed

            glBindTexture(GL_TEXTURE_2D, texture[5]); // Bind the tree texture

            glBegin(GL_QUADS);
            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -151.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -151.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -151.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -151.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -154.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -154.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -151.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -151.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -154.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -151.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -151.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -154.0 + offsetZ);

            // Top face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -151.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -151.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -154.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f, -151.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f, -151.0 + offsetZ);
            glEnd();

            // Draw the cube leaves on top of the tree trunk
            glBindTexture(GL_TEXTURE_2D, texture[6]); // Bind the leaf texture

            glBegin(GL_QUADS);
            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -150.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -155.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -155.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -155.0 + offsetZ);

            // Top face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -150.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -150.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -150.0 + offsetZ);
            glEnd();
        }
    }

    //parking lot 1,2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            // Bind the texture for the ground
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -165.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -155.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -165.0 + offsetZ);
            glEnd();
        }
    }

    // Draw the road 1,2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            glBindTexture(GL_TEXTURE_2D, texture[1]);

            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -140.0 + offsetZ); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -140.0 + offsetZ);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -135.0 + offsetZ);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -135.0 + offsetZ); // Top-left corner
            glEnd();
        }
    }

    // Draw the parking lot 2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            // Bind the texture for the ground
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -110.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -110.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -120.0 + offsetZ);
            glEnd();
        }
    }

    //parking grass 2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            float offsetX = i * 10.0f;
            float offsetZ = j * 10.0f;
            float height = 5.5f;

            // Top face
            glBindTexture(GL_TEXTURE_2D, texture[3]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, height, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, height, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -120.0 + offsetZ);
            glEnd();

            // Other faces
            glBindTexture(GL_TEXTURE_2D, texture[4]);
            glBegin(GL_QUADS);
            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -120.0 + offsetZ);

            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -120.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -125.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -125.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -125.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -125.0 + offsetZ);
            glEnd();
        }
    }

    // Draw trees on the grass 2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            float offsetX = i * 10.0f;
            float offsetZ = j * 10.0f;
            float treeHeight = 15.0f; // Adjust the tree height as needed

            glBindTexture(GL_TEXTURE_2D, texture[5]); // Bind the tree texture

            glBegin(GL_QUADS);
            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -121.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -121.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -121.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -121.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -124.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -124.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -121.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -121.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -124.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -121.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -121.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -124.0 + offsetZ);

            // Top face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f + treeHeight, -121.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f + treeHeight, -121.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0 + offsetX, 5.5f, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0 + offsetX, 5.5f, -124.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0 + offsetX, 5.5f, -121.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0 + offsetX, 5.5f, -121.0 + offsetZ);
            glEnd();

            // Draw the cube leaves on top of the tree trunk
            glBindTexture(GL_TEXTURE_2D, texture[6]); // Bind the leaf texture

            glBegin(GL_QUADS);
            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -120.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -125.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -125.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -125.0 + offsetZ);

            // Top face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 9.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 9.5f + treeHeight, -120.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0 + offsetX, 5.5f + treeHeight, -120.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0 + offsetX, 5.5f + treeHeight, -120.0 + offsetZ);
            glEnd();
        }
    }

    //parking lot 2,2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            // Bind the texture for the ground
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -135.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -125.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -135.0 + offsetZ);
            glEnd();
        }
    }

    // Draw the road 2,2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            glBindTexture(GL_TEXTURE_2D, texture[1]);

            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -110.0 + offsetZ); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -110.0 + offsetZ);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -105.0 + offsetZ);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -105.0 + offsetZ); // Top-left corner
            glEnd();
        }
    }

    //parking bushes 2,2
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            float offsetX = i * 10.0f;
            float offsetZ = j * 10.0f;
            float height = 6.5f;

            // Top face
            glBindTexture(GL_TEXTURE_2D, texture[7]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, height, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, height, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -105.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -105.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -105.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -105.0 + offsetZ);

            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -105.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -105.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -105.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -105.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -100.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -100.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -105.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -100.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -105.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -105.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -100.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -100.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -105.0 + offsetZ);
            glEnd();
        }
    }

    // Draw the road //front stadium
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            glBindTexture(GL_TEXTURE_2D, texture[1]);

            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -60.0 + offsetZ); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -60.0 + offsetZ);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -100.0 + offsetZ);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -100.0 + offsetZ); // Top-left corner
            glEnd();
        }
    }

    // bushes beside poles left
    for (int i = -numRepetitionsX / 4; i <= numRepetitionsX / 4; ++i) {
        for (int j = -numRepetitionsZ / 4; j <= numRepetitionsZ / 4; ++j) {
            float offsetX = i * 5.0f;
            float offsetZ = j * 5.0f;
            float height = 6.5f;

            // Top face
            glBindTexture(GL_TEXTURE_2D, texture[7]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0 + offsetX, height, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0 + offsetX, height, -90.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0 + offsetX, 5.0, -90.0 + offsetZ);

            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0 + offsetX, height, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0 + offsetX, height, -90.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0 + offsetX, height, -80.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0 + offsetX, height, -90.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0 + offsetX, height, -90.0 + offsetZ);
            glEnd();
        }
    }

    // bushes beside poles right
    for (int i = -numRepetitionsX / 4; i <= numRepetitionsX / 4; ++i) {
        for (int j = -numRepetitionsZ / 4; j <= numRepetitionsZ / 4; ++j) {
            float offsetX = i * 5.0f;
            float offsetZ = j * 5.0f;
            float height = 6.5f;

            // Top face
            glBindTexture(GL_TEXTURE_2D, texture[7]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0 + offsetX, height, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0 + offsetX, height, -90.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0 + offsetX, 5.0, -90.0 + offsetZ);

            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0 + offsetX, height, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0 + offsetX, height, -90.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0 + offsetX, height, -80.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0 + offsetX, height, -90.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0 + offsetX, height, -90.0 + offsetZ);
            glEnd();
        }
    }


    // pole bottom
    for (int i = -numRepetitionsX / 4; i <= numRepetitionsX / 4; ++i) {
        for (int j = -numRepetitionsZ / 4; j <= numRepetitionsZ / 4; ++j) {
            float offsetX = i * 5.0f;
            float offsetZ = j * 5.0f;
            float height = 5.5f;

            // Top face
            glBindTexture(GL_TEXTURE_2D, texture[8]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0 + offsetX, height, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0 + offsetX, height, -90.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0 + offsetX, 5.0, -90.0 + offsetZ);

            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0 + offsetX, height, -90.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0 + offsetX, height, -90.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0 + offsetX, height, -80.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(20.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0 + offsetX, height, -90.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0 + offsetX, 5.0, -80.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0 + offsetX, 5.0, -90.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0 + offsetX, height, -80.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0 + offsetX, height, -90.0 + offsetZ);
            glEnd();
        }
    }

    float poleRadius = 0.2f; // Adjust the radius as needed
    float poleHeight = 20.0f; // Adjust the height as needed
    // Draw cylinder poles
    glBindTexture(GL_TEXTURE_2D, texture[9]);

    for (int i = 3; i <= 3; ++i) {
        for (int j = 3; j <= 3; ++j) {
            float offsetX = i * 5.0f;
            float offsetZ = j * 5.0f;
            float baseHeight = 5.5f; // Height of the pole bottom
            float flagHeight = poleHeight + 3; // Adjust the flag height as needed

            glPushMatrix();
            glTranslatef(-6.0f + offsetX, baseHeight, -100.0f + offsetZ); // Position of the first pole
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotate the cylinder to make it upright
            gluCylinder(quadObj, poleRadius, poleRadius, poleHeight, 32, 32); // Draw the cylinder
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-3.0f + offsetX, baseHeight, -100.0f + offsetZ); // Position of the second pole
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluCylinder(quadObj, poleRadius, poleRadius, poleHeight, 32, 32);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(0.0f + offsetX, baseHeight, -100.0f + offsetZ); // Position of the third pole
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluCylinder(quadObj, poleRadius, poleRadius, poleHeight, 32, 32);
            glPopMatrix();

            // Draw the flag utem
            glBindTexture(GL_TEXTURE_2D, texture[12]);
            glPushMatrix();
            glTranslatef(-6.0f + offsetX, flagHeight, -100.0f + offsetZ); // Position of the flag
            glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 2.0f, 0.0f);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 2.0f, 0.0f); // Top-left corner
            glEnd();
            glPopMatrix();

            // Draw the flag msia
            glBindTexture(GL_TEXTURE_2D, texture[11]);
            glPushMatrix();
            glTranslatef(-3.0f + offsetX, flagHeight, -100.0f + offsetZ); // Position of the flag
            glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 2.0f, 0.0f);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 2.0f, 0.0f); // Top-left corner
            glEnd();
            glPopMatrix();

            // Draw the flag melaka
            glBindTexture(GL_TEXTURE_2D, texture[10]);
            glPushMatrix();
            glTranslatef(0.0f + offsetX, flagHeight, -100.0f + offsetZ); // Position of the flag
            glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 2.0f, 0.0f);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 2.0f, 0.0f); // Top-left corner
            glEnd();
            glPopMatrix();
        }
    }

    // Draw the road
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            glBindTexture(GL_TEXTURE_2D, texture[1]);

            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -165.0 + offsetZ); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -165.0 + offsetZ);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -170.0 + offsetZ);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -170.0 + offsetZ); // Top-left corner
            glEnd();
        }
    }

    //parking bushes 3
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            float offsetX = i * 10.0f;
            float offsetZ = j * 10.0f;
            float height = 6.5f;

            // Top face
            glBindTexture(GL_TEXTURE_2D, texture[7]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, height, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, height, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -175.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -175.0 + offsetZ);

            // Bottom face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -175.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -175.0 + offsetZ);

            // Front face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -175.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -175.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -175.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -175.0 + offsetZ);

            // Back face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -170.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -170.0 + offsetZ);

            // Left face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -175.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -170.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0 + offsetX, height, -175.0 + offsetZ);

            // Right face
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -175.0 + offsetZ);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -170.0 + offsetZ);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -170.0 + offsetZ);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, height, -175.0 + offsetZ);
            glEnd();
        }
    }

    // Draw the road
    for (int i = -numRepetitionsX; i <= numRepetitionsX; ++i) {
        for (int j = -numRepetitionsZ; j <= numRepetitionsZ; ++j) {
            glBindTexture(GL_TEXTURE_2D, texture[1]);

            float offsetX = i * 10.0f; // Adjust this value based on the size of your ground
            float offsetZ = j * 10.0f; // Adjust this value based on the size of your ground

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0 + offsetX, 5.0, -175.0 + offsetZ); // Bottom-left corner
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0 + offsetX, 5.0, -175.0 + offsetZ);  // Bottom-right corner
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0 + offsetX, 5.0, -180.0 + offsetZ);  // Top-right corner
            glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0 + offsetX, 5.0, -180.0 + offsetZ); // Top-left corner
            glEnd();
        }
    }

    // Draw the vertical road repeated side by side (shorter side adjacent)
    int numVerticalRoads = 1; // Number of vertical road repetitions
    for (int i = -numVerticalRoads; i <= numVerticalRoads; ++i) {
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        float offsetZ = i * 10.0f; // Adjust this value to position each vertical road segment side by side

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-70.0, 5.0, -70.0 + offsetZ); // Bottom-left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-65.0, 5.0, -70.0 + offsetZ);  // Bottom-right corner
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-65.0, 5.0, -170.0 + offsetZ);  // Top-right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0, 5.0, -170.0 + offsetZ); // Top-left corner
        glEnd();
    }

    // Draw the vertical road repeated side by side (shorter side adjacent)
    for (int i = -numVerticalRoads; i <= numVerticalRoads; ++i) {
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        float offsetZ = i * 10.0f; // Adjust this value to position each vertical road segment side by side

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0, 5.0, -70.0 + offsetZ); // Bottom-left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(65.0, 5.0, -70.0 + offsetZ);  // Bottom-right corner
        glTexCoord2f(1.0f, 1.0f); glVertex3f(65.0, 5.0, -170.0 + offsetZ);  // Top-right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0, 5.0, -170.0 + offsetZ); // Top-left corner
        glEnd();
    }

    // Draw the vertical road repeated side by side (shorter side adjacent)

    for (int i = -numVerticalRoads; i <= numVerticalRoads; ++i) {
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        float offsetZ = i * 10.0f; // Adjust this value to position each vertical road segment side by side

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-95.0, 5.0, -70.0 + offsetZ); // Bottom-left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0, 5.0, -70.0 + offsetZ);  // Bottom-right corner
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0, 5.0, -170.0 + offsetZ);  // Top-right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-95.0, 5.0, -170.0 + offsetZ); // Top-left corner
        glEnd();
    }

    for (int i = -numVerticalRoads; i <= numVerticalRoads; ++i) {
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        float offsetZ = i * 10.0f; // Adjust this value to position each vertical road segment side by side

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(95.0, 5.0, -70.0 + offsetZ); // Bottom-left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0, 5.0, -70.0 + offsetZ);  // Bottom-right corner
        glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0, 5.0, -170.0 + offsetZ);  // Top-right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(95.0, 5.0, -170.0 + offsetZ); // Top-left corner
        glEnd();
    }

    // Draw the car
    glPushMatrix();
    glTranslatef(carX, 6.0, carZ); // Position the car using carX and carZ
    drawCar1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-carX, 6.0, carZ - 30); // Position the car using carX and carZ
    drawCar2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(car3X, 6.0, car3Z); // 
    glRotatef(90, 0.0, 1.0, 0.0); // Rotate around the y-axis
    drawCar2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(car3X - 10, 7.0, car3Z); // bus
    glRotatef(90, 0.0, 1.0, 0.0); // Rotate around the y-axis
    drawBus();
    glPopMatrix();

    // Draw the parked car
    glPushMatrix();
    glTranslatef(-50.0, 6.0, -130.0); // Position the car on the ground
    glRotatef(90, 0.0, 1.0, 0.0); // Rotate around the y-axis
    drawCar1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-44.0, 6.0, -130.0); // Position the car on the ground
    glRotatef(90, 0.0, 1.0, 0.0); // Rotate around the y-axis
    drawCar2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-44.0, 6.0, -115.0); // Position the car on the ground
    glRotatef(90, 0.0, 1.0, 0.0); // Rotate around the y-axis
    drawCar1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 7, -27);
    glScaled(0.8, 0.8, 0.8);
    drawManniCharacter();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(13.5, 7, -27);
    glScaled(1.5, 1.5, 1.5);
    drawLowCharacter();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(16.5, 6.5, -27);
    glScaled(1.4, 1.4, 1.4);
    drawLimCharacter();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 0.5, -30);
    glColor3f(0.80, 0.80, 0.80);
    glScaled(1, 1, 1.5);
    rectangle();//stage
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-12, 6, -30);
    glColor3f(0.80, 0.80, 0.80);
    glScaled(0.5, 0.5, 0.5);
    rectangle();//left side
    glPopMatrix();

    glPushMatrix();
    glTranslatef(42, 6, -30);
    glColor3f(0.80, 0.80, 0.80);
    glScaled(0.5, 0.5, 0.5);
    rectangle();//right side
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-30, 0, -10);
    glRotatef(315, 0, 1, 0);
    spotlight();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30, 0, 60);
    glRotatef(45, 0, 1, 0);
    spotlight();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60, 0, 60);
    glRotatef(135, 0, 1, 0);
    spotlight();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60, 0, -10);
    glRotatef(225, 0, 1, 0);
    spotlight();
    glPopMatrix();

    for (int i = -30.9; i < 5; i++) {
        glPushMatrix();
        glTranslatef(i, -0.17, -20);
        fence();
        glPopMatrix();
    }
    for (int i = 20; i < 60; i++) {
        glPushMatrix();
        glTranslatef(i, -0.17, -20);
        fence();
        glPopMatrix();
    }

    glPushMatrix();
    SpectatorSeats();
    glPopMatrix();

    glPushMatrix();
    glTranslated(55, 0, 0);
    SpectatorSeats();
    glPopMatrix();

    glPushMatrix();
    chairColor();
    glPopMatrix();


    glPushMatrix();
    chairColor2();
    glPopMatrix();

    glPushMatrix();
    glTranslated(29, 0, 0);
    chairColor3();
    glPopMatrix();

    glPushMatrix();
    glTranslated(42, 0, 0);
    chairColor4();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-14.6, 0.5, -23.5);
    glScaled(0.8, 0.6, 0.5);
    glRotated(270, 0, 1, 0);
    stair();
    glPopMatrix();

    glPushMatrix();
    glTranslated(45, 0.5, -16);
    glRotated(90, 0, 1, 0);
    glScaled(0.5, 0.6, 0.8);
    stair();
    glPopMatrix();

    glPushMatrix();
    glTranslated(40, 0.5, -23.5);
    glScaled(0.8, 0.6, 0.5);
    glRotated(270, 0, 1, 0);
    stair();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-10, 0.5, -16);
    glRotated(90, 0, 1, 0);
    glScaled(0.5, 0.6, 0.8);
    stair();
    glPopMatrix();

    for (int i = -30.9; i < 4; i++) {
        glPushMatrix();
        glTranslatef(i, 1.5, -23.5);
        fence();
        glPopMatrix();
    }
    for (int i = 26; i < 58; i++) {
        glPushMatrix();
        glTranslatef(i, 1.5, -23.5);
        fence();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(-35.5, 0.5, 0);
    LeftNRightStair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60, 0.5, 1);
    LeftNRightStair();
    glPopMatrix();
    float k = 0;

    for (float i = 0; i < 20; i += 1.5) {
        glPushMatrix();
        glTranslatef(-37.8, 0 + k, -20 - i);
        glRotatef(90, 0, 1, 0);
        fence();
        glPopMatrix();
        k += 0.4;
    }

    for (float i = 0; i < 19; i += 1.5) {
        glPushMatrix();
        glTranslatef(62, -5.2 + k, -20 - i);
        glRotatef(90, 0, 1, 0);
        fence();
        glPopMatrix();
        k += 0.4;
    }
    glPushMatrix();
    glTranslatef(15, 7, -30);
    glColor3f(1, 1, 1);
    rectangle();//stage
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 12, -30);
    glScaled(1, 1, 0.3);
    glColor3f(1, 1, 1);
    rectangle();//stage
    glPopMatrix();

    for (int i = 5; i < 24; i++) {
        glPushMatrix();
        glTranslatef(i, 6, -27.5);
        fence();
        glPopMatrix();
    }

    for (int i = 5; i < 24; i++) {
        glPushMatrix();
        glTranslatef(i, 11, -30.5);
        fence();
        glPopMatrix();
    }

    glPushMatrix();//front door
    glTranslatef(15, 18, -42);
    glScaled(1, 6, 0.8);
    glColor3f(0.752941, 0.752941, 0.752941);
    stadium();
    glPopMatrix();

    for (float y = 0; y < 5; y++) {
        for (float i = 0; i < 100; i++) {
            glPushMatrix();//road
            glTranslatef(-35 + i, 5.5 - y, -41.2);
            glColor3f(0.329412, 0.329412, 0.329412);//dark grey
            road();
            glPopMatrix();
        }
    }
    for (float y = 0; y < 25; y++) {
        for (float i = 0; i < 100; i++) {
            glPushMatrix();//road
            glTranslatef(-35 + i, 4.5, -43 - y);
            glColor3f(0.752941, 0.752941, 0.752941);//dark grey
            road();
            glPopMatrix();
        }
    }

    for (float y = 0; y < 5; y++) {
        for (float i = 0; i < 100; i++) {
            float xPosition = -35 + i;

            if (xPosition >= 5 && xPosition <= 24) {
                continue;
            }
            if (xPosition >= 24)
            {
                glPushMatrix();//navyblue  part
                glTranslatef(xPosition, 17 - y, -44.2);
                glScaled(1, 1, 5);
                glColor3f(0.137255, 0.137255, 0.556863);
                navyBlueWall();
                glPopMatrix();



            }
            glPushMatrix();//navyblue  part
            glTranslatef(xPosition, 17 - y, -41.2);
            glColor3f(0.137255, 0.137255, 0.556863);
            navyBlueWall();
            glPopMatrix();
        }
    }
    //tiang
    for (int i = 0; i < 15; i += 5) {
        glPushMatrix();//front door
        glTranslatef(60 - i, 6, -44);
        //glScaled(1, 6, 0.4);
        glColor3f(1, 0.5, 0);
        Cylinder(0.3, 8, 10);
        glPopMatrix();
    }

    //tiang
    for (int i = 0; i < 15; i += 5) {
        glPushMatrix();//front door
        glTranslatef(4 - i, 6, -44);
        //glScaled(1, 6, 0.4);
        glColor3f(1, 0.5, 0);
        Cylinder(1, 18, 10);
        glPopMatrix();
    }
    //tiang
    for (int i = 0; i < 15; i += 5) {
        glPushMatrix();//front door
        glTranslatef(35.5 - i, 6, -44);
        //glScaled(1, 6, 0.4);
        glColor3f(1, 0.5, 0);
        Cylinder(1, 18, 10);
        glPopMatrix();
    }
    //bumbung
    glPushMatrix();
    glTranslatef(15, 22.5, -26);
    glScaled(3, 1, 2);
    glColor3f(0.329412, 0.329412, 0.329412);
    bumbung();
    glPopMatrix();

    //front bumbung
    glPushMatrix();
    glTranslatef(15, 18, -59);
    glScaled(2, 1, 2);
    glColor3f(0.329412, 0.329412, 0.329412);
    bumbung();
    glPopMatrix();
    //tiang
    glPushMatrix();
    glTranslatef(30, 4, -65);
    glColor3f(1, 0.5, 0);
    Cylinder(1.5, 15, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(25, 4, -65);
    glColor3f(1, 0.5, 0);
    Cylinder(1.5, 15, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, 4, -65);
    glColor3f(1, 0.5, 0);
    Cylinder(1.5, 15, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 4, -65);
    glColor3f(1, 0.5, 0);
    Cylinder(1.5, 15, 10);
    glPopMatrix();

    //right small bumbung
    glPushMatrix();
    glTranslatef(-12.5, 10.5, -42.5);
    glRotatef(165, 1, 0, 0);
    glScaled(0.4, 1, 0.3);
    glColor3f(0.329412, 0.329412, 0.329412);
    bumbung();
    glPopMatrix();

    //left small bumbung
    glPushMatrix();
    glTranslatef(42.5, 10.5, -42.5);
    glRotatef(165, 1, 0, 0);
    glScaled(0.4, 1, 0.3);
    glColor3f(0.329412, 0.329412, 0.329412);
    bumbung();
    glPopMatrix();

    // glPushMatrix();
   //  glEnable(GL_TEXTURE_2D);
    // glTranslatef(42.5, 10.5, -42.5);
   //  window();
   //  glDisable(GL_TEXTURE_2D);
   //  glPopMatrix();
    glPushMatrix();
    glTranslated(0, 0, 10);
    glScaled(2.5, 1, 2.5);
    wholefield();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-50, 0, 0);
    ALLgrass();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5, 0, 17);
    goal();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(48.0, 0, 25);
    glRotatef(180, 0, 1, 0);
    goal();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.329412, 0.329412, 0.329412);
    bumbung();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 22, -35);
    glScaled(1, 5, 0.1);
    glColor3f(0.55, 0.09, 0.09);
    RedRectangle();
    glPopMatrix();
    //logo
    glPushMatrix();
    glTranslatef(15, 20, -35);
    logo();
    glPopMatrix();

    //screen
    glPushMatrix();
    glTranslatef(15, 16, -35);
    screen();
    glPopMatrix();

    //groupname
    glPushMatrix();
    glTranslatef(100, 16, -135);
    glScaled(0.4, 1, 0.3);
    groupname();
    glPopMatrix();

    //stadium word
    glPushMatrix();
    glTranslated(14.5, 12, -50.5);
    glRotated(180, 0, 1, 0);
    glScaled(0.8, 0.5, 0.5);
    drawTexturedRectangle();
    glPopMatrix();

    //Display Kompleks Sukan
    glPushMatrix();
    glScalef(5, 5, 5);
    glTranslatef(-23, 1.25, -29);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    WholeKompleksSukan();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timerFunc(int value) {
    updateCarPosition();
    glutTimerFunc(20, timerFunc, 0); // Call this function again after 16 ms (approx. 60 FPS)
}


void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        cameraX += cos(cameraYaw);
        cameraZ -= sin(cameraYaw);
        break;
    case 'W':
        cameraX += cos(cameraYaw);
        cameraZ -= sin(cameraYaw);
        break;
    case 's':
        cameraX -= cos(cameraYaw);
        cameraZ += sin(cameraYaw);
        break;
    case 'S':
        cameraX -= cos(cameraYaw);
        cameraZ += sin(cameraYaw);
        break;
    case 'a':
        cameraX -= sin(cameraYaw);
        cameraZ -= cos(cameraYaw);
        break;
    case 'A':
        cameraX -= sin(cameraYaw);
        cameraZ -= cos(cameraYaw);
        break;
    case 'd':
        cameraX += sin(cameraYaw);
        cameraZ += cos(cameraYaw);
        break;
    case 'D':
        cameraX += sin(cameraYaw);
        cameraZ += cos(cameraYaw);
        break;
    case 'q':
        cameraY += 0.5f;
        break;
    case 'Q':
        cameraY += 0.5f;
        break;
    case 'z':
        cameraY -= 0.5f;
        break;
    case 'Z':
        cameraY -= 0.5f;
        break;
    }
    glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMouseDragging = true;
            mouseX = x;
            mouseY = y;
        }
        else {
            isMouseDragging = false;
        }
    }
}

void motionFunc(int x, int y) {
    if (isMouseDragging) {
        int dx = x - mouseX;
        int dy = y - mouseY;

        cameraYaw += dx * 0.005;
        cameraPitch += dy * 0.005;

        if (cameraPitch > 89.0f * 3.14159 / 180.0f)
            cameraPitch = 89.0f * 3.14159 / 180.0f;
        if (cameraPitch < -89.0f * 3.14159 / 180.0f)
            cameraPitch = -89.0f * 3.14159 / 180.0f;

        mouseX = x;
        mouseY = y;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("UTeM Stadium");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    string bgmFile = "Lagu UTeM Terbilang.wav";
    playBackgroundMusic(bgmFile);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glEnable(GL_DEPTH_TEST);

    cout << "Press Key To Move";
    cout << "\n'W' = Move Forward \n'S' = Move Backward \n'A' = Move LeftSide \n'D' = Move RightSide \n'Q' = Move Upwards \n'Z' = Move Downwards";

    // Load the texture
    texture[0] = LoadTexture("Source/parkinglot1.bmp", 512, 512);
    texture[1] = LoadTexture("Source/parkingroad.bmp", 512, 512);
    texture[2] = LoadTexture("Source/parkinglot2.bmp", 512, 512);
    texture[3] = LoadTexture("Source/grass.bmp", 612, 612);
    texture[4] = LoadTexture("Source/cement.bmp", 5184, 3456);
    texture[5] = LoadTexture("Source/treewood.bmp", 640, 960);
    texture[6] = LoadTexture("Source/tree leaves.bmp", 640, 1138);
    texture[7] = LoadTexture("Source/bushes.bmp", 384, 256);
    texture[8] = LoadTexture("Source/whitetiles.bmp", 526, 351);
    texture[9] = LoadTexture("Source/silverpole.bmp", 256, 384);
    texture[10] = LoadTexture("Source/melaka.bmp", 1200, 600);
    texture[11] = LoadTexture("Source/malaysia.bmp", 1584, 792);
    texture[12] = LoadTexture("Source/utem.bmp", 500, 262);
    texture[13] = LoadTexture("Source/logo.bmp", 256, 256);
    texture[14] = LoadTexture("Source/stadium.bmp", 1193, 256);
    texture[15] = LoadTexture("Source/texture.bmp", 256, 256);
    texture[16] = LoadTexture("Source/stadium1.bmp", 1280, 852);
    texture[17] = LoadTexture("Source/stadium2.bmp", 1280, 854);
    texture[18] = LoadTexture("Source/stadium3.bmp", 1280, 854);
    texture[19] = LoadTexture("Source/stadium4.bmp", 1280, 854);
    texture[20] = LoadTexture("Source/stadium5.bmp", 1280, 854);
    texture[21] = LoadTexture("Source/stadium6.bmp", 1280, 854);
    texture[22] = LoadTexture("Source/name.bmp", 940, 632);
    texture[23] = LoadTexture("Source/grey wall.bmp", 600, 450); //texture for wall
    texture[24] = LoadTexture("Source/yellow wall.bmp", 160, 160); //texture for wall
    texture[25] = LoadTexture("Source/blue roof.bmp", 900, 900); //texture for wall
    texture[26] = LoadTexture("Source/Dewan Sukan.bmp", 1908, 485); //texture for Dewan Sukan
    texture[27] = LoadTexture("Source/wall.bmp", 338, 338); //texture for Dewan Sukan bg
    texture[28] = LoadTexture("Source/Pusat Sukan.bmp", 469, 186); //texture for Pusat Sukan
    texture[29] = LoadTexture("Source/black glass.bmp", 432, 432); //texture for black glass door
    texture[30] = LoadTexture("Source/metal.bmp", 3500, 3500); //texture for metal frame
    texture[31] = LoadTexture("Source/alum roof.bmp", 224, 224); //texture for aluminium roof
    texture[32] = LoadTexture("Source/window.bmp", 418, 228); //texture for aluminium roof
    texture[33] = LoadTexture("Source/floor.bmp", 360, 270); //texture for floor
    texture[34] = LoadTexture("Source/roller shutter 1.bmp", 805, 828); //texture for roller shutter 
    texture[35] = LoadTexture("Source/roller shutter.bmp", 1293, 878); //texture for roller shutter
    texture[36] = LoadTexture("Source/white door.bmp", 320, 745); //texture for roller shutter
    texture[37] = LoadTexture("Source/Kompleks Sukan.bmp", 600, 189); //texture for roller shutter
    texture[38] = LoadTexture("Source/roof.bmp", 280, 280);
    // Set up the timer function to update the car's position
    glutTimerFunc(16, timerFunc, 0);
    //screen timer
    glutTimerFunc(2000, updateTexture, 0);


    glutMainLoop();

    // Free the texture after the main loop exits
    FreeTexture(texture[0]);
    FreeTexture(texture[1]);
    FreeTexture(texture[2]);
    FreeTexture(texture[3]);
    FreeTexture(texture[4]);
    FreeTexture(texture[5]);
    FreeTexture(texture[6]);
    FreeTexture(texture[7]);
    FreeTexture(texture[8]);
    FreeTexture(texture[9]);
    FreeTexture(texture[10]);
    FreeTexture(texture[11]);
    FreeTexture(texture[12]);
    FreeTexture(texture[13]);
    FreeTexture(texture[14]);
    FreeTexture(texture[15]);
    FreeTexture(texture[16]);
    FreeTexture(texture[17]);
    FreeTexture(texture[18]);
    FreeTexture(texture[19]);
    FreeTexture(texture[20]);
    FreeTexture(texture[21]);
    FreeTexture(texture[22]);
    FreeTexture(texture[23]);
    FreeTexture(texture[24]);
    FreeTexture(texture[25]);
    FreeTexture(texture[26]);
    FreeTexture(texture[27]);
    FreeTexture(texture[28]);
    FreeTexture(texture[29]);
    FreeTexture(texture[30]);
    FreeTexture(texture[31]);
    FreeTexture(texture[32]);
    FreeTexture(texture[33]);
    FreeTexture(texture[34]);
    FreeTexture(texture[35]);
    FreeTexture(texture[36]);
    FreeTexture(texture[37]);
    FreeTexture(texture[38]);
    return 0;
}
