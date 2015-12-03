#include <string>

typedef struct Planet{
	double curX;
	double curZ;
	double pos;
} Planet;

// Some important constants
const double SUN_R = 695;
const double MOON_R = 1.73;
const double PI = 3.141592653589793238;

/* subarray format: 
0: radius
1: distance from sun
2: orbit eccentricity
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
GLfloat SU_COLOR[] = { 1.0, 1.0, 0, 1.0f };
GLfloat kill[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat SAR_COLOR[] = { 0.4, 0.25, 0.1, 1.0f };
GLfloat URR_COLOR[] = { .9, .9, .9, 1.0f };
GLfloat TEXT_COLOR[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Material Properties for planets
const GLfloat MATS[9][4] = {
	{ 1.0, 0, 0, 1.0f },	// mercury
	{ 0.9, 0.7, 0, 1.0f },	// venus
	{ 0.0, 0.0, 1.0, 1.0f },// earth
	{ 0.6, 0.1, 0, 1.0f },	// mars
	{ 0.8, 0.7, 0.6, 1.0f },// jupiter
	{ 0.8, 0.7, 0.7, 1.0f },// saturn
	{ .7, .7, .7, 1.0f },	// uranus
	{ .5, .6, 1.0, 1.0f },	// neptune
	{ 1, .8, .8, 1.0f }		// pluto
};


// Movement speed for player
const double SPEED = 5;

// Light source values
GLfloat SPECULAR[4] = {1.0, 1.0, 1.0, 1.0f};
GLfloat DIFFUSE[4] = {0.9f, 0.9f, 0.9f, 1.0f};
GLfloat AMBIENT[4] = { 0.1, 0.1, 0.1, 1.0f };
GLfloat LIGHT_POSITION[4] = {0, 0, 0, 1};

char HELP0[] = "WELCOME TO THE CSC525 SOLAR SYSTEM!";
char HELP1[] = "PRESS F1 FOR HELP!";

char CSC0[] = "GET A DEGREE THAT'S REALLY USEFUL!";
char CSC1[] = "STUDY COMPUTER SCIENCE AT MISSOURI STATE!";
char CSC2[] = "GET PAID $$$$ TO SOLVE LOGIC PUZZLES ALL DAY!";
char CSC3[] = "PROGRAMMING IS OUT OF THIS WORLD";
