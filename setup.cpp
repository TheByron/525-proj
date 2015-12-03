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