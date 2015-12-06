/*==================================================================================================
PROGRAMMER:				Byron Himes
COURSE:					CSC 525/625
MODIFIED BY:			Byron Himes
LAST MODIFIED DATE:		12/06/2015
DESCRIPTION:			Term Project for CSC525 - Computer Graphics

NOTE:					Ensure "ambbrdg7.wav" is located in C:\TEMP

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
#include <string>
#include <vector>
#include <ctime>
#include <GL/glut.h>				// include GLUT library
#include "Constants.h"

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

int help_id = -1;		// help window
int main_id = -1;		// main window
double rf = 135.0;		// base scene rotation factor
double a = 0;			// step interval
double zfactor = 20;	// overall z position of system
double xfactor = 0;		// overall x position of system
double mouse_x = 600;	// Where the mouse was last recorded (for tracking pos change)
double mouse_y = 450;	// ""
double eye[3] = { 100.0, 1000.0, SUN_R + 9700.0 };
double tilt[3] = { 0, 1, 0 };
double lx = 0, lz = -1.0, ly = 0.0; // line of sight variables
double cam_angleH = 0.0;
double cam_angleV = 0.0;
double ship_rotV = 0.0;
double ship_rotH = 0.0;
bool time_flow = true;	// if false, all motion stops
bool labels_on = true;	// if false, planet labels will not appear
bool chase_on = false;	// if on, camera snaps to current chase planet
bool cam_lock = false;
bool cockpit = true;	// toggles cockpit appearance
bool orbits_on = true;	// toggles displaying of orbit paths
bool free_look = false;	// toggles free look within cockpit
int chase_p = 2;		// current planet to chase with camera
Planet planets[9];
GLUquadric* quad = gluNewQuadric(); // for drawing rings
int num_stars = 50000;
int num_sundots = 50000;
float sundots[50000][3] = {};
float stars[50000][3] = {};

vector<string> helptext;	// store messages for HELP WINDOW
vector<string> csctext;		// store computer science messages

//***********************************************************************************
void myInit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);	// Enable depth testing for z-calling
	glDepthFunc(GL_LEQUAL);		// Set the type of depth-test

	// Set up lighting (from the sun);
	glEnable(GL_LIGHTING);		// Enable lighting
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SPECULAR);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DIFFUSE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AMBIENT);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);	// Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

//******************--- Help Window-Related Funcions ---*****************************
void helpInit(){
	//glMatrixMode(GL_PROJECTION);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	gluOrtho2D(-200, 200, -450, 450);
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
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(-294, 415);
	glVertex2f(-294, 265);
	glVertex2f(-294, 265);
	glVertex2f(275, 265);
	glVertex2f(275, 265);
	glVertex2f(275, 415);
	glVertex2f(275, 415);
	glVertex2f(-294, 415);
	glEnd();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}

//*******************--- Miscellaneous Drawing Functions ---*************************
int cscToDisplay(){
	// Returns the index of the proper csc-related message to display
	double distance = sqrt((eye[0] * eye[0]) + (eye[2] * eye[2]));
	for (int i = 0; i < 9; i++){
		if (distance < PINFO[i][1]){
			return i;
		}
	}
	return 9;
}

void drawText(string text_to_write){
	//Text in 3D space example
	for (unsigned int i = 0; i < text_to_write.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text_to_write[i]);
	}
}

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

void drawOrbitPaths(){
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
}

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
	glEnable(GL_LIGHTING);
}

void drawFloatingText(){
	// This function draws the stroke texts that sit out in space
	glDisable(GL_LIGHTING);	// Disable lighting beforehand

	glColor3f(1, 1, 1);	// Set color to white

	// Welcome message:
	glPushMatrix();
	glTranslatef(-1500, 3000, 1000);
	for (int i = 0; i < 44; i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, HELP0[i]);
		glTranslatef(1, 0, 0);
	}
	glPopMatrix();

	// Re-enable lighting when done:
	glEnable(GL_LIGHTING);
}

void drawShip(){
	// Back panel
	glBegin(GL_POLYGON);
	glColor4f(1, 0.75, 0, 0.2);
	glVertex3f(-0.5, 0.55, 1);
	glVertex3f(-0.5, -0.45, 1);
	glVertex3f(0.5, -0.45, 1);
	glVertex3f(0.5, 0.55, 1);
	glEnd();

	// Back Right panel
	glBegin(GL_POLYGON);
	glColor4f(0.45, 0.45, 0.45, 1);
	glVertex3f(0.5, 0.55, 1);
	glColor4f(0.2, 0.2, 0.2, 1);
	glVertex3f(0.5, -0.45, 1);
	glVertex3f(0.5, -0.45, 3.19189e-016);
	glColor4f(0.45, 0.45, 0.45, 1);
	glVertex3f(0.5, 0.55, 3.19189e-016);
	glEnd();

	// Left Upper Strut
	glBegin(GL_POLYGON);
	glColor4f(0.2, 0.2, 0.2, 0.65);
	glVertex3f(-0.5, -0.25, -1.8);
	glVertex3f(-0.5, -0.35, -1.8);
	glColor4f(0.45, 0.45, 0.45, 1);
	glVertex3f(-0.5, 0.45, 3.19189e-016);
	glVertex3f(-0.5, 0.55, 3.19189e-016);
	glEnd();

	// Back left panel
	glBegin(GL_POLYGON);
	glColor4f(0.45, 0.45, 0.45, 1);
	glVertex3f(-0.5, 0.55, 3.19189e-016);
	glColor4f(0.2, 0.2, 0.2, 1);
	glVertex3f(-0.5, -0.45, 3.19189e-016);
	glVertex3f(-0.5, -0.45, 1);
	glColor4f(0.45, 0.45, 0.45, 1);
	glVertex3f(-0.5, 0.55, 1);
	glEnd();

	// Top panel
	glBegin(GL_POLYGON);
	glColor4f(0.5, 0.5, 0.5, 1);
	glVertex3f(0.5, 0.55, 1);
	glVertex3f(0.5, 0.55, 3.19189e-016);
	glVertex3f(-0.5, 0.55, 3.19189e-016);
	glVertex3f(-0.5, 0.55, 1);
	glEnd();

	// Bottom panel
	glBegin(GL_POLYGON);
	glColor4f(0.2, 0.2, 0.2, 1);
	glVertex3f(0.5, -0.45, 3.19189e-016);
	glVertex3f(0.5, -0.45, 1);
	glVertex3f(-0.5, -0.45, 1);
	glVertex3f(-0.5, -0.45, 3.19189e-016);
	glEnd();

	// Right upper strut
	glBegin(GL_POLYGON);
	glColor4f(0.45, 0.45, 0.45, 1);
	glVertex3f(0.5, 0.55, 3.19189e-016);
	glVertex3f(0.5, 0.45, 3.19189e-016);
	glColor4f(0.2, 0.2, 0.2, 0.65);
	glVertex3f(0.5, -0.35, -1.8);
	glVertex3f(0.5, -0.25, -1.8);
	glEnd();

	// Right window
	glBegin(GL_POLYGON);
	glColor4f(1, 0.75, 0, 0.2);
	glVertex3f(0.5, 0.45, 3.19189e-016);
	glVertex3f(0.5, -0.4, 3.19189e-016);
	glVertex3f(0.5, -0.4, -1.8);
	glVertex3f(0.5, -0.35, -1.8);
	glEnd();

	// Left window
	glBegin(GL_POLYGON);
	glColor4f(1, 0.75, 0, 0.2);
	glVertex3f(-0.5, -0.35, -1.8);
	glVertex3f(-0.5, -0.4, -1.8);
	glVertex3f(-0.5, -0.4, 3.19189e-016);
	glVertex3f(-0.5, 0.45, 3.19189e-016);
	glEnd();

	// Floor window
	glBegin(GL_POLYGON);
	glColor4f(1, 0.75, 0, 0.15);
	glVertex3f(0.5, -0.45, -1.8);
	glVertex3f(0.5, -0.45, 3.19189e-016);
	glVertex3f(-0.5, -0.45, 3.19189e-016);
	glVertex3f(-0.5, -0.45, -1.8);
	glEnd();

	// Dash panel
	glBegin(GL_POLYGON);
	glColor4f(0.2, 0.2, 0.2, 0.65);
	glVertex3f(0.5, -0.25, -1.8);
	glVertex3f(0.5, -0.45, -1.8);
	glVertex3f(-0.5, -0.45, -1.8);
	glVertex3f(-0.5, -0.25, -1.8);
	glEnd();

	// Main window
	glBegin(GL_POLYGON);
	glColor4f(1, 0.75, 0, 0);
	glVertex3f(0.5, 0.55, 3.19189e-016);
	glVertex3f(0.5, -0.25, -1.8);
	glVertex3f(-0.5, -0.25, -1.8);
	glVertex3f(-0.5, 0.55, 3.19189e-016);
	glEnd();

	// Left bottom strut
	glBegin(GL_POLYGON);
	glColor4f(0.2, 0.2, 0.2, 0.65);
	glVertex3f(-0.5, -0.4, -1.8);
	glVertex3f(-0.5, -0.45, -1.8);
	glColor4f(0.2, 0.2, 0.2, 1);
	glVertex3f(-0.5, -0.45, 3.19189e-016);
	glVertex3f(-0.5, -0.4, 3.19189e-016);
	glEnd();

	// Right bottom strut
	glBegin(GL_POLYGON);
	glColor4f(0.2, 0.2, 0.2, 1);
	glVertex3f(0.5, -0.4, 3.19189e-016);
	glVertex3f(0.5, -0.45, 3.19189e-016);
	glColor4f(0.2, 0.2, 0.2, 0.65);
	glVertex3f(0.5, -0.45, -1.8);
	glVertex3f(0.5, -0.4, -1.8);
	glEnd();
}

void drawShipPanel(){
	// green dash light
	glDisable(GL_LIGHTING);
	glBegin(GL_POLYGON);
	glColor4f(0, 0.8, 0, 1);
	glVertex3f(-0.4, -0.25, -1.75);
	glColor4f(0, 0.8, 0, 1);
	glVertex3f(-0.4, -0.3, -1.75);
	glColor4f(0, 0.8, 0, 1);
	glVertex3f(-0.3, -0.3, -1.75);
	glColor4f(0, 0.8, 0, 1);
	glVertex3f(-0.3, -0.25, -1.75);
	glEnd();

	// orange-white dash light
	glBegin(GL_POLYGON);
	glColor4f(1, 0.75, 0, 1);
	glVertex3f(0.3, -0.25, -1.75);
	glColor4f(1, 0.75, 0, 1);
	glVertex3f(0.3, -0.3, -1.75);
	glColor4f(1, 1, 1, 1);
	glVertex3f(0.4, -0.3, -1.75);
	glColor4f(1, 1, 1, 1);
	glVertex3f(0.4, -0.25, -1.75);
	glEnd();
	glEnable(GL_LIGHTING);


	glTranslatef(0, -.35, -1.75);

	// Console outline
	glColor4f(0.2, 0.2, 0.2, 1);
	glBegin(GL_POLYGON);
	glVertex3f(-.24, 0.06, -.01);
	glVertex3f(-.24, -0.06, -.01);
	glVertex3f(.24, -0.06, -.01);
	glVertex3f(.24, 0.06, -.01);
	glEnd();

	// Disable lighting for following elements
	glDisable(GL_LIGHTING);

	// Black console
	glColor4f(0, 0, 0, 1);
	glBegin(GL_POLYGON);
	glVertex3f(-.23, 0.05, 0);
	glVertex3f(-.23, -0.05, 0);
	glVertex3f(.23, -0.05, 0);
	glVertex3f(.23, 0.05, 0);
	glEnd();

	// Green console text
	glTranslatef(-0.21, 0, 0.01);
	glPushMatrix();
	glColor4f(0, 1, 0, 1);
	glScalef(.00013, .00013, .00013);
	int msg = cscToDisplay();
	for (unsigned int i = 0; i < csctext[msg].size(); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, csctext[msg][i]);
	}
	glPopMatrix();

	glTranslatef(0.7, 0.21, 0.2);
	glRotatef(90, 0, -1, 0);
	glRotatef(24.5, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(.0007, .0003, .0007);
	string c = "CSC-525";
	for (unsigned int i = 0; i < c.size(); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c[i]);
	}


	glEnable(GL_LIGHTING);
}

//********************--- Functions related to Solar System Setup ---****************
void setUpPlanets(){
	for (int i = 0; i < 9; i++){
		planets[0].curX = 0;
		planets[1].curZ = PINFO[i][1];
		planets[2].pos = 0;
	}
}

void setUpTexts(){
	helptext.push_back("Welcome to Star Simulator!");
	helptext.push_back("Here is how you navigate around:");
	helptext.push_back("WASD controls movement.");
	helptext.push_back("Use the mouse to look around.");
	helptext.push_back("Press 'f' to toggle the Planetary Chase-Cam!");
	helptext.push_back("While in the Chase-Cam, use UP/DOWN arrows to change planets.");
	helptext.push_back("Press 'l' to toggle planet lables.");
	helptext.push_back("Press 'o' to toggle orbit paths.");
	helptext.push_back("Press 't' to stop time (and start it).");
	helptext.push_back("Press 'c' to toggle cockpit.");
	helptext.push_back("Press 'e' to look around the cockpit!");
	helptext.push_back("Page Up and Page Down change star visibility.");
	

	csctext.push_back("Our average starting salaries are stellar!");
	csctext.push_back("Computing offers many opportunites for creativity!");
	csctext.push_back("Demand for programmers has risen astronomically!");
	csctext.push_back("Help shape the future!");
	csctext.push_back("Computing is part of everything we do!");
	csctext.push_back("Career choices are limitless!");
	csctext.push_back("Computing is the future!");
	csctext.push_back("Learn game development!");
	csctext.push_back("Learn from the best computer science department!");
	csctext.push_back("You're really far out here...");
}

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

//********************--- Draw the Solar System (Display Callback) ---***************
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
		if (cam_lock){
			gluLookAt(eye[0], eye[1], eye[2],
				planets[chase_p].curX, 0, planets[chase_p].curZ,
				tilt[0], tilt[1], tilt[2]
			);
		}
		else{
			gluLookAt(eye[0], eye[1], eye[2],
				eye[0] + lx, eye[1] + ly, eye[2] + lz,
				tilt[0], tilt[1], tilt[2]
				);
		}
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

	drawFloatingText();

	drawStars();

	if (cockpit){
		glPushMatrix();
		glTranslatef(eye[0], eye[1], eye[2]);
		glRotatef(ship_rotH, 0, -1, 0);
		glRotatef(ship_rotV, -1, 0, 0);
		drawShip();
		drawShipPanel();
		glPopMatrix();
	}
	
	// Swap buffers and flush
	glutSwapBuffers();
	glFlush();
}

//********************--- Utility Functions ---**************************************
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

void specKeys(int key, int x, int y){
	if (chase_on){
		if (key == GLUT_KEY_UP){
			// go towards the sun
			chase_p -= 1;
			cam_lock = true;
			if (chase_p < 0){
				chase_p = 0;
			}
			PlaySound(TEXT("C:\\beep"), NULL, SND_FILENAME);
		}
		if (key == GLUT_KEY_DOWN){
			// go out towards pluto
			chase_p += 1;
			cam_lock = true;
			if (chase_p > 8){
				chase_p = 8;
			}
			PlaySound(TEXT("C:\\beep"), NULL, SND_FILENAME);
		}
		ly = 0;
		lx = planets[chase_p].curX - (planets[chase_p].curX*1.01);
		lz = planets[chase_p].curZ - (planets[chase_p].curZ*1.01);
	}
	if (key == GLUT_KEY_PAGE_DOWN){
		num_stars -= 500;
		if (num_stars < 0){
			num_stars = 0;
		}

	}
	if (key == GLUT_KEY_PAGE_UP){
		num_stars += 500;
		if (num_stars > 50000){
			num_stars = 50000;
		}

	}
	drawSystem();
}

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
			cam_lock = false;
		}
		else{
			chase_on = true;
			cam_lock = true;
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
	if (key == 'c'){
		if (cockpit){
			cockpit = false;
		}
		else{
			cockpit = true;
		}
	}
	if (key == 'e'){
		if (free_look){
			free_look = false;
		}
		else{
			free_look = true;
			cockpit = true;	// turn on cockpit with freelook
		}
	}
	drawSystem();
}

void mouseMove(int x, int y){
	if (cam_lock){
		ly = 0;
		lx = planets[chase_p].curX - (planets[chase_p].curX*1.01);
		lz = planets[chase_p].curZ - (planets[chase_p].curZ*1.01);
		cam_lock = false;
	}
	
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
	
	// update cockpit rotation information
	if (!free_look){
		ship_rotH = ((cam_angleH)* 180) / PI;
		ship_rotV = -((cam_angleV)* 180) / PI;
		if (ship_rotV > 60){
			ship_rotV = 60;
		}
		else if (ship_rotV < -60){
			ship_rotV = -60;
		}
	}

	// Update mouse information
	mouse_x = x;
	mouse_y = y;
	drawSystem();
}

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

//********************--- Main ---***************************************************
void main(int argc, char ** argv)
{
	glutInit(&argc, argv);

	setUpTexts();					// Initialize in-game text vectors
	setUpPlanets();					// set up vector of planet locations

	// window setup
	glutInitWindowSize(1200, 900);					// specify a window size
	glutInitWindowPosition(100, 0);				// specify a window position
	main_id = glutCreateWindow("Solar System!");	// create a titled window
	glutInitDisplayMode(GLUT_DOUBLE);
	myInit();

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
	glutInitWindowSize(600, 900);
	glutInitWindowPosition(1315, 31);
	help_id = glutCreateWindow("Help");
	glutDisplayFunc(helpDisplay);
	helpInit();
	glutSetWindow(main_id);
		
	glutWarpPointer(600, 450);					// place mouse in hardcoded window center
	PlaySound(TEXT("C:\\TEMP\\masseffect.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	glutMainLoop();							// get into an infinite loop
}
