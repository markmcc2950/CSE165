#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif




// Store the width and height of the window
int width = 640, height = 640;
bool clear = false;																// If true, clears the grid back to default
bool AI = false;																// If true, AI will alternate turns against the player
bool start = true;																// Designates the start of the game to pick a random player to go first
bool game = false;																// If true, game is still going, if false, game is over
bool selection = true;															// For selecting if it's 1 player vs AI or 2 player game

// Keeps track of which grid has had interaction
int grids[2][9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
// Coordinates of the grid
float coords[2][3] = {(-0.57 - 0.15) / 2, (0.35 - 0.15) / 2, (0.35 + 0.76) / 2,  0.5, 0.0, -0.5};

float radius = 0.15;															// Radius of the outer circle
float radius2 = radius * 0.75;													// Radius of the inner black circles

float xOffset = 0.3;
float yOffset = 0.2;

int player;																		// Keeps track of whose turn it is
int counter = 0;

bool left = false;
bool right = false;
bool up = false;
bool down = false;
bool animate = false;
bool top = false;
bool dragging = false;
float red = 0.0;
float green = 1.0;
float blue = 0.0;
float xD = 0;
float yD = 0;

//-------------------------------------------------------
// A function to draw the scene
//-------------------------------------------------------
void appDrawScene() {
	float M_PI = 3.14;
	float theta;
	float thetaInc = M_PI / 100;

	if (start == true && selection == true && game == false) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Draw the player icons (1 and 2)
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex2f(-0.5, 0.4);
		glVertex2f(-0.5, -0.4);
		glVertex2f(-0.55, -0.4);
		glVertex2f(-0.45, -0.4);
		glVertex2f(-0.5, 0.4);
		glVertex2f(-0.55, 0.3);
		glEnd();

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		for (theta = 0; theta < M_PI; theta += thetaInc) {
			glVertex3f(0.25 * cos(theta) + 0.25, 0.25 * sin(theta) + 0.15, 0.5);
		}
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		for (theta = 0; theta < M_PI; theta += thetaInc) {
			glVertex3f(0.23 * cos(theta) + 0.25, 0.23 * sin(theta) + 0.15, 0.55);
		}
		glEnd();
		glColor3f(0.0, 0.0, 1.0);
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex2f(0.5, 0.15);
		glVertex2f(0.0, -0.4);
		glVertex2f(0.0, -0.4);
		glVertex2f(0.5, -0.4);
		glEnd();

		// For picking a random player for each match
		srand(time(0));
		player = rand() % 2;
	}
	if (selection == false) {

		// Center points for Rows (1, 2, 3) and Columns (1, 2, 3)
		float R;
		float C;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set background color to black
		glClearColor(0.0, 0.0, 0.0, 1.0);

		// Set up the transformations stack
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Draw the grid
		glColor3f(0.75, 0.0, 1.0);
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex2f(-0.15, 0.75);
		glVertex2f(-0.15, -0.75);
		glVertex2f(0.35, 0.75);
		glVertex2f(0.35, -0.75);
		glVertex2f(-0.56, 0.25);
		glVertex2f(0.76, 0.25);
		glVertex2f(-0.56, -0.25);
		glVertex2f(0.76, -0.25);
		glEnd();

		// Draw the 'clear' button
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(5);
		glBegin(GL_LINES);

		glVertex2f(-0.95, 0.95);
		glVertex2f(-0.6, 0.95);
		glVertex2f(-0.95, 0.95);
		glVertex2f(-0.95, 0.75);
		glVertex2f(-0.95, 0.75);
		glVertex2f(-0.6, 0.75);
		glVertex2f(-0.6, 0.75);
		glVertex2f(-0.6, 0.95);

		glEnd();

		if (clear == true) {
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 0;
				grids[1][i] = 2;
			}
			clear = false;
			start = true;
			selection = false;
			game = true;
		}
		// Render whose turn it is
		if (player == 0) {
			R = 0.5;
			C = -0.9;
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_POLYGON);
			for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
				glVertex3f(radius / 2 * cos(theta) + C, radius / 2 * sin(theta) + R, 0.5);
			}
			glEnd();
			// Fill in the circle with a black circle to match background
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_POLYGON);
			for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
				glVertex3f(radius2 / 2 * cos(theta) + C, radius2 / 2 * sin(theta) + R, 0.55);
			}
			glEnd();
		}
		else if (player == 1) {
			R = 0.3;
			C = -0.9;
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);

			glVertex2f(C - 0.15 / 2, R - 0.15 / 2);
			glVertex2f(C + 0.15 / 2, R + 0.15 / 2);
			glVertex2f(C + 0.15 / 2, R - 0.15 / 2);
			glVertex2f(C - 0.15 / 2, R + 0.15 / 2);

			glEnd();
		}

		// TOP LEFT BOX
		if (grids[0][0] != 0) {
			R = coords[1][0];
			C = coords[0][0];
			if (grids[1][0] == 0) {													// Draw a circle in grid 1 (for player 1) [TOP LEFT]
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][0] == 1) {												// Draw an X in grid 1 (for player 2) [TOP LEFT]
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// TOP MIDDLE BOX
		if (grids[0][1] != 0) {
			R = coords[1][0];
			C = coords[0][1];
			if (grids[1][1] == 0) {													// Draw a circle in grid 2 (for player 1) [TOP MIDDLE]
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][1] == 1) {												// Draw an X in grid 2 (for player 2) [TOP MIDDLE]
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// TOP RIGHT BOX
		if (grids[0][2] != 0) {
			R = coords[1][0];
			C = coords[0][2];
			if (grids[1][2] == 0) {													// Draw a circle in grid 1 (for player 1) [TOP MIDDLE]
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][2] == 1) {												// Draw an X in grid 1 (for player 2) [TOP MIDDLE]
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// LEFT CENTER BOX
		if (grids[0][3] != 0) {
			R = coords[1][1];
			C = coords[0][0];
			if (grids[1][3] == 0) {
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][3] == 1) {
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// MIDDLE CENTER BOX
		if (grids[0][4] != 0) {
			R = coords[1][1];
			C = coords[0][1];
			if (grids[1][4] == 0) {
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][4] == 1) {
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// RIGHT CENTER BOX
		if (grids[0][5] != 0) {
			R = coords[1][1];
			C = coords[0][2];
			if (grids[1][5] == 0) {
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][5] == 1) {
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// BOTTOM LEFT BOX
		if (grids[0][6] != 0) {
			R = coords[1][2];
			C = coords[0][0];
			if (grids[1][6] == 0) {
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][6] == 1) {
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// BOTTOM MIDDLE BOX
		if (grids[0][7] != 0) {
			R = coords[1][2];
			C = coords[0][1];
			if (grids[1][7] == 0) {
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][7] == 1) {
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// BOTTOM RIGHT BOX
		if (grids[0][8] != 0) {
			R = coords[1][2];
			C = coords[0][2];
			if (grids[1][8] == 0) {
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius * cos(theta) + C, radius * sin(theta) + R, 0.5);
				}
				glEnd();
				// Fill in the circle with a black circle to match background
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				for (theta = 0; theta < 2 * M_PI; theta += thetaInc) {
					glVertex3f(radius2 * cos(theta) + C, radius2 * sin(theta) + R, 0.55);
				}
				glEnd();
			}
			else if (grids[1][8] == 1) {
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(5);
				glBegin(GL_LINES);

				glVertex2f(C - 0.15, R - 0.15);
				glVertex2f(C + 0.15, R + 0.15);
				glVertex2f(C + 0.15, R - 0.15);
				glVertex2f(C - 0.15, R + 0.15);

				glEnd();
			}
			else {
				std::cout << "ERROR: Player not found.\nTERMINATING PROGRAM" << std::endl;
			}
		}
		// Game win conditions (P1)
		// TOP ROW
		if (grids[1][0] == 0 && grids[1][1] == 0 && grids[1][2] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][0], coords[1][0], 0.6);
			glVertex3f(coords[0][2], coords[1][0], 0.6);
			glEnd();
			game = false;
		}
		// MIDDLE ROW
		else if (grids[1][3] == 0 && grids[1][4] == 0 && grids[1][5] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][0], coords[1][1], 0.6);
			glVertex3f(coords[0][2], coords[1][1], 0.6);
			glEnd();
			game = false;
		}
		// BOTTOM ROW
		else if (grids[1][6] == 0 && grids[1][7] == 0 && grids[1][8] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][0], coords[1][2], 0.6);
			glVertex3f(coords[0][2], coords[1][2], 0.6);
			glEnd();
			game = false;
		}
		// LEFT COLUMN
		else if (grids[1][0] == 0 && grids[1][3] == 0 && grids[1][6] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][0], coords[1][0], 0.6);
			glVertex3f(coords[0][0], coords[1][2], 0.6);
			glEnd();
			game = false;
		}
		// CENTER COLUMN
		else if (grids[1][1] == 0 && grids[1][4] == 0 && grids[1][7] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][1], coords[1][0], 0.6);
			glVertex3f(coords[0][1], coords[1][2], 0.6);
			glEnd();
			game = false;
		}
		// RIGHT COLUMN
		else if (grids[1][2] == 0 && grids[1][5] == 0 && grids[1][8] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][2], coords[1][0], 0.6);
			glVertex3f(coords[0][2], coords[1][2], 0.6);
			glEnd();
			game = false;
		}
		// TOP-LEFT TO BOTTOM-RIGHT
		else if (grids[1][0] == 0 && grids[1][4] == 0 && grids[1][8] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][0], coords[1][0], 0.6);
			glVertex3f(coords[0][2], coords[1][2], 0.6);
			glEnd();
			game = false;
		}
		// TOP-RIGHT TO BOTTOM-LEFT
		else if (grids[1][6] == 0 && grids[1][4] == 0 && grids[1][2] == 0) {
			std::cout << "Game won! Congratulations Player 1!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(0.0, 0.0, 1.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex3f(coords[0][0], coords[1][2], 0.6);
			glVertex3f(coords[0][2], coords[1][0], 0.6);
			glEnd();
			game = false;
		}

		// Game win conditions (P2)
		// TOP ROW
		if (grids[1][0] == 1 && grids[1][1] == 1 && grids[1][2] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][0], coords[1][0]);
			glVertex2f(coords[0][2], coords[1][0]);
			glEnd();
			game = false;
		}
		// CENTER ROW
		else if (grids[1][3] == 1 && grids[1][4] == 1 && grids[1][5] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][0], coords[1][1]);
			glVertex2f(coords[0][2], coords[1][1]);
			glEnd();
			game = false;
		}
		// BOTTOM ROW
		else if (grids[1][6] == 1 && grids[1][7] == 1 && grids[1][8] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][0], coords[1][2]);
			glVertex2f(coords[0][2], coords[1][2]);
			glEnd();
			game = false;
		}
		// LEFT COLUMN
		else if (grids[1][0] == 1 && grids[1][3] == 1 && grids[1][6] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][0], coords[1][0]);
			glVertex2f(coords[0][0], coords[1][2]);
			glEnd();
			game = false;
		}
		// CENTER COLUMN
		else if (grids[1][1] == 1 && grids[1][4] == 1 && grids[1][7] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][1], coords[1][0]);
			glVertex2f(coords[0][1], coords[1][2]);
			glEnd();
			game = false;
		}
		// RIGHT COLUMN
		else if (grids[1][2] == 1 && grids[1][5] == 1 && grids[1][8] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][2], coords[1][0]);
			glVertex2f(coords[0][2], coords[1][2]);
			glEnd();
			game = false;
		}
		// TOP-LEFT TO BOTTOM-RIGHT
		else if (grids[1][0] == 1 && grids[1][4] == 1 && grids[1][8] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][0], coords[1][0]);
			glVertex2f(coords[0][2], coords[1][2]);
			glEnd();
			game = false;
		}
		// TOP-RIGHT TO BOTTOM-LEFT
		else if (grids[1][6] == 1 && grids[1][4] == 1 && grids[1][2] == 1) {
			std::cout << "Game won! Congratulations Player 2!" << std::endl;
			for (int i = 0; i < 9; i++) {
				grids[0][i] = 1;
			}
			glColor3f(1.0, 0.0, 0.0);
			glLineWidth(5);
			glBegin(GL_LINES);
			glVertex2f(coords[0][0], coords[1][2]);
			glVertex2f(coords[0][2], coords[1][0]);
			glEnd();
			game = false;
		}
	}


	// We have been drawing everything to the back buffer
	// Swap the buffers to see the result of what we drew
	glFlush();
	glutSwapBuffers();
}

/*
	int x is the x-coordinate of the area to be drawn around
	int y is the y-coordinate of the area to be drawn around
*/


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

void wtsX(int x) {
	float mx = (float)x;
	mx = (2.0f*(mx / float(width))) - 1.0f;
}

void wtsY(int y) {
	float my = (float)y;
	my = 1.0f - (2.0f*(my / float(height)));
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


bool insideCircle(float xOrigin, float yOrigin, float radius, float mx, float my){
    // Check to see if distance from origin to mouse <= radius
    return sqrt(pow(xOrigin - mx, 2) + pow(yOrigin - my, 2)) <= radius;
}

void gridClick(int x, int y) {
	float mx = (float)x;
	float my = (float)y;
	windowToScene(mx, my);
	int Xo, Yo;
	// Check if 'clear' button was pushed
	if (x >= 15 && x <= 130 && y >= 10 && y <= 70) {
		std::cout << "CLEAR BUTTON CLICKED\n" << std::endl;
		clear = true;
	}
	// Check if click was within grid 1 (Top Left)
	if (mx >= -0.56 && mx <= -0.13 && my >= 0.27 && my <= 0.75 && grids[0][0] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 1 CLICKED\n" << std::endl;
		grids[0][0] = 1;
		if (player == 0) {
			grids[1][0] = 0;
			player = 1;
		}
		else {
			grids[1][0] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 2 (Top Middle)
	else if (mx >= -0.17 && mx <= 0.33 && my >= 0.27 && my <= 0.75 && grids[0][1] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 2 CLICKED\n" << std::endl;
		grids[0][1] = 1;
		if (player == 0) {
			grids[1][1] = 0;
			player = 1;
		}
		else {
			grids[1][1] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 3 (Top Right)
	else if (mx >= 0.37 && mx <= 0.76 && my >= 0.27 && my <= 0.75 && grids[0][2] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 3 CLICKED\n" << std::endl;
		grids[0][2] = 1;
		if (player == 0) {
			grids[1][2] = 0;
			player = 1;
		}
		else {
			grids[1][2] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 4 (Middle Left)
	else if (mx >= -0.56 && mx <= -0.13 && my >= -0.23 && my <= 0.23 && grids[0][3] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 4 CLICKED\n" << std::endl;
		grids[0][3] = 1;
		if (player == 0) {
			grids[1][3] = 0;
			player = 1;
		}
		else {
			grids[1][3] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 5 (Middle Middle)
	else if (mx >= -0.17 && mx <= 0.33 && my >= -0.23 && my <= 0.23 && grids[0][4] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 5 CLICKED\n" << std::endl;
		grids[0][4] = 1;
		if (player == 0) {
			grids[1][4] = 0;
			player = 1;
		}
		else {
			grids[1][4] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 6 (Middle Right)
	else if (mx >= 0.37 && mx <= 0.76 && my >= -0.23 && my <= 0.23 && grids[0][5] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 6 CLICKED\n" << std::endl;
		grids[0][5] = 1;
		if (player == 0) {
			grids[1][5] = 0;
			player = 1;
		}
		else {
			grids[1][5] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 7 (Bottom Left)
	else if (mx >= -0.56 && mx <= -0.13 && my >= -0.75 && my <= -0.27 && grids[0][6] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 7 CLICKED\n" << std::endl;
		grids[0][6] = 1;
		if (player == 0) {
			grids[1][6] = 0;
			player = 1;
		}
		else {
			grids[1][6] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 8 (Bottom Middle)
	else if (mx >= -0.17 && mx <= 0.33 && my >= -0.75 && my <= -0.27 && grids[0][7] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 8 CLICKED\n" << std::endl;
		grids[0][7] = 1;
		if (player == 0) {
			grids[1][7] = 0;
			player = 1;
		}
		else {
			grids[1][7] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	// Check if click was within grid 9 (Bottom Right)
	else if (mx >= 0.37 && mx <= 0.76 && my >= -0.75 && my <= -0.27 && grids[0][8] != 1) {
		Xo = mx;
		Yo = my;
		std::cout << "GRID 9 CLICKED\n" << std::endl;
		grids[0][8] = 1;
		if (player == 0) {
			grids[1][8] = 0;
			player = 1;
		}
		else {
			grids[1][8] = 1;
			player = 0;
		}
		// player = (player + 1) % 2;
	}
	else {
		player = player;
	}
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
	int Xo, Yo;
	int AIPick;																	// Random picks for the AI

	windowToScene(mx, my);

	//std::cout << b << std::endl;
	if (start == true && selection == true) {
		if (b == 0) {
			// Responding to left click
			if (s == 0) {
				// Check if 1 player vs AI or 2 player was selected
				if (mx <= 0.0) {
					AI = true;
					start = false;
					selection = false;
					game = true;
					_sleep(100);
					clear = true;
				}
				else if (mx > 0.0) {
					AI = false;
					start = false;
					selection = false;
					game = true;
					_sleep(100);
					clear = true;
				}
			}
		}
	}
	if (AI == true && selection == false) {
		while (player == 1 && game == true) {
			_sleep(100);
			AIPick = rand() % 9;
			if (grids[0][AIPick] == 0) {
				grids[0][AIPick] = 1;
				grids[1][AIPick] = 1;
				player = 0;
			}
			counter++;
			if (counter >= 20) {
				game = false;
				player = 0;
			}
		}
		counter = 0;
		if (game == false) {
			player = 0;
		}
		if (player == 0) {
			if (b == 0) {
				// Responding to left click
				std::cout << "Button clicked:\t" << s << std::endl;
				std::cout << "Area clicked (x,y):\t(" << x << ", " << y << ")" << std::endl;
				if (s == 0) {
					// Button down
					if (insideCircle(xOffset, yOffset, radius, mx, my)) {
						red = 1.0;
						green = 1.0;
						blue = 0.0;
						xD = mx - xOffset;
						yD = my - yOffset;

						dragging = true;

						// std::cout << "New Area Clicked:\t" << mx << "\t" << my << std::endl;
					}
					gridClick(x, y);
					// player = (player + 1) % 2;
				}
				else if (s == 1) {
					// Button up
					red = 0.0;
					green = 1.0;
					blue = 0.0;
					dragging = false;
				}
			}

			// Redraw the scene by calling appDrawScene above
			// so that the point we added above will get painted
			glutPostRedisplay();
		}
	}
	else if (AI == false && selection == false) {
		if (b == 0){
			// Responding to left click
			std::cout << "Button clicked:\t" << s << std::endl;
			std::cout << "Area clicked (x,y):\t(" << x << ", " << y << ")" << std::endl;
			if (s == 0) {
				// Button down
				if (insideCircle(xOffset, yOffset, radius, mx, my)){
					red = 1.0;
					green = 1.0;
					blue = 0.0;
					xD = mx - xOffset;
					yD = my - yOffset;
				
					dragging = true;

					// std::cout << "New Area Clicked:\t" << mx << "\t" << my << std::endl;
				}
				gridClick(x, y);
				// player = (player + 1) % 2;
			}
			else if (s == 1){
				// Button up
				red = 0.0;
				green = 1.0;
				blue = 0.0;
				dragging = false;
			}
		}

		// Redraw the scene by calling appDrawScene above
		// so that the point we added above will get painted
		glutPostRedisplay();
	}

}

//-------------------------------------------------------
// A function to handle mouse dragging
// Called every time mouse moves while button held down
// Arguments: 	
//	x, y - current coordinates of the mouse
//-------------------------------------------------------
void appMotionFunc(int x, int y) {
    // Convert from Window to Scene coordinates
    float mx = (float)x;
    float my = (float)y;

    windowToScene(mx, my);
    if (dragging){
        xOffset = mx - xD;
        yOffset = my - yD;
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
            
        case ' ':
            animate = !animate;
            break;
        case 27:
            exit(0);
            break;
        case 'q':
            //std::cout << "Q was pressed" << std::endl;
            // Increase radius
            radius += 0.1;
            break;
        
        case 'a':
            //std::cout << "A was pressed" << std::endl;
            radius -= 0.1;
            break;
        default:
            break;
    }

	// After all the state changes, redraw the scene
	glutPostRedisplay();
}



void appSpecialKey(int key, int x, int y){
//    cout << key << " was pressed" << endl;
    switch (key) {
        case 100:
            // Left
            left = true;
            break;
        
        case 101:
            // Up
            up = true;
            break;
            
        case 102:
            // Right
            right = true;
            break;
        case 103:
            // Down
            down = true;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}
void appSpecialUp(int key, int x, int y){
    //cout << key << " was released" << endl;
    switch (key) {
        case 100:
            // Left
            left = false;
            break;
            
        case 101:
            // Up
            up = false;
            break;
            
        case 102:
            // Right
            right = false;
            break;
        case 103:
            // Down
            down = false;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void idle() {
    if (animate){
        if (!top) yOffset += 0.05;
        else yOffset -= 0.05;
        
        if (yOffset > 0.5) top = true;
        if (yOffset < -0.5) top = false;
    }
    
    if (left) xOffset -= 0.05;
    if (right) xOffset += 0.05;
    if (up) yOffset += 0.05;
    if (down) yOffset -= 0.05;
    glutPostRedisplay();
}


int main(int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	// Setup window position, size, and title
	glutInitWindowPosition(20, 60);
	glutInitWindowSize(width, height);
	glutCreateWindow("Mark McCullough: CSE165 Lab 2");

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);


	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing the window
    glutReshapeFunc(appReshapeFunc);

	// Set callback to handle mouse clicks
	glutMouseFunc(appMouseFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboad events
	glutKeyboardFunc(appKeyboardFunc);
    
    glutSpecialFunc(appSpecialKey);
    
    glutSpecialUpFunc(appSpecialUp);
    
    glutIdleFunc(idle);



	// Start the main loop
	glutMainLoop();
}
