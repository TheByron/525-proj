/*=======================================================================================
PROGRAMMERS:			Byron Himes, Cameron Weston, Kendall Lewis
WORKLOAD BREAKDOWN:		Byron Himes - Supreme Leader (33%)
							- All calculations for motion 
							- Space ship design
							- drawSystem()
							- Super rad polygon positioning program (SRPPP)
							- Figured out 3D

						Cameron Weston (33%)
							- Designed and implemented Constants.h
							- Designed and implemented help window (and text)
							- Asteroid design via SRPPP
							- Master of star planning committee
							- Found Mass Effect music (no credit for music itself)
						
						Kendall Lewis (33%)
							- Designed and implemented input callbacks
							- Got the lighting going
							- Researched and implemented Comp Sci texts
							- Countless hours of bug-hunting

						Space Ghost (1%)
							- Split his contribution into three perfect and equal pieces
							  and then donated them to Byron, Cameron, and Kendall. 
							  What a guy.

COURSE:					CSC 525/625
MODIFIED BY:			Byron Himes
LAST MODIFIED DATE:		12/06/2015
DESCRIPTION:			Term Project for CSC525 - Computer Graphics...
						Pilot a ship around the local solar system!

NOTE:					Ensure "masseffect.wav" is located in C:\TEMP

FILES:					officialProject.cpp, Constants.h, (project.sln, ...)
IDE/COMPILER:			MicroSoft Visual Studio 2013
INSTRUCTION FOR COMPILATION AND EXECUTION:
1.		Double click on project.sln	to OPEN the project
2.		Press Ctrl+F7					to COMPILE
3.		Press Ctrl+Shift+B				to BUILD (COMPILE+LINK)
4.		Press Ctrl+F5					to EXECUTE
=======================================================================================*/
#include <iostream>
#include <Windows.h>				// Needed for sound
#include <string>
#include <vector>
#include <ctime>
#include <GL/glut.h>				// include GLUT library
#include "Constants.h"

using namespace std;

// Initialization of global variables
string NAMES[9] = {		// Array of planet names
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
double eye[3] = { 100.0, 1000.0, SUN_R + 9700.0 };	// Camera location array (VRP)
double tilt[3] = { 0, 1, 0 };		// Camera tilt array (VUP)
double lx = 0, lz = -1.0, ly = 0.0; // line of sight variables
double cam_angleH = 0.0;			// Tracks horizontal orientation
double cam_angleV = 0.0;			// Tracks vertical orientation
double ship_rotV = 0.0;				// Tracks vertical orientation of cockpit
double ship_rotH = 0.0;				// Tracks horizontal orientation of cockpit
bool time_flow = true;	// if false, all motion stops
bool labels_on = true;	// if false, planet labels will not appear
bool chase_on = false;	// if on, camera snaps to current chase planet
bool cam_lock = false;	// it on, camera looks at current planet until mouse is moved
bool cockpit = true;	// toggles cockpit appearance
bool orbits_on = true;	// toggles displaying of orbit paths
bool free_look = false;	// toggles free look within cockpit
int chase_p = 2;		// current planet to chase with camera
Planet planets[9];		// Array for planet location information
GLUquadric* quad = gluNewQuadric(); // for drawing rings
int num_stars = 50000;				// Number of stars to draw
float stars[50000][3] = {};			// Array containing star positions

vector<string> helptext;	// store messages for HELP WINDOW
vector<string> csctext;		// store computer science messages

//***********************************************************************************
void myInit()
{	// Initialization function, sets up desired graphical settings 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// Set background color to black and opaque
	glClearDepth(1.0f);						// Set background depth to farthest
	glEnable(GL_COLOR_MATERIAL);			// Enable colors as materials
	glEnable(GL_DEPTH_TEST);				// Enable depth testing for z-calling
	glDepthFunc(GL_LEQUAL);					// Set the type of depth-test

	// Set up lighting (from the sun);
	glEnable(GL_LIGHTING);			// Enable lighting
	glEnable(GL_LIGHT0);			// Enable primary light source
	glLightfv(GL_LIGHT0, GL_SPECULAR, SPECULAR);	// Set specular setting
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DIFFUSE);		// Set diffuse
	glLightfv(GL_LIGHT0, GL_AMBIENT, AMBIENT);		// Set ambient
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Turn on blending for semi-transparent polygons
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);	// Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

//******************--- Help Window-Related Funcions ---*****************************
void helpInit(){
	// Initialize help window settings
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
	// Display callback for the help window
	glClear(GL_COLOR_BUFFER_BIT);	// draw the background
	glMatrixMode(GL_PROJECTION);	// switch to projection matrix
	glPushMatrix();					// push on another matrix 
	gluOrtho2D(-300, 300, -450, 450);	// switch to 2D 

	glMatrixMode(GL_MODELVIEW);		// back to modelview
	glPushMatrix();					// push on something
	glLoadIdentity();

	glDisable(GL_LIGHTING);			// turn off lighting so text isn't affected
	helptextdraw();					// Draw the help window text
	glLineWidth(1);					// Draw the white box around the text
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

	glPopMatrix();					// pop off that modelview matrix

	glMatrixMode(GL_PROJECTION);	// back to projection matrix stack again
	glPopMatrix();					// pop off that matrix
	glEnable(GL_LIGHTING);			// turn lighting back on
	glutSwapBuffers();
}

//*******************--- Miscellaneous Drawing Functions ---*************************
int cscToDisplay(){
	// Returns the index of the proper csc-related message to display

	// Get distance from sun in x-z plane
	double distance = sqrt((eye[0] * eye[0]) + (eye[2] * eye[2]));

	// return index based roughly on which two bodies you are between 
	for (int i = 0; i < 9; i++){
		if (distance < PINFO[i][1]){
			return i;
		}
	}
	return 9;
}

void drawText(string text_to_write){
	// Function for drawing text
	for (unsigned int i = 0; i < text_to_write.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text_to_write[i]);
	}
}

void drawStars(){
	// This function draws all of the stars (not the sun)
	glDisable(GL_LIGHTING);	// turn off lighting so they aren't affected/dimmed
	glPointSize(1);			// set point size to 1 so they seem far away

	glBegin(GL_POINTS);
	for (int i = 0; i < num_stars; i++){
		float blueness = (rand() % 10)*0.1;	// randomize amount of blue/white
		glColor3f(blueness, blueness, 1);
		glVertex3f(							// draw star based on coords in array
			stars[i][0],
			stars[i][1],
			stars[i][2]
			);
	}
	glEnd();
	glEnable(GL_LIGHTING);	// re-enable lighting
}

void drawOrbitPaths(){
	// This function draws all of the orbit path lines
	glColor3f(1, 1, 1);		// change color to white
	glLineWidth(1);			// change line width to 1
	double angle = 0.0;		// angle (will be in radians)
	for (int q = 0; q < 9; q++){	// for all nine planets...
		for (int i = 0; i < 360; i++){	// for every section of 1 degree in length
			angle = ((double)i * PI) / 180.0f;	// convert the degree to radians
			// draw a line segment for that section
			glBegin(GL_LINES);
			glVertex3f(PINFO[q][1] * cos(angle), 0, PINFO[q][1] * PINFO[q][2] * sin(angle));
			angle = ((double)(i + 1) * PI) / 180.0f;
			glVertex3f(PINFO[q][1] * cos(angle), 0, PINFO[q][1] * PINFO[q][2] * sin(angle));
			glEnd();
		}
	}
}

void drawAsteroid(){
	// This function draws an asteroid (they all look the same!)
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
	// This function draws the celestial body labels
	glDisable(GL_LIGHTING);		// disable lighting
	glColor3f(1, 1, 1);			// change color to white
	glRasterPos3f(0, SUN_R + 10, 0);	// set text position for the sun's label
	drawText("Sol");			// Draw sun's label
	for (unsigned int i = 0; i < 9; i++){	// draw the rest of the planets' labels
		// Set label position:
		glRasterPos3f(planets[i].curX, PINFO[i][0] + 1, planets[i].curZ);
		// Draw the text
		for (int j = 0; j < NAMES[i].size(); j++){
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, NAMES[i][j]);
		}
	}
	glEnable(GL_LIGHTING);		// re-enable lighting
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
	// This function draws the main part of your ship
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
	// This function draws all of the little cockpit details
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

	// move to the dash
	glTranslatef(0, -.35, -1.75);

	// Console outline
	glColor4f(0.2, 0.2, 0.2, 1);
	glBegin(GL_POLYGON);
	glVertex3f(-.24, 0.06, -.01);
	glVertex3f(-.24, -0.06, -.01);
	glVertex3f(.24, -0.06, -.01);
	glVertex3f(.24, 0.06, -.01);
	glEnd();

	// Disable lighting for following elements since they remain lit
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
	glTranslatef(-0.21, 0, 0.01);	// Position the text
	glPushMatrix();				// push on another matrix
	glColor4f(0, 1, 0, 1);		// set color to green
	glScalef(.00013, .00013, .00013);	// scale down stroke text
	int msg = cscToDisplay();	// get the proper message to show
	// Draw the message:
	for (unsigned int i = 0; i < csctext[msg].size(); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, csctext[msg][i]);
	}
	glPopMatrix();				// pop off that matrix

	// Angled window text (CSC-525)
	glTranslatef(0.7, 0.21, 0.2);	// Move into position
	glRotatef(90, 0, -1, 0);		// Rotate along y-axis
	glRotatef(24.5, 0, 0, 1);		// rotate along z-axis from there
	glRotatef(90, 1, 0, 0);			// rotate along x-axis from there
	glScalef(.0007, .0003, .0007);	// Scale as needed
	string c = "CSC-525";			// Text we're going to show
	// Draw the text:
	for (unsigned int i = 0; i < c.size(); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c[i]);
	}
	
	glEnable(GL_LIGHTING);	// Don't forget to re-enable lighting
}

//********************--- Functions related to Solar System Setup ---****************
void setUpPlanets(){
	// Initialize planet location array
	for (int i = 0; i < 9; i++){
		planets[0].curX = 0;
		planets[1].curZ = PINFO[i][1];
		planets[2].pos = 0;
	}
}

void setUpTexts(){
	// Set up the two main text vectors with the appropriate messages
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
	// Randomize the positions of the stars (different every time!)
	for (int i = 0; i < num_stars; i++){
		double x, y, z;
		// randomize coord coefficients between -1.000 and 1.000
		do{
			x = ((rand() % 2001) / 1000.0f) - 1;	
			y = ((rand() % 2001) / 1000.0f) - 1;
			z = ((rand() % 2001) / 1000.0f) - 1;
		} while ((x*x) + (y*y) + (z*z) > 1);	// make sure they're in the right range
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
	// * This function does all of the heavy lifting and is the main display function

	// Clear color and depth buffers, switch to modelview matrix and load id matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set up camera
	if (chase_on){		// if chase mode
		// move camera to just outside planet orbit
		eye[0] = planets[chase_p].curX * 1.01;
		eye[1] = PINFO[chase_p][0] + 2;
		eye[2] = planets[chase_p].curZ * 1.01;
		if (cam_lock){	// if the player has yet to move mouse after locking to planet
			gluLookAt(eye[0], eye[1], eye[2],
				planets[chase_p].curX, 0, planets[chase_p].curZ,
				tilt[0], tilt[1], tilt[2]
			);
		}
		else{	// once the player has moved the mouse after locking to the planet
			gluLookAt(eye[0], eye[1], eye[2],
				eye[0] + lx, eye[1] + ly, eye[2] + lz,
				tilt[0], tilt[1], tilt[2]
				);
		}
	}
	else{	// camera set up here if not in chase mode
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
	glMaterialfv(GL_FRONT, GL_EMISSION, SU_COLOR);	// set yellow emission value
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(SUN_R, 100, 100);	// draw sun's sphere
	

	// kill the yellow emission value
	glMaterialfv(GL_FRONT, GL_EMISSION, kill);

	// Move out and draw Mercury
	glPushMatrix();		// push on another matrix
	glTranslatef(planets[0].curX, 0, planets[0].curZ);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(PINFO[0][0], 20, 20);
	glPopMatrix();		// pop off that matrix

	// Move out and draw Venus
	glPushMatrix();		// push on another matrix
	glTranslatef(planets[1].curX, 0, planets[1].curZ);
	glRotatef(rf*.008, 0, rf*.008, 0);
	glColor3f(0.9, 0.7, 0);
	glutSolidSphere(PINFO[1][0], 30, 30);
	glPopMatrix();		// pop off that matrix

	// Move out and draw Earth
	glPushMatrix();		// push on another matrix
	glTranslatef(planets[2].curX, 0, planets[2].curZ);
	glRotatef(90, 90, 0, 0);
	glRotatef(rf, rf*0.05, 0, rf*0.95);
	glColor4f(0.0, 0.0, 1.0f, 1.0f);
	glutSolidSphere(PINFO[2][0], 30, 30);
	// Draw Earth's atmosphere
	glColor4f(1, 1, 1, 0.4f);
	glutSolidSphere(PINFO[2][0] + 0.25, 30, 30);

	// Draw the Earth's moon while we're here
	glRotatef(20, 1, 0, 1);
	glTranslatef(-9.0f, 0.0f, 0.0f);
	glColor4f(0.7, 0.7, 0.7, 1.0f);
	glutSolidSphere(MOON_R, 20, 20);

	glPopMatrix();	// pop off earth's temp matrix


	// Move out and draw Mars
	glPushMatrix();		// push on another matrix
	glTranslatef(planets[3].curX, 0, planets[3].curZ);
	glRotatef(rf, 0, rf, 0);
	glColor3f(0.6, 0.1, 0);
	glutSolidSphere(PINFO[3][0], 30, 30);
	glPopMatrix();		// pop off that matrix

	// Move out to the asteroid belt!
	glPushMatrix();		// push on another matrix
	int num_ast = 50;	// draw 50 asteroids

	// offset the asteroid belt to make it look cool
	glRotatef(5, 0, 0, 0.2);
	glRotatef(rf*0.05, 0, 1, 0);	// rotate belt around y axis (spin it)
	double angle = 0.125664;		// angle of increment for each asteroid
	for (int i = 0; i < num_ast; i++){
		glPushMatrix();
		glTranslatef(	// move to halfway between mars and earth
			(PINFO[3][1] + PINFO[4][1]) * sin((double)i*angle) / 2,
			0,
			(PINFO[3][1] + PINFO[4][1]) * cos((double)i*angle) / 2
			);
		if ((i % 4) == 0){		// some asteroids are rotated differently
			glPushMatrix();				// push on a matrix
			glRotatef(45, 0, 1, 0.5);	// rotate
			drawAsteroid();				// draw it
			glPopMatrix();				// pop off that matrix
		}
		else if ((i % 4) == 1){	// some asteroids are rotated differently
			glPushMatrix();				// push on a matrix
			glRotatef(62, 1, 1, 0);		// rotate the asteroid by 62 degrees
			drawAsteroid();				// draw it
			glPopMatrix();				// pop off that matrix
		}
		else{				// otherwise draw the asteroid as-is
			drawAsteroid();
			glPopMatrix();	// pop off that matrix
		}
	}
	glPopMatrix();	// done with asteroid belt's temp matrix, pop it

	// Move out and draw Jupiter
	glPushMatrix();			// push on a matrix
	glTranslatef(planets[4].curX, 0, planets[4].curZ);
	glRotatef(rf * 9, 0, rf * 9, 0);	// rotate jupiter accordingly
	glColor3f(0.8, 0.7, 0.6);			// color it
	glutSolidSphere(PINFO[4][0], 40, 40);	// draw the planet
	glPopMatrix();			// pop off that matrix

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
	glPopMatrix();				// pop off that matrix

	// Move out and draw Uranus
	glPushMatrix();
	glTranslatef(planets[6].curX, 0, planets[6].curZ);
	glColor3f(.9, .9, .9);
	gluPartialDisk(quad, PINFO[6][0] + 10, PINFO[6][0] + 11, 70, 5, 0, 360);
	glRotatef(rf * 5, 0, 0, rf * 5);
	glColor3f(.7, .7, .7);
	glutSolidSphere(PINFO[6][0], 50, 50);
	glPopMatrix();				// pop off that matrix

	// Move out and draw Neptune
	glPushMatrix();
	glTranslatef(planets[7].curX, 0, planets[7].curZ);
	glRotatef(rf * 5.5, 0, rf * 5.5, 0);
	glColor3f(.5, .6, 1.0);
	glutSolidSphere(PINFO[7][0], 50, 50);
	glPopMatrix();				// pop off that matrix

	// Move out and draw Pluto
	glPushMatrix();
	glTranslatef(planets[8].curX, 0, planets[8].curZ);
	glRotatef(rf*0.16, rf*0.08, 0, rf*0.08);
	glColor3f(1, .8, .8);
	glutSolidSphere(PINFO[8][0], 30, 30);
	glPopMatrix();				// pop off that matrix

	// draw the planet labels (toggle-able)
	if (labels_on){
		drawLabels();
	}

	drawFloatingText();		// draw the stroke text above the sun

	drawStars();			// draw the stars

	if (cockpit){			// draw the cockpit if it's enabled
		glPushMatrix();		// push on a matrix
		glTranslatef(eye[0], eye[1], eye[2]);	// move to camera location
		glRotatef(ship_rotH, 0, -1, 0);		// rotate horizontally
		glRotatef(ship_rotV, -1, 0, 0);		// rotate vertically
		drawShip();							// draw the ship
		drawShipPanel();					// draw the ship dash/details
		glPopMatrix();						// pop off that matrix
	}
	
	// flush
	glFlush();
}

//********************--- Utility Functions ---**************************************
void timerEvent(int timer_id){
	// This callback function handles the timer events
	// Timer 1 controls master universal ultra mega overlord rotation
	if (timer_id == 1){
		if (time_flow){
			rf += 3.0;
			drawSystem();
		}
		glutTimerFunc(50, timerEvent, 1);
	}
	// Timer 2 controls the updating of planet locations
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
	// This function handles all special key presses
	if (chase_on){	// The following keys only work if in chase-cam mode
		if (key == GLUT_KEY_UP){
			// go towards the sun
			chase_p -= 1;
			cam_lock = true;	// lock the camera
			if (chase_p < 0){
				chase_p = 0;
			}
			PlaySound(TEXT("C:\\beep"), NULL, SND_FILENAME);	// play beep
		}
		if (key == GLUT_KEY_DOWN){
			// go out towards pluto
			chase_p += 1;
			cam_lock = true;	// lock the camera
			if (chase_p > 8){
				chase_p = 8;
			}
			PlaySound(TEXT("C:\\beep"), NULL, SND_FILENAME);	// play beep
		}
		// also direct view to the currently chased planet
		ly = 0;
		lx = planets[chase_p].curX - (planets[chase_p].curX*1.01);
		lz = planets[chase_p].curZ - (planets[chase_p].curZ*1.01);
	}

	// These keys always work:
	if (key == GLUT_KEY_PAGE_DOWN){		// hold to decrease number of stars
		num_stars -= 500;
		if (num_stars < 0){
			num_stars = 0;
		}

	}
	if (key == GLUT_KEY_PAGE_UP){		// hold to increase number of stars
		num_stars += 500;
		if (num_stars > 50000){
			num_stars = 50000;
		}

	}
	drawSystem();		// redraw the system
}

void normKeys(unsigned char key, int x, int y){
	// This function handles 'normal' key presses
	if (key == 'w'){	// move forward
		eye[0] = eye[0] + (lx*SPEED);
		eye[1] = eye[1] + (ly * SPEED);
		eye[2] = eye[2] + (lz*SPEED);
	}

	if (key == 'a'){	// move left
		eye[0] -= cos(cam_angleH) * SPEED;
		eye[2] -= sin(cam_angleH) * SPEED;
	}

	if (key == 's'){	// move backward
		eye[0] = eye[0] - (lx*SPEED);
		eye[1] = eye[1] - (ly * SPEED);
		eye[2] = eye[2] - (lz*SPEED);
	}

	if (key == 'd'){	// move right
		eye[0] += cos(cam_angleH) * SPEED;
		eye[2] += sin(cam_angleH) * SPEED;
	}
	if (key == 'f'){	// toggle chase cam
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
	if (key == 't'){	// toggle time-flow
		if (time_flow){
			time_flow = false;
		}
		else{
			time_flow = true;
		}
	}
	if (key == 'l'){	// toggle labels showing or no
		if (labels_on){
			labels_on = false;
		}
		else{
			labels_on = true;
		}
	}
	if (key == 'o'){	// toggle orbit path drawing
		if (orbits_on){
			orbits_on = false;
		}
		else{
			orbits_on = true;
		}
	}
	if (key == 'c'){	// toggle drawing of cockpit
		if (cockpit){
			cockpit = false;
		}
		else{
			cockpit = true;
		}
	}
	if (key == 'e'){	// toggle freelook mode (look around cockpit)
		if (free_look){
			free_look = false;
		}
		else{
			free_look = true;
			cockpit = true;	// turn on cockpit with freelook
		}
	}
	drawSystem();	// redraw the system
}

void mouseMove(int x, int y){
	// This function handles the mouse input

	// If camera lock is enabled, disable it so you can look around 
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

		// cap vertical rotation capabilities of ship
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
	drawSystem();	// redraw the system
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;                // To prevent 0 division
	GLfloat aspect = (GLfloat)width / (GLfloat)height;	// get aspect ratio

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 1000000.0f);	// reset perspective
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
	glutDisplayFunc(drawSystem);		// display callback
	glutReshapeFunc(reshape);			// window resize callback
	glutSpecialFunc(specKeys);			// special keypress callback
	glutKeyboardFunc(normKeys);			// normal keypress callback
	glutPassiveMotionFunc(mouseMove);	// mouse movement callback
	glutTimerFunc(1, timerEvent, 1);	// handles rotation
	glutTimerFunc(1, timerEvent, 2);	// handles orbit motion

	// Create help window 
	glutInitWindowSize(600, 900);		// set help window size
	glutInitWindowPosition(1315, 31);	// set help window position
	help_id = glutCreateWindow("Help");	// set help window id
	glutDisplayFunc(helpDisplay);		// help window display callback
	helpInit();							// initialize help window
	glutSetWindow(main_id);				// set window to main again
		
	glutWarpPointer(600, 450);					// place mouse in hardcoded window center

	// play really cool song from BioWare/EA's Mass Effect
	PlaySound(TEXT("C:\\TEMP\\masseffect.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	glutMainLoop();							// get into an infinite loop
}
