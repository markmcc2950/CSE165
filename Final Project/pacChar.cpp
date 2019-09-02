// Code by Mark McCullough
#include "pacChar.h"
#include <iostream>



pacChar::pacChar(const char* map_filename, int rows, int cols, int rate, bool visible = false, bool animated = false, float x = 0, float y = 0, float w = 0.5, float h = 0.5) : TexRect(map_filename, x, y, w, h) {


	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	texture_id = SOIL_load_OGL_texture
	(
		map_filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	this->rows = rows;
	this->cols = cols;

	flipped = false;
	moveUp = false;
	moveDown = false;
	moveLeft = false;
	moveRight = true;

	curr_row = 1;
	curr_col = 1;

	complete = false;
	animating = animated;
	loop = animated;

	this->visible = visible;
	setRate(rate);
	start();
}

bool pacChar::done() {
	return complete;
}

void pacChar::flip() {
	flipped = !flipped;
}

void pacChar::up() {																	// Redraws Pacman using the same sprite so it's facing up
	moveUp = true;
	moveDown = false;
	moveLeft = false;
	moveRight = false;
}

void pacChar::down() {																	// Redraws Pacman using the same sprite so it's facing down
	moveDown = true;
	moveUp = false;
	moveLeft = false;
	moveRight = false;
}

void pacChar::left() {																	// Redraws Pacman using the same sprite so it's facing left
	moveLeft = true;
	moveDown = false;
	moveUp = false;
	moveRight = false;
}

void pacChar::right() {																	// Redraws Pacman using the same sprite so it's facing right
	moveRight = true;
	moveDown = false;
	moveLeft = false;
	moveUp = false;
}

void pacChar::draw(float z) {
	if (animating || visible) {
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


		float xinc = 1.0 / cols;
		float yinc = 1.0 / rows;


		float top = 1 - yinc * (curr_row - 1);
		float bottom = 1 - yinc * curr_row;

		float left = xinc * (curr_col - 1);
		float right = xinc * curr_col;

		if (flipped) {
			float temp = left;
			left = right;
			right = temp;
		}
		// Draw Pacman facing the default direction (right)
		if (moveRight) {
			glBegin(GL_QUADS);

			glTexCoord2f(left, bottom);
			glVertex3f(x, y - h, z);

			glTexCoord2f(left, top);
			glVertex3f(x, y, z);

			glTexCoord2f(right, top);
			glVertex3f(x + w, y, z);

			glTexCoord2f(right, bottom);
			glVertex3f(x + w, y - h, z);

			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
		// Draw Pacman facing left
		else if (moveLeft) {
			glBegin(GL_QUADS);

			glTexCoord2f(left, bottom);
			glVertex3f(x + w, y - h, z);

			glTexCoord2f(left, top);
			glVertex3f(x + w, y, z);

			glTexCoord2f(right, top);
			glVertex3f(x, y, z);

			glTexCoord2f(right, bottom);
			glVertex3f(x, y - h, z);

			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
		// Draw Pacman facing down
		else if (moveDown) {
			glBegin(GL_QUADS);

			glTexCoord2f(left, bottom);
			glVertex3f(x, y, z);

			glTexCoord2f(left, top);
			glVertex3f(x + w, y, z);

			glTexCoord2f(right, top);
			glVertex3f(x + w, y - h, z);

			glTexCoord2f(right, bottom);
			glVertex3f(x, y - h, z);

			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
		// Draw Pacman facing up
		else if (moveUp) {
			glBegin(GL_QUADS);

			glTexCoord2f(left, bottom);
			glVertex3f(x, y - h, z);

			glTexCoord2f(left, top);
			glVertex3f(x + w, y - h, z);

			glTexCoord2f(right, top);
			glVertex3f(x + w, y, z);

			glTexCoord2f(right, bottom);
			glVertex3f(x, y, z);

			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	}
}

void pacChar::advance() {
	if (curr_col < cols) {
		curr_col++;
	}
	else {
		if (curr_row < rows) {
			curr_row++;
			curr_col = 1;
		}
		else {
			curr_row = 1;
			curr_col = 1;
		}
	}

	if (curr_row == rows && curr_col == cols) {
		complete = true;
	}
}

void pacChar::reset() {
	complete = false;
	curr_row = 1;
	curr_col = 1;
}

void pacChar::playLoop() {
	reset();
	animating = true;
	loop = true;
}

void pacChar::playOnce() {
	reset();
	animating = true;
	loop = false;
}

void pacChar::pause() {
	animating = false;
}

void pacChar::resume() {
	animating = true;
}

void pacChar::play() {
	start();
}

void pacChar::disappear() {
	visible = false;
	animating = false;
}

void pacChar::reappear() {
	visible = true;
	animating = true;
}

void pacChar::action() {
	if (animating) {
		if (loop) {
			advance();
			redrawScene();
		}
		else {
			if (!done()) {
				advance();
				redrawScene();
			}
		}
	}
}

