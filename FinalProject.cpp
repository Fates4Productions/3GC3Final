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
#include "Walls.h"

using namespace std;

//Declaring Variables

//Screen size for glut
int screenSize[2] = { 1280, 720 };

//Initial camera position
int cameraPos[3] = { 300, 300, 300 };
float cameraAngle = 0;

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;
float lastx, lasty;
bool mouseActive;

//Scene Rotation angle
float sceneRotation[3];
float sceneRotationMax = 5.00;

float boxSize = 200;

//physics variables
float gravityConstant = .00003;
float dx = 0;
float dy = 0;
float drotx = 0;
float droty = 0;

//Initial light source variable
float lightsource1[4] = { 0.0, 0.0, 150.0, 1.0 };
float lightsource2[4] = { 0, -100.0, 110.0, 1.0 };

//Initial ball size
float ballSizeDefault = 15.0;
int ballRenderQuality = 30;

//initial ball position and rotation
float ballPosition[2] = { 0, 0 };
float ballRotation[2] = { 0, 0 };

//
bool keyboardStates[256] = { false };

//Light variables
float amb[4] = { 1.0, 1, 1, 1 };
float diff[4] = { 1, 1, 1, 1 };
float spec[4] = { 1, 1, 1, 0 };

//Material Variables

//Emerald
float e_amb[] = { 0.0215, 0.1745, 0.0215, 0.55 };
float e_dif[] = { 0.07568, 0.61424, 0.07568, 0.55 };
float e_spec[] = { 0.633, 0.727811, 0.633, 0.55 };
float e_emis[] = { 0.0, 0.0, 0.0, 0.0 };
float e_shiny = 76.8;

//Ruby
float r_amb[] = { 0.1745, 0.01175, 0.01175, 0.55 };
float r_dif[] = { 0.61424, 0.04136, 0.04136, 0.55 };
float r_spec[] = { 0.727811, 0.626959, 0.626959, 0.55 };
float r_emis[] = { 0.0, 0.0, 0.0, 0.0 };
float r_shiny = 76.8;

//Black
float b_amb[] = { 0.0, 0.0, 0.0, 1.0 };
float b_dif[] = { 0.0, 0.0, 0.0, 1.0 };
float b_spec[] = { 0.0225, 0.0225, 0.0225, 1.0 };
float b_emis[] = { 0.0, 0.0, 0.0, 1.0 };
float b_shiny = 12.8;

//level stuff
int currentLevel = 0;

//lists
vector< vector<walls> > wallList;
//str::vector< <holeClass> > holeList;

//Prints manual to console
void printManual()
{

}

//Draws enclosed box
void drawBoxArea(float size)
{
	glBegin(GL_QUADS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, e_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, e_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, e_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, e_shiny);
	//Left cube face
	glNormal3d(-1, 0, 0);
	glVertex3f(size, size, size*0.25);
	glVertex3f(size, -size, size*0.25);
	glVertex3f(size, -size, 0);
	glVertex3f(size, size, 0);
	//Back cube face
	glNormal3d(0, 1, 0);
	glVertex3f(-size, -size, size*0.25);
	glVertex3f(size, -size, size*0.25);
	glVertex3f(size, -size, 0);
	glVertex3f(-size, -size, 0);
	//Right cube face
	glNormal3d(1, 0, 0);
	glVertex3f(-size, -size, size*0.25);
	glVertex3f(-size, size, size*0.25);
	glVertex3f(-size, size, 0);
	glVertex3f(-size, -size, 0);
	//Front cube face
	glNormal3d(0, -1, 0);
	glVertex3f(size, size, size*0.25);
	glVertex3f(-size, size, size*0.25);
	glVertex3f(-size, size, 0);
	glVertex3f(size, size, 0);
	//Bottom cube face
	glNormal3d(0, 0, 1);
	glVertex3f(size, size, 0);
	glVertex3f(-size, size, 0);
	glVertex3f(-size, -size, 0);
	glVertex3f(size, -size, 0);
	glEnd();
}

void designLevel(){
	wallList.push_back(vector<walls>());
	wallList[0].push_back(walls(point3D(0,0,0),10,10,true));
}

//Draws ball
void drawBall()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, r_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, r_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, r_spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, r_emis);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, r_shiny);
	glPushMatrix();
	glTranslatef(ballPosition[0], ballPosition[1], ballSizeDefault);
	glPushMatrix();
	//rotate ball
	glRotatef(ballRotation[0], 0, 1, 0);
	glRotatef(ballRotation[1], -1, 0, 0);

	//draws the ball
	glutSolidSphere(ballSizeDefault, ballRenderQuality, ballRenderQuality);
	glPopMatrix();
	glPopMatrix();
}

//Draws walls
void drawWalls()
{
	for (int i = 0; i < wallList[currentLevel].size(); i++)
	{
		glBegin(GL_QUADS);
		//material
		
			glVertex3f(wallList[currentLevel][i].getBL().x, wallList[currentLevel][i].getBL().y, wallList[currentLevel][i].getBL().z);
			glVertex3f(wallList[currentLevel][i].getTL().x, wallList[currentLevel][i].getTL().y, wallList[currentLevel][i].getTL().z);
			glVertex3f(wallList[currentLevel][i].getTR().x, wallList[currentLevel][i].getTR().y, wallList[currentLevel][i].getTR().z);
			glVertex3f(wallList[currentLevel][i].getBR().x, wallList[currentLevel][i].getBR().y, wallList[currentLevel][i].getBR().z);
		


		glEnd();
	}
}

//Draws holes
void drawHoles()
{


}

//Draws level
void drawLevel()
{
	drawWalls();
	drawHoles();
}

void checkCollisions()
{
	if (dx < 0 && ballPosition[0] + dx - ballSizeDefault< -boxSize)
	{
		dx = 0;
		drotx = 0;
		ballPosition[0] = -boxSize + ballSizeDefault;
	}
	else if (dx > 0 && ballPosition[0] + dx + ballSizeDefault> boxSize)
	{
		dx = 0;
		drotx = 0;
		ballPosition[0] = boxSize - ballSizeDefault;
	}
	if (dy < 0 && ballPosition[1] + dy - ballSizeDefault< -boxSize)
	{
		dy = 0;
		droty = 0;
		ballPosition[1] = -boxSize + ballSizeDefault;
	}
	else if (dy > 0 && ballPosition[1] + dy + ballSizeDefault> boxSize)
	{
		dy = 0;
		droty = 0;
		ballPosition[1] = boxSize - ballSizeDefault;
	}
	/*
	for (int i = 0; i < wallList.length(); i++){
		if (list[i].orientation = x)
		{
			if (ballPosition[1] + ballSizeDefault <list[i].y && ballPosition[1] + ballSizeDefault > -list[i].y)
			{

				if (dx < 0 && ballPosition[0] + dx - ballSizeDefault< list[i].x)
				{
					dx = 0;
					drotx = 0;
					ballPosition[0] = list[i].x + ballSizeDefault;
				}
				else if (dx > 0 && ballPosition[0] + dx + ballSizeDefault> list[i].x)
				{
					dx = 0;
					drotx = 0;
					ballPosition[0] = list[i].x - ballSizeDefault;
				}
			}

		}
		else if (list[i].orientation = y)
		{
			if (ballPosition[0] + ballSizeDefault <list[i].x && ballPosition[0] + ballSizeDefault > -list[i].x)
			{

				if (dy < 0 && ballPosition[1] + dy - ballSizeDefault< list[i].y)
				{
					dy = 0;
					droty = 0;
					ballPosition[1] = list[i].y + ballSizeDefault;
				}
				else if (dx > 0 && ballPosition[0] + dx + ballSizeDefault> list[i].y)
				{
					dy = 0;
					droty = 0;
					ballPosition[1] = list[i].y - ballSizeDefault;
				}
			}

		}

	}
	*/

}


void ballPhysics()
{
	float gx = sind(sceneRotation[1]) * gravityConstant;
	float gy = -sind(sceneRotation[0]) * gravityConstant;
	dx += gx;
	dy += gy;

	float rax = dx*dx / ballSizeDefault;
	float ray = dy*dy / ballSizeDefault;
	drotx += dx / (ballSizeDefault * 100);
	droty += dy / (ballSizeDefault * 100);


	checkCollisions();
	

	ballPosition[0] += dx;
	ballPosition[1] += dy;

	ballRotation[0] += drotx;
	ballRotation[1] += droty;
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
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &objX, &objY, &objZ);

	return point3D(objX, objY, objZ);
}

//Handling keyboard input
void kbd(unsigned char key, int x, int y)
{
	//if the "q" or esc key is pressed, quit the program
	if (key == 'q' || key == 'Q' || key == 27)
	{
		exit(0);
	}
}

//Mouse controls
void MouseClick(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
}


void SpecialKeyDown(int key, int x, int y)
{
	keyboardStates[key] = true;
}

void SpecialKeyUp(int key, int x, int y)
{
	keyboardStates[key] = false;
}


void update(void)
{
	if (keyboardStates[GLUT_KEY_LEFT])
	{
		if (sceneRotation[1] <= sceneRotationMax)
		{
			sceneRotation[1] += 0.005;
		}
	}
	if (keyboardStates[GLUT_KEY_RIGHT])
	{
		if (sceneRotation[1] >= -sceneRotationMax)
		{
			sceneRotation[1] -= 0.005;
		}
	}if (keyboardStates[GLUT_KEY_UP])
	{
		if (sceneRotation[0] <= sceneRotationMax)
		{
			sceneRotation[0] += 0.005;
		}
	}if (keyboardStates[GLUT_KEY_DOWN])
	{
		if (sceneRotation[0] >= -sceneRotationMax)
		{
			sceneRotation[0] -= 0.005;
		}
	}
	ballPhysics();
	glutPostRedisplay();
}

void camera(void)
{
	glRotatef(xrot, 1.0, 0.0, 0.0);  //rotate our camera on the x-axis (left and right)
	glRotatef(yrot, 0.0, 1.0, 0.0);  //rotate our camera on the y-axis (up and down)
	glTranslated(-xpos, -ypos, -zpos); //translate the screento the position of our camera
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


	GLfloat lightpos1[] = { lightsource1[0], lightsource1[1], lightsource1[2], lightsource1[3] };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);

	GLfloat lightpos2[] = { lightsource2[0], lightsource2[1], lightsource2[2], lightsource2[3] };
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);

	glShadeModel(GL_SMOOTH);
}

//Main display function
void display(void)
{
	//Clear the buffers and clears glMatrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

	//Sets up projection view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.78, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	camera();

	//Camera location & viewing
	gluLookAt(cameraPos[0] * sind(cameraAngle), cameraPos[1] * cosd(cameraAngle), cameraPos[2], 0, 0, 0, 0, 0, 1);

	glRotatef(sceneRotation[0], 1, 0, 0);
	glRotatef(sceneRotation[1], 0, 1, 0);

	//Drawing of scene
	drawBoxArea(boxSize);
	//drawWalls();
	//drawHoles();

	//Draw ball
	drawBall();
	
	//Draw levle
	drawLevel();

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
	glEnable(GL_DEPTH_TEST);

	//Enables backculling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	//initialize lighting
	lighting();

	//setup levels
	designLevel();

	//Registers display callback func & idle callback func
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutIdleFunc(update);

	//Starts event loop
	glutMainLoop();

	//because why not
	return(0);
}