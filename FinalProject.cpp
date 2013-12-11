/*
Labyrinth
Name: Brendan Sim, Chris Ko
macID: simb3, koh4
Student #: 1139649, 1156482
*/

#include <windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Math3DLib.h"
#include "Particle.h"
#include "TrigLib.h"

using namespace std;

//Declaring Variables

//Screen size for glut
int screenSize[2] = {1280,720};

//Initial camera position
int cameraPos[3] = {300, 300, 300};
float cameraAngle = 0;

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;
bool mouseActive;

//Scene Rotation angle
float sceneRotation[3];

//Initial light source variable
float lightsource1[4] = {0.0,0.0,150.0,1.0};
float lightsource2[4] = {0,-100.0,110.0,1.0};
float lightsourceSize = 5.0;

//Initial ball size
float ballSizeDefault = 5.0;

//
bool* keyboardStates = new bool[256];

//Bool for backface culling toggle
bool bfCulling = false;

//Light variables
float amb[4] = {1.0, 1, 1, 1};
float diff[4] = {1,1,1, 1};
float spec[4] = {1,1,1, 0};

//Material Variables

//Cyan Plastic
float cp_amb[] = {0.0,0.1,0.06,1.0};
float cp_dif[] = {0.0,0.50980392,0.50980392, 1.0};
float cp_spec[] = {0.50196078,0.50196078,.50196078,1.0};
float cp_shiny = 0.25;

//Copper
float c_amb[] = {0.19125,0.0735,0.0225, 1.0};
float c_dif[] = {0.7038,0.27048,0.0828, 1.0};
float c_spec[] = {0.256777,0.137622,0.086014, 1.0};
float c_shiny = 0.1;

//Emerald
float e_amb[] = {0.0215,0.1745,0.0215, 1.0};
float e_dif[] = {0.07568,0.61424,0.07568, 1.0};
float e_spec[] = {0.633,0.727811,0.633, 1.0};
float e_shiny = 0.6;

//Red Plastic
float rp_amb[] = {0.0,0.0,0.0, 1.0};
float rp_dif[] = {0.5,0.0,0.0, 1.0};
float rp_spec[] = {0.7,0.6,0.6, 1.0};
float rp_shiny = 0.25;

//White Rubber
float w_amb[] = {0.05,0.05,0.05, 1.0};
float w_dif[] = {0.5,0.5,0.5, 1.0};
float w_spec[] = {0.7,0.7,0.7, 1.0};
float w_shiny = 0.078125;


//Prints manual to console
void printManual()
{
	
}

//Draws enclosed box
void drawBoxArea(float size)
{
	glBegin(GL_QUADS);
		glColor3f(0.20,0.80,0.65);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cp_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cp_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cp_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, cp_shiny);
		//Left cube face
		glNormal3d(-1, 0, 0);
		glVertex3f(size, size, size*.5);
		glVertex3f(size, -size, size*.5);
		glVertex3f(size, -size, 0);
		glVertex3f(size, size, 0);
		//Back cube face
		glNormal3d(0, 1, 0);
		glVertex3f(-size, -size, size*.5);
		glVertex3f(size, -size, size*.5);
		glVertex3f(size, -size, 0);
		glVertex3f(-size, -size, 0);
		//Right cube face
		glNormal3d(1, 0, 0);
		glVertex3f(-size, -size, size*.5);
		glVertex3f(-size, size, size*.5);
		glVertex3f(-size, size, 0);
		glVertex3f(-size, -size, 0);
		//Front cube face
		glNormal3d(0,-1,0);
		glVertex3f(size,size,size*.5);
		glVertex3f(-size,size,size*.5);
		glVertex3f(-size,size,0);
		glVertex3f(size,size,0);
		//Bottom cube face
		glNormal3d(0, 0, 1);
		glVertex3f(size, size, 0);
		glVertex3f(-size,size,0);
		glVertex3f(-size,-size,0);
		glVertex3f(size,-size,0);
	glEnd();
}


//Draws ball
void drawBall()
{
	glPushMatrix();
	//set ball position
	//glTranslated(particleList[i].getPosition().x,particleList[i].getPosition().y,particleList[i].getPosition().z);
		glPushMatrix();
			//rotate ball
			//glRotatef(particleList[i].getRotation().x,1,0,0);
			//glRotatef(particleList[i].getRotation().y,0,1,0);
			//glRotatef(particleList[i].getRotation().z,0,0,1);

			//selects the particle material
			//selectMaterial(particleList[i]);

			//draws the ball
			glutSolidSphere(ballSizeDefault, 30, 30);
		glPopMatrix();
	glPopMatrix();
}

//Ray-casting to fetch 3D location from 2D coordinates
point3D fetchLocation(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble objX, objY, objZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &objX,&objY,&objZ);

	return point3D(objX,objY,objZ);
}



void backfaceCulling(void)
{
	if(bfCulling == true)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

//Handling keyboard input
void kbd(unsigned char key, int x, int y)
{
	//if the "q" or esc key is pressed, quit the program
	if(key == 'q' || key == 'Q' || key == 27)
	{
		exit(0);
	}
}

//Mouse controls
void MouseClick(int btn, int state, int x, int y)
{
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
}


void SpecialKeyDown(int key, int x, int y)
{
	keyboardStates[key] = 1;
}

void SpecialKeyUp(int key, int x, int y)
{
	keyboardStates[key] = 0;
}

//Idle function - updates camera & lighting
void update(void)
{
	//Updates light source position
	GLfloat lightpos1[] = {lightsource1[0],lightsource1[1],lightsource1[2],lightsource1[3]};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);

	GLfloat lightpos2[] = {lightsource2[0],lightsource2[1],lightsource2[2],lightsource2[3]};
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);

	//backface culling update
	backfaceCulling();
}

void camera (void)
{
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screento the position of our camera
}

//Initializes lighting
void lighting(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);

	glShadeModel(GL_SMOOTH);
}

//Main display function
void display(void)
{
	//Clear the buffers and clears glMatrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
	
	//Sets up projection view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (60.0, 1.78, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	camera();

	//Camera location & viewing
	gluLookAt(cameraPos[0]*sind(cameraAngle), cameraPos[1]*cosd(cameraAngle),cameraPos[2],0,0,0,0,0,1);
	
	glRotatef(sceneRotation[0],1,0,0);
	glRotatef(sceneRotation[1],0,1,0);

	//Drawing of scene
	drawBoxArea(200);
	//drawWalls();
	//drawHoles();

	//Draw ball
	drawBall();

	//Double buffering
	glutSwapBuffers();
}

//Main function - Program entry point
int main(int argc, char** argv)
{
	//Prints manual to console
	printManual();

	//Needed for rand() to work
	srand(time(0));

	//Initialize GLUT
	glutInit(&argc, argv);

	//Declaring buffers
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//Initializing screen & creates window
	glutInitWindowSize(screenSize[0], screenSize[1]);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Labyrinth");

	//Clearing screen
	glClearColor(0, 0, 0, 0);
	
	//Handles inputs
	glutKeyboardFunc(kbd);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
	glutMouseFunc(MouseClick);

	//Enables depth test for proper z buffering
	glEnable (GL_DEPTH_TEST);

	//Enables backculling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	//initialize lighting
	lighting();

	//Registers display callback func & idle callback func
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutIdleFunc(update);

	//Starts event loop
	glutMainLoop();

	//because why not
	return(0);
}