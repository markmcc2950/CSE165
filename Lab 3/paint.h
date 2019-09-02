#ifndef paint_h
#define paint_h

#include <iostream>
#include <cmath>
#include <vector>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <cmath>

struct point {
	float R;
	float G;
	float B;
	float x = 0.0;
	float y = 0.0;
	
	point();
	point(float x, float y, float R, float G, float B);
	void drawDot();
	void eraseDot();
};

struct colors {
	float R, G, B;
	float x, y, w, h;

	colors();
	colors(float x, float y, float w, float h, float R, float G, float B);
	colors(float x, float y, float R, float G, float B);
	void colorDraw();
	bool contains(float mx, float my);
};

struct Rect {
	float x;
	float y;
	float w;
	float h;
	float R;
	float G;
	float B;

	Rect();
	Rect(float x, float y, float w, float h, float R, float G, float B);
	bool contains(float mx, float my);
	void draw();
};

#endif
