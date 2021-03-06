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
#include "Hole.h"

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
float sceneRotationMax = 20.00;
float rotationSpeed = 0.1;

float floorSize = 200;
float holeSize = floorSize/8;

//physics variables
float gravityConstant = .003;
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
float ballPosition[2] = { 175, 175 };
float ballRotation[2] = { 0, 0 };

//initial lives
int lives = 3;

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
float e_shiny = 76.8;

//Ruby
float r_amb[] = { 0.1745, 0.01175, 0.01175, 0.55 };
float r_dif[] = { 0.61424, 0.04136, 0.04136, 0.55 };
float r_spec[] = { 0.727811, 0.626959, 0.626959, 0.55 };
float r_shiny = 76.8;

//Polished Bronze
float pb_amb[] = {0.25    ,     0.148 ,       0.06475   ,   1.0};
float pb_dif[] = {0.4       ,   0.2368    ,   0.1036    ,   1.0};
float pb_spec[] ={0.774597   ,  0.458561   ,  0.200621  ,   1.0};
float pb_shiny =76.8;

//Black
float b_amb[] = { 0.0, 0.0, 0.0, 1.0 };
float b_dif[] = { 0.0, 0.0, 0.0, 1.0 };
float b_spec[] = { 0.0225, 0.0225, 0.0225, 1.0 };
float b_shiny = 12.8;

//level stuff
int currentLevel = 0;

//lists
vector< vector<walls> > wallList;
vector< vector<hole> >  holeList;
vector<hole> startPositionList;
vector<hole> winPosition;

//Prints manual to console
void printManual()
{

}

//Draws enclosed box
void drawFloor(float size)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, e_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, e_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, e_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, e_shiny);
	glBegin(GL_QUADS);	
	glNormal3d(0, 0, 1);
	glVertex3f(size, size, 0);
	glVertex3f(-size, size, 0);
	glVertex3f(-size, -size, 0);
	glVertex3f(size, -size, 0);
	glEnd();
}



void designLevel(){
	wallList.push_back(vector<walls>());
	wallList[0].push_back(walls(point3D(-200, 200, 0), 400, 50, true));
	wallList[0].push_back(walls(point3D(200, -200, 0), 400, 50, false));
	wallList[0].push_back(walls(point3D(-200, -200, 0), 400, 50, true));
	wallList[0].push_back(walls(point3D(-200, -200, 0), 400, 50, false));

	wallList[0].push_back(walls(point3D(0, 0, 0), 200, 50, false));
	wallList[0].push_back(walls(point3D(-150, 0, 0), 150, 50, true));
	wallList[0].push_back(walls(point3D(-150, 50, 0), 150, 50, false));
	wallList[0].push_back(walls(point3D(-150, -100, 0), 100, 50, false));
	wallList[0].push_back(walls(point3D(0, -200, 0), 100, 50, false));
	wallList[0].push_back(walls(point3D(0, 0, 0), 150, 50, true));
	//wallList[0].push_back(walls(point3D(150, -200, 0), 50, 50, false));
	wallList[0].push_back(walls(point3D(150, -100, 0), 100, 50, false));
	wallList[0].push_back(walls(point3D(150, -100, 0), 100, 50, false));
	wallList[0].push_back(walls(point3D(50, 150, 0), 150, 50, true));

	wallList.push_back(vector<walls>());
	wallList[1].push_back(walls(point3D(-200, 200, 0), 400, 50, true));
	wallList[1].push_back(walls(point3D(200, -200, 0), 400, 50, false));
	wallList[1].push_back(walls(point3D(-200, -200, 0), 400, 50, true));
	wallList[1].push_back(walls(point3D(-200, -200, 0), 400, 50, false));

	wallList[1].push_back(walls(point3D(-200,100, 0), 150, 50, true));
	wallList[1].push_back(walls(point3D(-150, 150, 0), 50, 50, false));
	wallList[1].push_back(walls(point3D(-50,100,0), 55, 50, false));
	wallList[1].push_back(walls(point3D(0,0,0), 200, 50, false));
	wallList[1].push_back(walls(point3D(-100,0,0), 100, 50, true));
	wallList[1].push_back(walls(point3D(-200,0,0), 50, 50, true));
	wallList[1].push_back(walls(point3D(-100,-150,0), 150, 50, false));
	wallList[1].push_back(walls(point3D(-200,-150,0), 50, 50, true));
	wallList[1].push_back(walls(point3D(150,-150,0),350, 50, false));
	wallList[1].push_back(walls(point3D(100,-200,0),350,50,false));

	wallList.push_back(vector<walls>());
	wallList[2].push_back(walls(point3D(-200, 200, 0), 400, 50, true));
	wallList[2].push_back(walls(point3D(200, -200, 0), 400, 50, false));
	wallList[2].push_back(walls(point3D(-200, -200, 0), 400, 50, true));
	wallList[2].push_back(walls(point3D(-200, -200, 0), 400, 50, false));

	wallList[2].push_back(walls(point3D(-150,-50,0), 350, 50, true));
	wallList[2].push_back(walls(point3D(50,-200,0),100,50, false));
	wallList[2].push_back(walls(point3D(-50,-200,0), 100, 50, false));
	wallList[2].push_back(walls(point3D(-150,-50,0), 100, 50, false));
	wallList[2].push_back(walls(point3D(-150,150,0),50,50,false));
	wallList[2].push_back(walls(point3D(-100,100,0), 100, 50, false));
	wallList[2].push_back(walls(point3D(150,100,0), 100, 50, false));
	wallList[2].push_back(walls(point3D(0,-50,0),200,50,false));

	holeList.push_back(vector <hole>());
	holeList[0].push_back(hole(25,25));
	holeList[0].push_back(hole(175,125));
	holeList[0].push_back(hole(175,-175));
	holeList[0].push_back(hole(25,-175));
	holeList[0].push_back(hole(25,-125));
	holeList[0].push_back(hole(-75,-175));
	holeList[0].push_back(hole(-75,-175));
	holeList[0].push_back(hole(-175,175));
	holeList[0].push_back(hole(-25,25));

	holeList.push_back(vector <hole>());
	holeList[1].push_back(hole(-75, 75));
	holeList[1].push_back(hole(-75, 125));
	holeList[1].push_back(hole(25, 125));
	holeList[1].push_back(hole(-175, -25));
	holeList[1].push_back(hole(-175, 25));
	holeList[1].push_back(hole(-175, -125));
	holeList[1].push_back(hole(25, -125));
	holeList[1].push_back(hole(-25, -125));
	holeList[1].push_back(hole(25, -75));
	holeList[1].push_back(hole(75, 25));

	holeList.push_back(vector <hole>());
	holeList[2].push_back(hole(175,-25));
	holeList[2].push_back(hole(125,25));
	holeList[2].push_back(hole(75,125));
	holeList[2].push_back(hole(-25,125));
	holeList[2].push_back(hole(-75, 25));
	holeList[2].push_back(hole(-125, 175));
	holeList[2].push_back(hole(-125, -125));
	holeList[2].push_back(hole(25, -125));
	holeList[2].push_back(hole(75, -125));
	holeList[2].push_back(hole(75, -175));
	holeList[2].push_back(hole(125, -175));
	holeList[2].push_back(hole(175, -175));
	holeList[2].push_back(hole(175, -125));

	startPositionList.push_back(hole(-50, 175));
	startPositionList.push_back(hole(-175, 175));
	startPositionList.push_back(hole(-175, 175));

	winPosition.push_back(hole(-25,175));
	winPosition.push_back(hole(175,175));
	winPosition.push_back(hole(175,-175));
}


//Draws ball
void drawBall()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, r_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, r_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, r_spec);
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
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pb_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pb_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pb_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pb_shiny);
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
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, b_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, b_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, b_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, b_shiny);
	for (int i = 0; i < holeList[currentLevel].size(); i++)
	{
		glPushMatrix();
		glTranslatef(holeList[currentLevel][i].x,holeList[currentLevel][i].y,holeList[currentLevel][i].z);
		glutSolidCone(holeSize,0,30,1);		
		glPopMatrix();		
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pb_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pb_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pb_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pb_shiny);

	glPushMatrix();
	glTranslatef(winPosition[currentLevel].x,winPosition[currentLevel].y,winPosition[currentLevel].z);
	glutSolidTeapot(holeSize/3);		
	glPopMatrix();		


}


void nextLevel()
{
	currentLevel++;
	if(currentLevel==wallList.size())
	{
		currentLevel = 0;
	}
	ballPosition[0] = startPositionList[currentLevel].x;
	ballPosition[1] = startPositionList[currentLevel].y;
	ballRotation[0] = 0;
	ballRotation[1] = 0;
	sceneRotation[0] = 0;
	sceneRotation[1] = 0;
	dx = 0;
	dy = 0;
}
void gameOver()
{
	currentLevel = 0;
	lives = 3;
}
void death()
{
	lives--;
	if(lives==0)
	{
		gameOver();
	}
	ballPosition[0] = startPositionList[currentLevel].x;
	ballPosition[1] = startPositionList[currentLevel].y;
	ballRotation[0] = 0;
	ballRotation[1] = 0;
	sceneRotation[0] = 0;
	sceneRotation[1] = 0;
	dx = 0;
	dy = 0;
}

void checkCollisions()
{

	for (int i = 0; i < wallList[currentLevel].size(); i++){

		if (ballPosition[1] - ballSizeDefault <wallList[currentLevel][i].getBR().y && ballPosition[1] + ballSizeDefault > wallList[currentLevel][i].getBL().y)
		{
			if (ballPosition[0] - ballSizeDefault>= wallList[currentLevel][i].getBL().x && ballPosition[0] + dx - ballSizeDefault<= wallList[currentLevel][i].getBL().x)
			{
				dx = 0;
				drotx = 0;
				ballPosition[0] = wallList[currentLevel][i].getBL().x + ballSizeDefault;
			}
			else if (ballPosition[0] +  ballSizeDefault<= wallList[currentLevel][i].getBL().x && ballPosition[0] + dx + ballSizeDefault>= wallList[currentLevel][i].getBL().x)
			{
				dx = 0;
				drotx = 0;
				ballPosition[0] = wallList[currentLevel][i].getBL().x - ballSizeDefault;
			}
		}
		if (ballPosition[0] - ballSizeDefault <wallList[currentLevel][i].getBR().x && ballPosition[0] + ballSizeDefault > wallList[currentLevel][i].getBL().x)
		{

			if (ballPosition[1] - ballSizeDefault>= wallList[currentLevel][i].getBL().y && ballPosition[1] + dy - ballSizeDefault<= wallList[currentLevel][i].getBL().y)
			{
				dy = 0;
				droty = 0;
				ballPosition[1] = wallList[currentLevel][i].getBL().y + ballSizeDefault;
			}
			else if (ballPosition[1] + ballSizeDefault<= wallList[currentLevel][i].getBL().y && ballPosition[1] + dy + ballSizeDefault>= wallList[currentLevel][i].getBL().y)
			{
				dy = 0;
				droty = 0;
				ballPosition[1] = wallList[currentLevel][i].getBL().y - ballSizeDefault;
			}
		}
	}
	for (int i = 0; i < holeList[currentLevel].size(); i++){
		if(pow(ballPosition[0]-holeList[currentLevel][i].x,2) + pow(ballPosition[1]-holeList[currentLevel][i].y,2) <= pow(ballSizeDefault*0.9,2))
		{
			death();
		}
	}
	if(pow(ballPosition[0]-winPosition[currentLevel].x,2) + pow(ballPosition[1]-winPosition[currentLevel].y,2) <= pow(ballSizeDefault*0.7,2))
	{
		nextLevel();
	}

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
			sceneRotation[1] += rotationSpeed;
		}
	}
	if (keyboardStates[GLUT_KEY_RIGHT])
	{
		if (sceneRotation[1] >= -sceneRotationMax)
		{
			sceneRotation[1] -= rotationSpeed;
		}
	}if (keyboardStates[GLUT_KEY_UP])
	{
		if (sceneRotation[0] <= sceneRotationMax)
		{
			sceneRotation[0] += rotationSpeed;
		}
	}if (keyboardStates[GLUT_KEY_DOWN])
	{
		if (sceneRotation[0] >= -sceneRotationMax)
		{
			sceneRotation[0] -= rotationSpeed;
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

void disableLighting()
{
	glDisable(GL_LIGHTING);
}
void displayText()
{
	glColor3f(0.0, 1.0, 0.0); // Green

	void * font = GLUT_BITMAP_9_BY_15;
	string s = "";

	glRasterPos2i(10, screenSize[1]-15);
	s = "Current Level: "+to_string(currentLevel)+"                              Lives: "+to_string(lives);
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}

void displayRotation()
{

	glColor3f(1,0,0);
	glPointSize(15);
	glBegin(GL_POINTS);
	glVertex2f(-sceneRotation[1]/sceneRotationMax*50+screenSize[0]-100,sceneRotation[0]/sceneRotationMax*50+screenSize[1]-100);
	glEnd();

	glColor3f(0.678,.847,0.902);
	glBegin(GL_QUADS);
	glVertex2f(screenSize[0]-150,screenSize[1]-150);
	glVertex2f(screenSize[0]-150,screenSize[1]-50);
	glVertex2f(screenSize[0]-50,screenSize[1]-50);
	glVertex2f(screenSize[0]-50,screenSize[1]-150);
	glEnd();
}

//Main display function
void display(void)
{
	//Enables depth test for proper z buffering
	glEnable(GL_DEPTH_TEST);
	lighting();

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
	drawFloor(floorSize);
	drawWalls();
	drawHoles();

	//Draw ball
	drawBall();


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, screenSize[0], 0.0, screenSize[1]);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	disableLighting();

	displayText();
	displayRotation();

	glPopMatrix();
	glPopMatrix();

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

	//Enables backculling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

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