// Code by Mark McCullough
#ifndef pacChar_h
#define pacChar_h

#include "TexRect.h"
#include "Timer.h"

class pacChar : public TexRect, protected Timer {

	int rows;
	int cols;

	int curr_row;
	int curr_col;

	bool complete;

	bool loop;

	bool animating;
	bool visible;
	bool pacman;

	void advance();
	bool done();

	bool flipped;
	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;

public:


	pacChar(const char*, int, int, int, bool, bool, float, float, float, float);

	void draw(float z);

	void playLoop();

	void playOnce();

	void reset();

	void pause();

	void resume();

	void action();

	void play();

	void flip();

	void up();

	void down();

	void left();

	void right();

	void disappear();
	void reappear();

};

#endif
