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
double eye[3] = { 100.0, 0.0, SUN_R + 9700.0 };	// location of camera
double tilt[3] = { 0, 1, 0 };					// camera tilt
double lx = 0, lz = -1.0, ly = 0.0;				// line of sight variables
double cam_angleH = 0.0;						// Horizontal orientation (rads)
double cam_angleV = 0.0;						// Vertical orientation	(rads)
double SPEED = 5;		// Movement speed for player

