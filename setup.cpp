#include "setup.h"
void init()
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

void createMainWindow(){
	// window setup
	glutInitWindowSize(1200, 900);					// specify a window size
	glutInitWindowPosition(100, 0);				// specify a window position
	main_id = glutCreateWindow("Solar System!");	// create a titled window
	glutInitDisplayMode(GLUT_DOUBLE);
}

void createHelpWindow(){
	// Create help window 
	glutInitWindowSize(600, 900);
	glutInitWindowPosition(1315, 31);
	help_id = glutCreateWindow("Help");
}

void initializeGlobals(){
	bool time_flow = true;	// if false, all motion stops
	bool labels_on = true;	// if false, planet labels will not appear
	bool chase_on = false;	// if on, camera snaps to current chase planet
	bool orbits_on = true;	// toggles displaying of orbit paths

	int chase_p = 2;		// current planet to chase with camera
	int num_stars = 10000;	// Number of stars to draw
	int help_id = -1;		// help window id
	int main_id = -1;		// main window id

	double rf = 135.0;		// base scene rotation factor
	double a = 0;			// step interval
	double zfactor = 20;	// overall z position of system
	double xfactor = 0;		// overall x position of system
	double mouse_x = 600;	// Where the mouse was last recorded (for tracking pos change)
	double mouse_y = 450;	// ""

	double eye[3] = { 100.0 , 0.0, SUN_R + 9700.0};	// location of camera x
	double tilt[3] = { 0, 1, 0 };			// camera tilt
	double lx = 0, lz = -1.0, ly = 0.0;			// line of sight variables
	double cam_angleH = 0.0;						// Horizontal orientation (rads)
	double cam_angleV = 0.0;						// Vertical orientation	(rads)
	double SPEED = 10;		// Movement speed for player
}