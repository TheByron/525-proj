#pragma once

/*==================================================================================
PROGRAMMED BY:		Cameron Weston
UPDATED LAST:		12-06-2015
FILE NAME:			Constants.h
DESCRIPTION:		Holds some important constants, arrays, etc. plus a struct. 
					To be used with officialProject.cpp for the CSC-525 term project
==================================================================================*/

#include <string>

// This is a struct for planet position information
typedef struct Planet{
	double curX;
	double curZ;
	double pos;
} Planet;

// Some important constants
const double SUN_R = 695;	// sun's radius
const double MOON_R = 1.73;	// moon's radius
const double PI = 3.141592653589793238;	// Just Pi

/* subarray format for PINFO (planet info): 
0: radius
1: distance from sun
2: orbit eccentricity coefficient
3: orbit position increment (radians)*/
const double PINFO[9][4]{
	{ 2.44, SUN_R + 387, 1.2056, 0.01434515 },	// mercury
	{ 6.05, SUN_R + 723, 1.0068, 0.00555297 },	// venus
	{ 6.38, SUN_R + 1000, 1.0167, 0.00344284 },	// earth
	{ 3.40, SUN_R + 1524, 1.0934, 0.00183130 },	// mars
	{ 71.5, SUN_R + 5203, 1.0483, 0.00029029 },	// jupiter
	{ 60.3, SUN_R + 9537, 1.0560, 0.00011687 },	// saturn
	{ 25.6, SUN_R + 19191, 1.0461, 0.00004098 },// uranus
	{ 24.8, SUN_R + 30069, 1.0097, 0.00002089 },// neptune
	{ 1.15, SUN_R + 39482, 1.2482, 0.0000014}	// pluto
};

// Miscellaneous materials
GLfloat SU_COLOR[] = { 1.0, 1.0, 0, 1.0f };		// sun needs emission component
GLfloat kill[] = { 0.0f, 0.0f, 0.0f, 0.0f };	// so everything isn't yellow

// Movement speed for player
const double SPEED = 10;

// Light source values
GLfloat SPECULAR[4] = {1.0, 1.0, 1.0, 1.0f};
GLfloat DIFFUSE[4] = {0.9f, 0.9f, 0.9f, 1.0f};
GLfloat AMBIENT[4] = { 0.1, 0.1, 0.1, 1.0f };
GLfloat LIGHT_POSITION[4] = {0, 0, 0, 1};

// Used for the stroke text seen above the sun
char HELP0[] = "WELCOME TO THE MISSOURI STATE SOLAR SYSTEM!";
