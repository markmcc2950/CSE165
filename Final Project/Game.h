#ifndef Game_hpp
#define Game_hpp

#include "AppComponent.h"
#include "AnimatedRect.h"
#include "pacChar.h"
#include "ghostChar.h"
#include "Timer.h"
#include "sound.h"
#include <vector>
#include <sstream>

class Game: public AppComponent, public Timer{	
	std::vector<Rect> dots;																	// Vector of dots for Pacman to eat
	std::vector<bool> dotsGrabbed;															// Vector of bools to check if Pacman has eaten a dot

	std::vector<AppComponent*> components;
	void addComponent(AppComponent* component) {
		components.push_back(component);
	}

	// Iterators
	int i;
	int k;
	
	//**************************************VARIABLE DECLARATIONS*********************************************************
	float dotsSpacing = 0.00;																// How far spaced the dots are
	bool fakeDeath = false;																	// For pressing 'k' to force kill Pacman
	bool flipped = false;																	// For flipping speed boost

	// GAME TYPE SELECTION
	bool gameSel = true;																	// True while selection game mode
	bool classic = false;																	// Collect dots to win
	bool survival = false;																	// Time survival mode

	// DIFFICULTY SELECTIONS FOR TIME SURVIVAL
	bool diffSel = false;																	// True at beginning of game, set to false when difficulty is selected
	bool easySel = false;																	// Time doesn't reset upon death
	bool normalSel = false;																	// Time resets upon death. Must survive 90 seconds in one life
	bool hardSel = false;																	// Same as normal, ghosts are faster
	bool expertSel = false;																// Same as hard, but only one life
	
	// MAIN GAME STUFF
	int timer = 0;																			// Timer to use for multiple things
	float size = 0.1;																		// Size of pacman and ghosts
	float speed = 0.0005;																	// How fast pacman moves
	float tempSpeed = 0;																	// Used for swapping Pacman's speeds when using a speed boost powerup
	float ghostSpeed = speed * 1.15;														// How fast ghosts move (always slightly faster)
	bool ghostMove = true;																	// Ghosts move while true
	int lives = 3;																			// Amount of lives (max 3)
	float buffer = 0.01;																	// How much distance is checked for when ghosts hit their positions
	float pathBuff = 6 * buffer;															// Proximity for Pacman within paths to be centered
	bool soundLoop = false;																	// If true, sound is looping (usually during Pacman's movements)
	int ghostTimer = 0;																		// Timer to change ghosts back to blue when power pellet used

	// POWERUP STUFF
	bool powerup = true;																	// If true, powerup will spawn on map
	bool powerupUsed = false;																// True if spawned powerup is used
	float randPUpX[4];
	float randPUpY[4];
	bool boost = false;
	bool boostGrabbed = false;
	int boostCounter = 0;

	// PACMAN POSITIONS
	float spawnX = 0.0;																		// Where Pacman spawns x-coordinate
	float spawnY = -0.38;																	// Where Pacman spawns y-coordinate	

	// GHOST POSITIONS
	float bX = 0.88;																		// Blinky's X-coordinate spawnpoint
	float bY = -0.38;																		// Blinky's Y-coordinate spawnpoint
	float pX = -0.98;																		// Pinky's X-coordinate spawnpoint
	float pY = 0.68;																		// Pinky's Y-coordinate spawnpoint
	float iX = -0.38;																		// Inky's Y-coordinate spawnpoint
	float iY = 0.68;																		// Inky's's Y-coordinate spawnpoint
	float cX = 0.37;																		// Clyde's Y-coordinate spawnpoint
	float cY = 0.33;																		// Clyde's Y-coordinate spawnpoint
	float cY2 = 0.13;																		// Clyde's 2nd Y-coordinate for movement	

	// GHOST STUFF
	bool ghostsMoving = true;																// Ghosts move if true (helps with pausing the game)
	int blinkyPos = 4;																		// Associated with Blinky's movement algorithm
	int pinkyPos = 1;																		// Associated with Pinky's movement algorithm
	int inkyPos = 4;																		// Associated with Inky's movement algorithm
	int clydePos = 6;																		// Associated with Clyde's movement algorithm

	// OTHER STUFF
	float distance = 0.1;																	// Border thickness
	bool gameOver = false;																	// If defeated
	bool victory = false;																	// If victorious
	bool gameRunning = false;																// If the user can control and play the game

	// MOVEMENT BOOLEANS
	bool moving = false;																	// General movement
	bool moveUp = false;																	// When moving up (if able)
	bool moveDown = false;																	// When moving down (if able)
	bool moveLeft = false;																	// When moving left (if able)
	bool moveRight = false;																	// When moving right (if able)
	//****************//
	bool upCmd = true;																		// If true, can move up (unblocked)
	bool downCmd = true;																	// If true, can move down (unblocked)
	bool leftCmd = true;																	// If true, can move left (unblocked)
	bool rightCmd = true;																	// If true, can move right (unblocked)

	bool soundPlaying = false;																// Prevents sound from restarting if directional movements triggered multiple times
	bool hit = false;																		// For if Pacman is hit
	bool spawn = true;																		// If false, show Respawn menu

	float rate = 0.00001;																	// Rate the progress bar fills. 0.00001 = 90 seconds
	float modifier = 1;																		// Used to alter ghost movement speeds
	float temp;																				// Used as third variable in tandem with modifier (above)
	bool paused = false;																	// If game is paused or not
	//********************************************************************************************************************	

	//**********************************************DRAWING SECTION*******************************************************
	// DIFFICULTY & GAMETYPE CHOICES
	TexRect* gametype;																		// Menu for selecting gametype
	TexRect* selection;																		// Menu for choosing difficulty (header)
	TexRect* difficulties;																	// Menu for choosing difficulty (actual difficulties)
	TexRect* easy;																			// Draws Easy on bottom right if selected
	TexRect* normal;																		// Draws Normal on bottom right if selected
	TexRect* hard;																			// Draws Hard on bottom right if selected
	TexRect* expert;																		// Draws Expert on bottom right if selected
	TexRect* movement;																		// UI guide for moving controls

	// POWERUPS
	AnimatedRect* powerPellet;																// When picked up, makes ghosts blue
	AnimatedRect* speedBoost;																	// When picked up, makes Pacman temporarily faster

	// PACMAN
	pacChar* pacman;																		// User-controlled Pacman
	pacChar* pacHit;																		// Death animation for Pacman
	sound* waka;																			// Sound that loops for movement
	sound* dead;																			// Sound that plays once with death animation

	// GHOSTS
	ghostChar* blinky;																		// Blinky = Red Ghost
	ghostChar* bDead;
	ghostChar* pinky;																		// Pinky = Pink Ghost (Duh)
	ghostChar* pDead;
	ghostChar* inky;																		// Inky = Cyan Ghost
	ghostChar* iDead;
	ghostChar* clyde;																		// He's the orange one
	ghostChar* cDead;
	
	// PACMAN'S LIVES
	TexRect* life1;
	TexRect* life2;
	TexRect* life3;
	
	// UI SCREENS
	TexRect* respawn;																		// Shows up if dead
	TexRect* spacebar;																		// Shows up if dead (Press Spacebar to respawn)
	Rect* progress;																			// Progress bar (Survival or Classic Mode)
	Rect* finishLine;																		// Used as reference for when the player wins in Survival
	TexRect* lost;																			// Game over screen
	AnimatedRect* won;																		// You won screen
	TexRect* restart;																		// Restart prompt screen

	// STAGE BUILDING
	Rect* stageBorder1;																		// Black background
	Rect* stageBorder2;																		// White border
	Rect* yellowColumn;																		// Bottom left block of stage
	Rect* pinkColumn;																		// Top middle block of stage
	Rect* redColumn;																		// Bottom middle block of stage
	Rect* blueColumn;																		// Right block of stage
	Rect* orangeColumn;																		// Top left block of stage
	//********************************************************************************************************************
public:
    Game();
    void draw() const;
    void handleKeyDown(unsigned char, float, float);
    void action();

    ~Game();
};

#endif 
