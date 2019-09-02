#include <iostream>
#include <cmath>
#include <vector>
#include "paint.h"

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <cmath>

point::point() {
	x = 0.0;
	y = 0.0;
	R = 0.0;
	G = 0.0;
	B = 0.0;
}
point::point(float x, float y, float R, float G, float B) {
	this->x = x;
	this->y = y;
	this->R = R;
	this->G = G;
	this->B = B;
}
void point::drawDot() {
	glPointSize(20.0f);
	glColor3f(R, G, B);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}
void point::eraseDot() {
	glPointSize(30.0f);
	glColor3f(R, G, B);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

colors::colors() {
	x = 0.0;
	y = 0.0;
	w = 0.1;
	h = 0.1;
	R = 0.0;
	G = 0.0;
	B = 0.0;
}
colors::colors(float x, float y, float w, float h, float R, float G, float B) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->R = R;
	this->G = G;
	this->B = B;
}
colors::colors(float x, float y, float R, float G, float B) {
	this->x = x;
	this->y = y;
	this->R = R;
	this->G = G;
	this->B = B;
	w = 0.05;
	h = 0.05;
}
void colors::colorDraw() {
	glColor3f(R, G, B);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y - h);
	glVertex2f(x, y - h);
	glEnd();
}
bool colors::contains(float mx, float my) {
	return mx >= x && mx <= x + w && my <= y && my >= y - h;
}

Rect::Rect() {
	x = 0.0;
	y = 0.0;
	w = 0.1;
	h = 0.1;
	R = 0.0;
	G = 0.0;
	B = 0.0;
}
Rect::Rect(float x, float y, float w, float h, float R, float G, float B) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->R = R;
	this->G = G;
	this->B = B;
}
bool Rect::contains(float mx, float my) {
	return mx >= x && mx <= x + w && my <= y && my >= y - h;
}
void Rect::draw() {
	glLineWidth(3);
	glColor3f(R, G, B);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y - h);
	glVertex2f(x, y - h);
	glEnd();
}