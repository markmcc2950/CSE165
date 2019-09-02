#include <iostream>
#include "App.h"

App* singleton;

void timer(int id){
    // This will get called every 16 milliseconds after
    // you call it once
    
    // If you want to manipulate the app in here
    // do it through the singleton pointer
	
	//std::cout << "Timer called." << std::endl;									// Verify timer is being called
	float bg1Pos = singleton->bg1->getX();										// Position of the 1st background
	float bg2Pos = singleton->bg2->getX();										// Position of the 2nd background
	float offset = singleton->offset;											// Speed of the background's motion
	
	singleton->bg1->setX(bg1Pos - offset);										// Moves the 1st background -x direction at a rate of [velocity]
	singleton->bg2->setX(bg2Pos - offset);										// Moves the 2nd background -x direction at a rate of [velocity]
	
	if (singleton->bg1->getX() <= -(singleton->bg2->getX()) - 2 * (singleton->bg2->getW())) {								// Move background 1 to far right when it goes off screen
		singleton->bg1->setX((singleton->bg2->getX())+(singleton->bg2->getW()) - 0.01);
	}
	if (singleton->bg2->getX() <= -(singleton->bg1->getX()) - 2*(singleton->bg1->getW())) {									// Move background 2 to far right when it goes off screen
		singleton->bg2->setX((singleton->bg1->getX()) + (singleton->bg1->getW()) - 0.01);
	}
	
    glutTimerFunc(16, timer, id);
}


App::App(int argc, char** argv, int width, int height, const char* title): GlutApp(argc, argv, width, height, title) {
	singleton = this;															// Singleton points to this App::App, prevents from being a null pointer																	

	offset = 0.01;																// How much the image moves per timer call (16 ms)

	bg1 = new TexRect("bg.png", -2.0, 1.0, 4.01, 2.0);	
	bg2 = new TexRect("bg.png", 2.0, 1.0, 4.01, 2.0);
	cat = new AnimatedRect("cat.png", 4, 2, 50, true, true, -0.5, -0.4, 1.0, 0.37);
	timer(5);																	// Call the timer
}

void App::draw() {
	bg1->draw(0.05);
	bg2->draw(0.05);
	cat->draw(0.1);
}

void App::keyDown(unsigned char key, float x, float y){
    if (key == 27){
        exit(0);
    }
	if (key == 'w') {
		if (singleton->offset < 0.1) {
			cat->resume();
			singleton->offset += 0.005;
			std::cout << "W key pressed" << std::endl;
		}
		else {
			std::cout << "Max speed reached" << std::endl;
		}
		
	}
	if (key == 's') {
		if (singleton->offset >= 0.01) {
			singleton->offset -= 0.005;
			std::cout << "S key pressed" << std::endl;
		}
		else {
			cat->reset();
			cat->pause();
			std::cout << "Minimum speed reached" << std::endl;
		}

	}
}

App::~App(){
	delete bg1;
	delete bg2;
	delete cat;
    std::cout << "Exiting..." << std::endl;
}
