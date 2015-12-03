/*==================================================================================================
PROGRAMMER:				Byron Himes
COURSE:					CSC 525/625
MODIFIED BY:			Byron Himes
LAST MODIFIED DATE:		12/03/2015
DESCRIPTION:			Term Project for CSC525 - Computer Graphics

To Do:					-Add CSC messages*****
						-Add misc stuff:
							* screenshot option?
							* right click menu?
						-REFACTOR EVERYTHING!!!!

FILES:					officialProject.cpp, Constants.h, (termProject.sln, ...)
IDE/COMPILER:			MicroSoft Visual Studio 2013
INSTRUCTION FOR COMPILATION AND EXECUTION:
1.		Double click on termProject.sln	to OPEN the project
2.		Press Ctrl+F7					to COMPILE
3.		Press Ctrl+Shift+B				to BUILD (COMPILE+LINK)
4.		Press Ctrl+F5					to EXECUTE
==================================================================================================*/
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ctime>
#include <GL/glut.h>				// include GLUT library
#include "Constants.h"
#include "globals.h"
#include "setup.h"
using namespace std;

string NAMES[9] = {
	"Mercury",
	"Venus",
	"Earth",
	"Mars",
	"Jupiter",
	"Saturn",
	"Uranus",
	"Neptune",
	"Pluto"
};

Planet planets[9];
GLUquadric* quad = gluNewQuadric(); // for drawing rings
float stars[10000][3] = {};

vector<string> helptext; //store messages for HELP WINDOW

//***********************************************************************************

void helpInit(){
	//glMatrixMode(GL_PROJECTION);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	gluOrtho2D(-200, 200, -450, 450);
}

void drawText(string text_to_write){
	//Text in 3D space example
	for (unsigned int i = 0; i < text_to_write.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text_to_write[i]);
	}
}

void helptextdraw(){
	//Print text to HELP WINDOW 
	glColor3f(1, 1, 1); //color white
	for (int i = 0; i < helptext.size(); i++){
		glRasterPos2i(-290, 400 - (i * 12));
		for (int j = 0; j < helptext[i].size(); j++){
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, helptext[i][j]);
		}
	}
}

void helpDisplay(){
	glClear(GL_COLOR_BUFFER_BIT);	// draw the background
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	gluOrtho2D(-300, 300, -450, 450);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	helptextdraw();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}

//***********************************************************************************
void drawStars(){
	glDisable(GL_LIGHTING);
	glPointSize(1);

	glBegin(GL_POINTS);
	for (int i = 0; i < num_stars; i++){
		float blueness = (rand() % 10)*0.1;
		glColor3f(blueness, blueness, 1);
		glVertex3f(
			stars[i][0],
			stars[i][1],
			stars[i][2]
			);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

//***********************************************************************************
void randomizeStars(){
	for (int i = 0; i < num_stars; i++){
		double x, y, z;
		do{
			x = ((rand() % 2001) / 1000.0f) - 1;
			y = ((rand() % 2001) / 1000.0f) - 1;
			z = ((rand() % 2001) / 1000.0f) - 1;
		} while ((x*x) + (y*y) + (z*z) > 1);
		// X location:
		stars[i][0] = 70000 * x;

		// Y location:
		stars[i][1] = 70000 * y;

		// Z location:
		stars[i][2] = 70000 * z;
	}
}

//***********************************************************************************
void drawOrbitPaths(){
	//glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glLineWidth(1);
	double angle = 0.0;
	for (int q = 0; q < 9; q++){
		for (int i = 0; i < 360; i++){
			angle = ((double)i * PI) / 180.0f;
			glBegin(GL_LINES);
			glVertex3f(PINFO[q][1] * cos(angle), 0, PINFO[q][1] * PINFO[q][2] * sin(angle));
			angle = ((double)(i + 1) * PI) / 180.0f;
			glVertex3f(PINFO[q][1] * cos(angle), 0, PINFO[q][1] * PINFO[q][2] * sin(angle));
			glEnd();
		}
	}
	//glEnable(GL_LIGHTING);
}

//***********************************************************************************
void drawAsteroid(){
	glColor3f(0.4, 0.35, 0.2);
	glScaled(19, 19, 19);
	glBegin(GL_POLYGON);
	glVertex3f(-0.6, 0.4, 2.35);
	glVertex3f(-0.15, -0.25, 2.35);
	glVertex3f(0.3, -0.3, 2.35);
	glVertex3f(0.4, 0.25, 2.35);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.6, 0.9, 2);
	glVertex3f(-0.6, 0.4, 2.35);
	glVertex3f(0.4, 0.25, 2.35);
	glVertex3f(0.6, 0.7, 1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.4, 0.25, 2.35);
	glVertex3f(0.3, -0.3, 2.35);
	glVertex3f(0.45, -0.55, 1.3);
	glVertex3f(0.6, 0.7, 1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.6, 0.9, 2);
	glVertex3f(-0.5, -0.5, 0.85);
	glVertex3f(-0.15, -0.25, 2.35);
	glVertex3f(-0.6, 0.4, 2.35);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.5, 0.5, -0.15);
	glVertex3f(-0.5, -0.5, -0.15);
	glVertex3f(-0.5, -0.5, 0.85);
	glVertex3f(-0.6, 0.9, 2);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.45, -0.55, 1.3);
	glVertex3f(0.3, -0.3, 2.35);
	glVertex3f(-0.15, -0.25, 2.3);
	glVertex3f(-0.5, -0.5, 0.85);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.6, 0.7, 1.2);
	glVertex3f(0.45, -0.55, 1.3);
	glVertex3f(0.45, -0.45, -0.1);
	glVertex3f(0.45, 0.5, -0.1);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.45, -0.45, -0.1);
	glVertex3f(0.45, -0.55, 1.3);
	glVertex3f(-0.5, -0.5, 0.85);
	glVertex3f(-0.5, -0.5, -0.15);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.6, 0.7, 1.2);
	glVertex3f(0.45, 0.5, -0.1);
	glVertex3f(-0.5, 0.5, -0.15);
	glVertex3f(-0.6, 0.9, 2);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.45, 0.5, -0.1);
	glVertex3f(0.45, -0.45, -0.1);
	glVertex3f(-0.5, -0.5, -0.15);
	glVertex3f(-0.5, 0.5, -0.15);
	glEnd();

}

//***********************************************************************************
void drawLabels(){
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glRasterPos3f(0, SUN_R + 10, 0);
	drawText("Sol");
	for (unsigned int i = 0; i < 9; i++){
		glRasterPos3f(planets[i].curX, PINFO[i][0] + 1, planets[i].curZ);
		for (int j = 0; j < NAMES[i].size(); j++){
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, NAMES[i][j]);
		}
	}

	glPushMatrix();
	glTranslatef(-1500, 3000, 1000);

	for (int i = 0; i < 36; i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, HELP0[i]);
		glTranslatef(1, 0, 0);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef(-1500, 2800, 1000);
	for (int i = 0; i < 18; i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, HELP1[i]);
		glTranslatef(1, 0, 0);
	}

	glPopMatrix();

	glEnable(GL_LIGHTING);
}

//***********************************************************************************
void setUpPlanets(){
	for (int i = 0; i < 9; i++){
		planets[0].curX = 0;
		planets[1].curZ = PINFO[i][1];
		planets[2].pos = 0;
	}
}

//***********************************************************************************
void drawSystem(){
	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set up camera
	if (chase_on){
		eye[0] = planets[chase_p].curX * 1.01;
		eye[1] = PINFO[chase_p][0] + 2;
		eye[2] = planets[chase_p].curZ * 1.01;
		gluLookAt(eye[0], eye[1], eye[2],
			eye[0] + lx, eye[1] + ly, eye[2] + lz,
			tilt[0], tilt[1], tilt[2]
			);
	}
	else{
		gluLookAt(
			eye[0], eye[1], eye[2],
			eye[0] + lx, eye[1] + ly, eye[2] + lz,
			tilt[0], tilt[1], tilt[2]
			);
	}

	// Set light position AFTER setting up camera
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION);

	// Draw the orbit paths of all the planets! 
	if (orbits_on)
		drawOrbitPaths();

	// Draw sun at center
	glColor3f(1, 1, 1);
	glRasterPos3f(-1, SUN_R + 1, 0);
	glMaterialfv(GL_FRONT, GL_EMISSION, SU_COLOR);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(SUN_R, 100, 100);

	// kill the yellow emission value
	glMaterialfv(GL_FRONT, GL_EMISSION, kill);

	// Move out and draw Mercury
	glPushMatrix();
	glTranslatef(planets[0].curX, 0, planets[0].curZ);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(PINFO[0][0], 20, 20);
	glPopMatrix();

	// Move out and draw Venus
	glPushMatrix();
	glTranslatef(planets[1].curX, 0, planets[1].curZ);
	glRotatef(rf*.008, 0, rf*.008, 0);
	glColor3f(0.9, 0.7, 0);
	glutSolidSphere(PINFO[1][0], 30, 30);
	glPopMatrix();

	// Move out and draw Earth
	glPushMatrix();
	glTranslatef(planets[2].curX, 0, planets[2].curZ);
	glRotatef(90, 90, 0, 0);
	glRotatef(rf, rf*0.05, 0, rf*0.95);
	glColor4f(0.0, 0.0, 1.0f, 1.0f);
	glutSolidSphere(PINFO[2][0], 30, 30);
	glColor4f(1, 1, 1, 0.4f);
	glutSolidSphere(PINFO[2][0] + 0.25, 30, 30);

	// Draw the Earth's moon while we're here
	glRotatef(20, 1, 0, 1);
	glTranslatef(-9.0f, 0.0f, 0.0f);
	glColor4f(0.7, 0.7, 0.7, 1.0f);
	glutSolidSphere(MOON_R, 20, 20);

	glPopMatrix();


	// Move out and draw Mars
	glPushMatrix();
	glTranslatef(planets[3].curX, 0, planets[3].curZ);
	glRotatef(rf, 0, rf, 0);
	glColor3f(0.6, 0.1, 0);
	glutSolidSphere(PINFO[3][0], 30, 30);
	glPopMatrix();

	// Move out to the asteroid belt!
	glPushMatrix();
	int num_ast = 50;

	glRotatef(5, 0, 0, 0.2);
	glRotatef(rf*0.05, 0, 1, 0);
	double angle = 0.125664;
	for (int i = 0; i < num_ast; i++){
		glPushMatrix();
		glTranslatef(
			(PINFO[3][1] + PINFO[4][1]) * sin((double)i*angle) / 2,
			0,
			(PINFO[3][1] + PINFO[4][1]) * cos((double)i*angle) / 2
			);
		if ((i % 4) == 0){
			glPushMatrix();
			glRotatef(45, 0, 1, 0.5);
			drawAsteroid();
			glPopMatrix();
		}
		else if ((i % 4) == 1){
			glPushMatrix();
			glRotatef(62, 1, 1, 0);
			drawAsteroid();
			glPopMatrix();
		}
		else{
			drawAsteroid();
			glPopMatrix();
		}
	}
	drawAsteroid();
	glPopMatrix();

	// Move out and draw Jupiter
	glPushMatrix();
	glTranslatef(planets[4].curX, 0, planets[4].curZ);
	glRotatef(rf * 9, 0, rf * 9, 0);
	glColor3f(0.8, 0.7, 0.6);
	glutSolidSphere(PINFO[4][0], 40, 40);
	glPopMatrix();

	// Move out and draw Saturn
	glPushMatrix();
	glTranslatef(planets[5].curX, 0, planets[5].curZ);
	glRotatef(60, 60, 0, 0);
	glColor3f(0.4, 0.25, 0.1);
	gluPartialDisk(quad, PINFO[5][0] + 20, PINFO[5][0] + 30, 50, 5, 0, 360);
	gluPartialDisk(quad, PINFO[5][0] + 35, PINFO[5][0] + 40, 50, 5, 0, 360);
	glRotatef(rf * 8, 0, rf * 8, 0);
	glColor3f(0.8, 0.7, 0.7);
	glutSolidSphere(PINFO[5][0], 50, 50);
	glPopMatrix();

	// Move out and draw Uranus
	glPushMatrix();
	glTranslatef(planets[6].curX, 0, planets[6].curZ);
	glColor3f(.9, .9, .9);
	gluPartialDisk(quad, PINFO[6][0] + 10, PINFO[6][0] + 11, 70, 5, 0, 360);
	glRotatef(rf * 5, 0, 0, rf * 5);
	glColor3f(.7, .7, .7);
	glutSolidSphere(PINFO[6][0], 50, 50);
	glPopMatrix();

	// Move out and draw Neptune
	glPushMatrix();
	glTranslatef(planets[7].curX, 0, planets[7].curZ);
	glRotatef(rf * 5.5, 0, rf * 5.5, 0);
	glColor3f(.5, .6, 1.0);
	glutSolidSphere(PINFO[7][0], 50, 50);
	glPopMatrix();

	// Move out and draw Pluto
	glPushMatrix();
	glTranslatef(planets[8].curX, 0, planets[8].curZ);
	glRotatef(rf*0.16, rf*0.08, 0, rf*0.08);
	glColor3f(1, .8, .8);
	glutSolidSphere(PINFO[8][0], 30, 30);
	glPopMatrix();

	// draw the planet labels (toggle-able)
	if (labels_on){
		drawLabels();
	}

	// draw Stars
	drawStars();

	// Swap buffers
	glutSwapBuffers();
}

void timerEvent(int timer_id){
	if (timer_id == 1){
		if (time_flow){
			rf += 3.0;
			drawSystem();
		}
		glutTimerFunc(50, timerEvent, 1);
	}
	else if (timer_id == 2){
		if (time_flow){
			for (int i = 0; i < 9; i++){
				planets[i].pos += PINFO[i][3];
				planets[i].curX = PINFO[i][1] * sin(planets[i].pos);
				planets[i].curZ = PINFO[i][1] * PINFO[i][2] * cos(planets[i].pos);
			}
			drawSystem();
		}
		glutTimerFunc(19, timerEvent, 2);
	}
}

//***********************************************************************************
void specKeys(int key, int x, int y){
	if (chase_on){
		if (key == GLUT_KEY_UP){
			// go towards the sun
			chase_p -= 1;
			if (chase_p < 0){
				chase_p = 0;
			}
		}
		if (key == GLUT_KEY_DOWN){
			// go out towards pluto
			chase_p += 1;
			if (chase_p > 8){
				chase_p = 8;
			}
			PlaySound(TEXT("C:\\TEMP\\warp.wav"), NULL, SND_FILENAME);
		}
		ly = 0;
		lx = planets[chase_p].curX - (planets[chase_p].curX*1.01);
		lz = planets[chase_p].curZ - (planets[chase_p].curZ*1.01);
	}
	if (key == GLUT_KEY_PAGE_DOWN){
		num_stars -= 100;
		if (num_stars < 0){
			num_stars = 0;
		}

	}
	if (key == GLUT_KEY_PAGE_UP){
		num_stars += 100;
		if (num_stars > 10000){
			num_stars = 10000;
		}

	}
	drawSystem();
}

//***********************************************************************************
void normKeys(unsigned char key, int x, int y){
	if (key == 'w'){
		eye[0] = eye[0] + (lx*SPEED);
		eye[1] = eye[1] + (ly * SPEED);
		eye[2] = eye[2] + (lz*SPEED);
	}

	if (key == 'a'){
		eye[0] -= cos(cam_angleH) * SPEED;
		eye[2] -= sin(cam_angleH) * SPEED;
	}

	if (key == 's'){
		eye[0] = eye[0] - (lx*SPEED);
		eye[1] = eye[1] - (ly * SPEED);
		eye[2] = eye[2] - (lz*SPEED);
	}

	if (key == 'd'){
		eye[0] += cos(cam_angleH) * SPEED;
		eye[2] += sin(cam_angleH) * SPEED;
	}
	if (key == 'f'){
		if (chase_on == true){
			chase_on = false;
		}
		else{
			chase_on = true;
			ly = 0;
			lx = planets[chase_p].curX - (planets[chase_p].curX*1.01);
			lz = planets[chase_p].curZ - (planets[chase_p].curZ*1.01);
		}
	}
	if (key == 't'){
		if (time_flow){
			time_flow = false;
		}
		else{
			time_flow = true;
		}
	}
	if (key == 'l'){
		if (labels_on){
			labels_on = false;
		}
		else{
			labels_on = true;
		}
	}
	if (key == 'o'){
		if (orbits_on){
			orbits_on = false;
		}
		else{
			orbits_on = true;
		}
	}
	drawSystem();
}

//***********************************************************************************
void mouseMove(int x, int y){

	// X movement
	if (x > mouse_x){ // turn right
		cam_angleH += 0.025f;
		lx = sin(cam_angleH);
		lz = -cos(cam_angleH);
	}
	else if (x < mouse_x) { // turn left
		cam_angleH -= 0.025f;
		lx = sin(cam_angleH);
		lz = -cos(cam_angleH);
	}

	// Y movement
	if (y > mouse_y){ // look down 
		cam_angleV -= 0.025f;
		if (cam_angleV < -1.56f){
			cam_angleV = -1.56f;
		}
		ly = sin(cam_angleV);
	}
	else if (y < mouse_y){ // look up
		cam_angleV += 0.025f;
		if (cam_angleV >= 1.56f){
			cam_angleV = 1.56f;
		}
		ly = sin(cam_angleV);
	}
	mouse_x = x;
	mouse_y = y;
	drawSystem();
}

//***********************************************************************************
/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 1000000.0f);
}

//***********************************************************************************
void main(int argc, char ** argv)
{
	glutInit(&argc, argv);

	initializeGlobals();

	helptext.push_back("1. Welcome to Star Simulator!");
	helptext.push_back("Here is how you navigate around:");
	helptext.push_back("WASD controls movement.");
	helptext.push_back("Use the mouse to look around.");
	helptext.push_back("Press 'f' to toggle the Planetary Chase-Cam!");
	helptext.push_back("While in the Chase-Cam, use UP/DOWN arrows to change planets.");
	helptext.push_back("Press 'l' to toggle planet lables.");
	helptext.push_back("Press 'o' to toggle orbit paths.");
	helptext.push_back("Press 't' to stop time (and start it).");
	helptext.push_back("Page Up and Page Down change star visibility.");

	// window setup
	createMainWindow();
	init();

	// set up random numbers
	srand(time(0));
	randomizeStars();

	//callbacks
	glutDisplayFunc(drawSystem);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specKeys);
	glutKeyboardFunc(normKeys);
	glutPassiveMotionFunc(mouseMove);
	glutTimerFunc(1, timerEvent, 1);	// handles rotation
	glutTimerFunc(1, timerEvent, 2);	// handles orbit motion

	// Create help window 
	createHelpWindow();
	glutDisplayFunc(helpDisplay);
	helpInit();

	// setup
	setUpPlanets();					// set up vector of planet locations
	glutWarpPointer(600, 450);					// place mouse in hardcoded window center
	PlaySound(TEXT("C:\\TEMP\\ambbrdg7.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	glutMainLoop();							// get into an infinite loop
}
