#include <iostream>
#include <cmath>
#include <vector>
#include "paint.cpp"

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using namespace std;

float xOffset = 0.3;
float yOffset = 0.2;
float radius = 0.5;
bool dragging = true;

// colorX and colorY are used for the coordinate positions of the color selection boxes; colorGap is for gap between boxes
float colorX = -0.92;
float colorY = 0.92;
float toolY = -0.03;
float colorGap = 0.14;
// Background colors (for eraser)
float bgRED = 0.85;
float bgGREEN = 0.85;
float bgBLUE = 0.85;
// RGB colors used for painting (modified by clicking in boxes)
float RED = 0.0;
float GREEN = 0.0;
float BLUE = 0.0;

// Store the width and height of the window
int width = 640, height = 640;

point Dot;
vector<Rect> box;																		// For the palette selection box
vector<colors> cBox;																	// For color select tool boxes
vector<point> eraser;
vector<point> dots;



//-------------------------------------------------------
// A function to draw the scene
//-------------------------------------------------------
void appDrawScene() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set background color to light gray
	glClearColor(0.7, 0.7, 0.7, 1.0);

	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i = dots.size() - 1; i >= 0; i--) {
		dots[i].drawDot();
	}
	// Create the color palette
	for (int i = 0; i < cBox.size(); i++) {
		cBox[i].colorDraw();
	}
	// Create the drawable space
	for (int i = 0; i < box.size(); i++) {
		box[i].draw();
	}


	/* We have been drawing everything to the back buffer
	   Swap the buffers to see the result of what we drew */
	glFlush();
	glutSwapBuffers();
}

//-------------------------------------------------------
// A function to convert window coordinates to scene
// We use it when a mouse event is handled
// Arguments: 	
//	x, y - the coordinates to be updated
//-------------------------------------------------------
void windowToScene(float& x, float& y) {
	x = (2.0f*(x / float(width))) - 1.0f;
	y = 1.0f - (2.0f*(y / float(height)));
}

//-------------------------------------------------------
// A function to handle window resizing
// Called every time the window is resized
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appReshapeFunc(int w, int h) {
	// Window size has changed
	width = w;
	height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range
	const double appXmin = -1.0;
	const double appXmax = 1.0;
	const double appYmin = -1.0;
	const double appYmax = 1.0;

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disered ones
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = (appXmax + appXmin) / 2;
		winXmin = center - (center - appXmin)*scale;
		winXmax = center + (appXmax - center)*scale;
		winYmin = appYmin;
		winYmax = appYmax;
	}
	else {
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = (appYmax + appYmin) / 2;
		winYmin = center - (center - appYmin)*scale;
		winYmax = center + (appYmax - center)*scale;
		winXmin = appXmin;
		winXmax = appXmax;
	}

	// Now we use glOrtho to set up our viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}

bool insideCircle(float xOrigin, float yOrigin, float radius, float mx, float my) {
	// Check to see if distance from origin to mouse <= radius
	return sqrt(pow(xOrigin - mx, 2) + pow(yOrigin - my, 2)) <= radius;
}

//-------------------------------------------------------
// A function to handle mouse clicks
// Called every time the mouse button goes up or down
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appMouseFunc(int b, int s, int x, int y) {
	// Convert from Window to Scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToScene(mx, my);
	if (b == 0) {
		if (s == 0) {
			// Start drawing if inside the area
			if (box[1].contains(mx, my)) {
				cout << "Clicked in drawing box." << endl;
				dots.push_back(point(mx, my, RED, GREEN, BLUE));
			}
			if (insideCircle(xOffset, yOffset, radius, mx, my)) {
				if (box[1].contains(mx, my)) {
					dragging = true;
				}				
			}
			/*
				COLOR SELECTIONS
			*/
			// Change color to red
			else if (cBox[0].contains(mx, my)) {
				RED = 1.0;
				GREEN = 0.0;
				BLUE = 0.0;
			}
			// Change color to green
			else if (cBox[1].contains(mx, my)) {
				RED = 0.0;
				GREEN = 1.0;
				BLUE = 0.0;
			}
			// Change color to blue
			else if (cBox[2].contains(mx, my)) {
				RED = 0.0;
				GREEN = 0.0;
				BLUE = 1.0;
			}
			// Change color to yellow
			else if (cBox[3].contains(mx, my)) {
				RED = 1.0;
				GREEN = 1.0;
				BLUE = 0.0;
			}
			// Change color to magenta
			else if (cBox[4].contains(mx, my)) {
				RED = 1.0;
				GREEN = 0.0;
				BLUE = 1.0;
			}
			// Change color to cyan
			else if (cBox[5].contains(mx, my)) {
				RED = 0.0;
				GREEN = 1.0;
				BLUE = 1.0;
			}
			// Change color to black
			else if (cBox[6].contains(mx, my)) {
				RED = 0.0;
				GREEN = 0.0;
				BLUE = 0.0;
			}
			// Change color to white
			else if (cBox[7].contains(mx, my)) {
				RED = 1.0;
				GREEN = 1.0;
				BLUE = 1.0;
			}
			else if (cBox[8].contains(mx, my)) {
				RED = bgRED;
				GREEN = bgGREEN;
				BLUE = bgBLUE;
			}
			/*
				TOOL BOX SELECTIONS
			*/
			//
			/*else if (cBox[8].contains(mx, my)) {
				RED = bgRED;
				GREEN = bgGREEN;
				BLUE = bgBLUE;
			}
			*/
			// Statement just to check if clicks are within the actual boxes
			else {
				for (int j = 0; j < cBox.size(); j++) { 
					if (cBox[j].contains(mx, my)) {
						cout << "Clicked on colored box # " << j + 1 << "." << endl;
						break;
					}
				}
			}
		}
	}


	// Redraw the scene by calling appDrawScene above
	// so that the point we added above will get painted
	glutPostRedisplay();
}

//-------------------------------------------------------
// A function to handle mouse dragging
// Called every time mouse moves while button held down
// Arguments: 	
//	x, y - current coordinates of the mouse
//-------------------------------------------------------
void appMotionFunc(int x, int y) {
	float mx = (float)x;
	float my = (float)y;

	windowToScene(mx, my);
	if (dragging) {
		if (box[1].contains(mx, my)) {
			dots.push_back(point(mx, my, RED, GREEN, BLUE));
		}
	}
	// Again, we redraw the scene
	glutPostRedisplay();
}

//-------------------------------------------------------
// A function to handle keyboard events
// Called every time a key is pressed on the keyboard
// Arguments: 	
//	key  - the key that was pressed
//	x, y - coordinates of the mouse when key is pressed
//-------------------------------------------------------
void appKeyboardFunc(unsigned char key, int x, int y) {

	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	// After all the state changes, redraw the scene
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	// Setup window position, size, and title
	glutInitWindowPosition(20, 60);
	glutInitWindowSize(width, height);
	glutCreateWindow("Mark McCullough CSE165 Lab 3");

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	box.push_back(Rect(-0.95, 0.95, 0.3, 0.60, 0.5, 0.5, 0.5));									// Color palette box
	box.push_back(Rect(-0.54, 0.89, 1.38, 1.63, bgRED, bgGREEN, bgBLUE));						// Actual drawing box (box[1])
	box.push_back(Rect(-0.57, 0.92, 1.44, 1.69, bgRED, bgGREEN, bgBLUE));						// Prevents from drawing over the border
	box.push_back(Rect(-0.60, 0.95, 1.5, 1.75, 0.0, 0.0, 0.0));									// Border for drawing box
	box.push_back(Rect(-0.95, 0.0, 0.3, 0.15, 0.5, 0.5, 0.5));									// Tool Selection Box

	
	// Colors for the drawing palette, rendered within the color palette box
	cBox.push_back(colors(colorX, colorY, 0.10, 0.10, 1.0, 0.0, 0.0));
	cBox.push_back(colors(colorX + colorGap, colorY, 0.10, 0.10, 0.0, 1.0, 0.0));
	cBox.push_back(colors(colorX, colorY - colorGap, 0.10, 0.10, 0.0, 0.0, 1.0));
	cBox.push_back(colors(colorX + colorGap, colorY - colorGap, 0.10, 0.10, 1.0, 1.0, 0.0));
	cBox.push_back(colors(colorX, colorY - (2 * colorGap), 0.10, 0.10, 1.0, 0.0, 1.0));
	cBox.push_back(colors(colorX + colorGap, colorY - (2 * colorGap), 0.10, 0.10, 0.0, 1.0, 1.0));
	cBox.push_back(colors(colorX, colorY - (3 * colorGap), 0.10, 0.10, 0.0, 0.0, 0.0));
	cBox.push_back(colors(colorX + colorGap, colorY - (3 * colorGap), 0.10, 0.10, 1.0, 1.0, 1.0));

	// Boxes for the tool selection box
	cBox.push_back(colors(colorX, toolY, 0.24, 0.1, 1.0, 0.2, 0.2));
	/*
	cBox.push_back(colors(colorX + colorGap, toolY, 0.10, 0.10, 0.0, 0.0, 0.0));
	cBox.push_back(colors(colorX, toolY - colorGap, 0.10, 0.10, 0.0, 0.0, 0.0));
	cBox.push_back(colors(colorX + colorGap, toolY - colorGap, 0.10, 0.10, 0.0, 0.0, 0.0));
	*/

	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing th window
	glutReshapeFunc(appReshapeFunc);

	// Set callback to handle mouse clicks
	glutMouseFunc(appMouseFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboad events
	glutKeyboardFunc(appKeyboardFunc);

	// Start the main loop
	glutMainLoop();
}
