#include <iostream>
#include "App.h"

App* singleton;
bool exploded = false;

void timer(int id){
    // This will get called every 16 milliseconds after
    // you call it once
    
    // If you want to manipulate the app in here
    // do it through the singleton pointer    
    if (singleton->up){
        float ypos = singleton->projectile->getY();
        ypos +=0.05;
        singleton->projectile->setY(ypos);
        singleton->redraw();
    }

	/*
	MY CODE (Mark McCullough):
	If the projectile is within the height bounds of the mushroom:
	- Delete the mushroom & projectile
	- Set exploded = true
		* This prevents the loopOnce animation from playing for every frame the projectile is within the bounds
	- Play the explosion animation once
	- Draw the explosion
	- Output a message (test to make sure bounds were met without needing rendering)
	*/

	if (singleton->projectile->getY() >= (singleton->mushroom->getY() - singleton->mushroom->getH())
		&& singleton->projectile->getY() <= singleton->mushroom->getY() && exploded == false)
	{
		exploded = true;
		std::cout << "Deleting mushroom..." << std::endl;
		delete singleton->mushroom;
		std::cout << "Deleting projectile..." << std::endl;
		delete singleton->projectile;		
		//singleton->projectile->setX(singleton->projectile->getX() + 2);
		singleton->explode->playLoop();
		singleton->explode->draw(0.2);
		std::cout << "BOOM" << std::endl;
	}

    glutTimerFunc(16, timer, id);
}

App::App(int argc, char** argv, int width, int height, const char* title): GlutApp(argc, argv, width, height, title){
	// Create AnimatedRect object for the fireball
	explode = new AnimatedRect("fireball.png", 6, 6, 40, true, false, -0.25,  0.9, 0.5, 0.5);
    mushroom = new TexRect("mushroom.png", -0.25, 0.9, 0.5, 0.5);
    projectile = new Rect(-0.05, -0.8, 0.1, 0.1);
    up = false;
    
    singleton = this;    
    timer(1);
}

void App::draw() {
	if (exploded == false) {													// Stops drawing when the explosion happens
		projectile->draw();
		mushroom->draw(0.1);		
	}   
	explode->draw(0.0);															// "Draw" explosion below the mushroom. Moves above mushroom z-coordinates for actual explosion
}

void App::keyDown(unsigned char key, float x, float y){
    if (key == 27){
        exit(0);
    }
    if (key == ' '){
        up = true;
    }
}

App::~App(){
    delete mushroom;
	delete projectile;
	delete explode;
    std::cout << "Exiting..." << std::endl;
}
