#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <MMSystem.h>
#include <vector>
#include "Game.h"
#include <time.h>

/*
	Code for Pacman game done by Mark McCullough, Darshan Bulsara, and Baudelio Perez
	Stage creation done by Darshan Bulsara
		- Placement of the 5 color Rect* class objects
		- Basic implementation of speed boost function 
			* This was fixed by Mark by allowing Pacman to slow back down after use
	All sprites and ghost pathing concept made by Baudelio Perez
		- General design of ghost movements
		- Inky, Blinky, Pinky, Clyde & poweruped blue ghost sprites
		- Pacman's animated sprite
		- Pacman's death animation sprite
		- Text sprites ("You Won", Game Over, Respawn, Press Space)
	All other code by Mark McCullough
		- Pacman movement
		- Ghost hit detection
		- Implementation and coordinate mapping of ghost movements
		- Pacman's lives
		- Everything UI (menus, life counter, progress bar, border around map, showing of difficulty while playing)
		- Map collision detection (block Pacman's movements through stage)
		- Dot generation and removal for Pacman to eat
		- Score counter for "Classic" mode
		- Time tracker for "Survival" mode
		- Sound input (Windows only)
		- All difficulty options and implementation
		- Reseting the board after losing and allowing the user to keep playing until they exit the program
		- Text Sprites (Choose your difficulty, easy, medium, hard, expert, classic & survival mode, movement keyboard prompt, restart
		- Speed boost and power pellet sprite
		- Powerup to disable ghost hit detection
	
	With assistance from code distributed by Angelo Kyrilov
	
	Sounds downloaded from https://www.classicgaming.cc/classics/pac-man/sounds
	Spring 2019, UC Merced
*/



Game::Game() {
	dots.resize(100);																		// Allocate initial size of vector (Will be resized smaller when all dots are drawn)
	srand(time(0));

	// Create positions of all the areas the powerup orb can spawn
	randPUpX[0] = bX;
	randPUpX[1] = pX;
	randPUpX[2] = cX;
	randPUpX[3] = iX;
	randPUpY[0] = bY;
	randPUpY[1] = pY;
	randPUpY[2] = cY;
	randPUpY[3] = iY;

	// DIFFICULTY & GAMETYPE SELECTION																	(@MM)
	gametype = new TexRect("Sprites/Text/gameSelection.png", -0.6, 0.6, 1.2, 1.2);			// Selecting gametype (survival or classic)
	selection = new TexRect("Sprites/Text/difficulty.png", -0.8, 0.68, 1.6, 0.3);			// Shows the phrase "Select Your Difficulty"
	difficulties = new TexRect("Sprites/Text/difficulties.png", -0.6, 0.3, 1.2, 1.0);		// Shows all difficulty options
	easy = new TexRect("Sprites/Text/1easy.png", -1.5, -0.7, 0.8, 0.2);
	normal = new TexRect("Sprites/Text/2normal.png", -1.5, -0.7, 1.2, 0.2);
	hard = new TexRect("Sprites/Text/3hard.png", -1.5, -0.7, 0.8, 0.2);
	expert = new TexRect("Sprites/Text/4expert.png", -1.5, -0.7, 1.2, 0.2);

	// POWERUP STUFF																					(@MM)
	powerPellet = new AnimatedRect("Sprites/PowerPellet.png", 1, 6, 150, true, true, 0, 0, size, size);
	speedBoost = new AnimatedRect("Sprites/SpeedBoost.png", 1, 3, 200, true, true, cX, pY, size * 3, size);

	// PACMAN STUFF																						(@MM)
	pacman = new pacChar("pacman.png", 4, 5, 20, true, true, spawnX, spawnY, size, size);
	pacHit = new pacChar("pacmanDeath.png", 4, 5, 80, false, false, 0.0, 0.0, size, size);
	waka = new sound("pacman_chomp.wav");
	dead = new sound("pacman_death.wav");

	// GHOST STUFF																						(@MM & BP)
	blinky = new ghostChar("Blinky-Down.png", 1, 2, 100, true, true, bX, bY, size, size);
	bDead = new ghostChar("Sprites/BlueVul2.png", 1, 4, 100, false, false, bX, bY, size, size);
	pinky = new ghostChar("Pinky-Down.png", 1, 2, 100, true, true, pX, pY, size, size);
	pDead = new ghostChar("Sprites/BlueVul2.png", 1, 4, 100, false, false, pX, pY, size, size);
	inky = new ghostChar("Inky-Down.png", 1, 2, 100, true, true, iX, iY, size, size);
	iDead = new ghostChar("Sprites/BlueVul2.png", 1, 4, 100, false, false, iX, iY, size, size);
	clyde = new ghostChar("Clyde-Down.png", 1, 2, 100, true, true, cX, cY, size, size);
	cDead = new ghostChar("Sprites/BlueVul2.png", 1, 4, 100, false, false, cX, cY, size, size);

	// UI STUFF																							(@MM)
	movement = new TexRect("Sprites/Text/Movement.png", 1.0, -0.6, 0.5, 0.4);
	life1 = new TexRect("Sprites/lives.png", 0.9, 0.9, 0.1, 0.1);
	life2 = new TexRect("Sprites/lives.png", 1.05, 0.9, 0.1, 0.1);
	life3 = new TexRect("Sprites/lives.png", 1.2, 0.9, 0.1, 0.1);
	respawn = new TexRect("Sprites/Text/Respawn.png", -0.21, 0.05, 0.5, 0.1);
	spacebar = new TexRect("Sprites/Text/Press_Space.png", -0.26, -0.1, 0.6, 0.1);
	progress = new Rect(-1.5, 0.9, 0.0, 0.05);
	finishLine = new Rect(-0.62, 0.93, 0.02, 0.11);
	progress->setR(1.0);
	progress->setG(0.0);
	progress->setB(0.0);
	lost = new TexRect("Sprites/Text/gameOver.png", -0.4, 0.1, 0.8, 0.2);					// Gameover Screen
	restart = new TexRect("Sprites/Text/restart.png", -0.3, -0.2, 0.6, 0.1);				// Gameover Screen
	won = new AnimatedRect("Sprites/Text/victory.png", 4, 2, 50, true, true, -0.4, 0.1, 0.8, 0.2);

	// STAGE STUFF (x, y, w, h)																			(@DB & MM) 
	stageBorder1 = new Rect(-1.0, 0.7, 2.0, 1.2, 0.0, 0.0, 0.0);							// Black background
	stageBorder2 = new Rect(-1.02, 0.72, 2.04, 1.24, 1.0, 1.0, 1.0);						// White border
	yellowColumn = new Rect(-0.85, -0.01, 0.45, 0.35, 0.8, 1.0, 0.0);						// Bottom left (yellow)
	orangeColumn = new Rect(-0.85, 0.56, 0.45, 0.40, 1.0, 0.5, 0.0);						// Top left (orange)
	blueColumn = new Rect(0.5, 0.56, 0.35, 0.91, 0.0, 0.0, 1.0);							// Right (blue)	
	pinkColumn = new Rect(-0.25, 0.56, 0.58, 0.20, 1.0, 0.7, 0.7);							// Top middle (pink)
	redColumn = new Rect(-0.25, 0.19, 0.58, 0.55, 1.0, 0.0, 0.0);							// Bottom middle (red)

	// DRAW DOTS																						(@MM)
	// DRAW BOTTOM ROW OF DOTS
	for (i = 0; i < 1000; i++) {
		dots[i].setX(pX + (pinky->getW() / 2) + dotsSpacing);
		dots[i].setY(bY - (blinky->getH() / 2));
		dotsSpacing += 0.15;
		if (dots[i].getX() >= bX) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	// DRAW TOP ROW OF DOTS
	for (i = k; i < 1000; i++) {
		dots[i].setX(pX + (pinky->getW() / 2) + dotsSpacing);
		dots[i].setY(pY - (pinky->getH() / 2));
		dotsSpacing += 0.15;
		if (dots[i].getX() >= bX) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	// DRAW LEFT SIDE ROW OF DOTS
	for (i = k; i < 1000; i++) {
		dots[i].setX(orangeColumn->getX() + dotsSpacing);
		dots[i].setY(cY2 - (clyde->getH() / 2));
		dotsSpacing += 0.15;
		if (dots[i].getX() >= iX) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	// DRAW CENTER RAISED AREA ROW OF DOTS
	for (i = k; i < 1000; i++) {
		dots[i].setX(pinkColumn->getX() + dotsSpacing);
		dots[i].setY(cY - (clyde->getH() / 2));
		dotsSpacing += 0.15;
		if (dots[i].getX() >= cX) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	// DRAW LEFT COLUMN OF DOTS
	for (i = k; i < 1000; i++) {
		dots[i].setX(pX + (pinky->getW() / 2));
		dots[i].setY(pY - (pinky->getH() / 2) - dotsSpacing);
		dotsSpacing += 0.15;
		if (dots[i].getY() <= bY) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	// DRAW RIGHT COLUMN OF DOTS
	for (i = k; i < 1000; i++) {
		dots[i].setX(bX + (blinky->getW() / 2));
		dots[i].setY(blueColumn->getY() - dotsSpacing);
		dotsSpacing += 0.13;
		if (dots[i].getY() <= bY) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	// DRAW CENTER RIGHT COLUMN OF DOTS
	for (i = k; i < 1000; i++) {
		dots[i].setX(cX + (clyde->getW() / 2));
		dots[i].setY(pY - (pinky->getH() / 2) - dotsSpacing);
		dotsSpacing += 0.15;
		if (dots[i].getY() <= bY) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	// DRAW CENTER LEFT COLUMN OF DOTS
	for (i = k; i < 1000; i++) {
		dots[i].setX(iX + (inky->getW() / 2));
		dots[i].setY(pY - (pinky->getH() / 2) - dotsSpacing);		
		dotsSpacing += 0.15;
		if (dots[i].getY() <= bY) {
			dotsSpacing = 0.00;
			k = i;																			// Saves the last value of the vector used before breaking
			break;
		}
	}
	dotsGrabbed.resize(k);																	// Set size of dotsGrabbed boolean array to be of size equal to number of dots created
	dots.resize(k);																			// Resize dots vector so we only use memory for what dots were drawn
	for (i = 0; i < k; i++) {
		dots[i].setW(0.01);																	// Set width of all dots made
		dots[i].setH(0.01);																	// Set height of all dots made
		dotsGrabbed[i] = false;																// Set all drawn dots as able to be grabbed by Pacman
	}

    setRate(1);
    start();
}

void Game::action() {
	// Game type selection																				(@MM)
	if (gameSel) {
		// Reset all dots to ungrabbed
		for (i = 0; i < k; i++) {
			dotsGrabbed[i] = false;
		}
		// Reinitialize starting variables
		rate = 0.00001;
		moving = false;
		moveUp = false;
		moveLeft = false;
		moveRight = false;
		moveDown = false;
		gameOver = false;
		victory = false;
		waka->stop();
		progress->setR(1.0);
		progress->setG(0.0);

		// Reset status bar
		progress->setW(0);

		// Reset spawn positions in case this is a restart after a defeat or victory
		pacman->setX(spawnX);
		pacman->setY(spawnY);
		inky->setX(iX);
		inky->setY(iY);
		blinky->setX(bX);
		blinky->setY(bY);
		pinky->setX(pX);
		pinky->setY(pY);
		clyde->setX(cX);
		clyde->setY(cY);
		powerup = true;
		powerupUsed = false;
		boostGrabbed = false;
		boost = false;
		

		if ((classic || survival)) {
			timer += 1;
			if (timer >= 500) {
				diffSel = true;
				gameSel = false;
			}
		}
	}
	if ((survival || classic)) {
		// During difficulty selection/Initial game setup
		if (diffSel && (easySel || normalSel || hardSel || expertSel)) {
			timer = 0;

			// Reinitialize starting variables
			rate = 0.00001;
			moving = false;
			moveUp = false;
			moveLeft = false;
			moveRight = false;
			moveDown = false;
			gameOver = false;
			victory = false;
			waka->stop();
			progress->setR(1.0);
			progress->setG(0.0);

			// Reset status bar
			progress->setW(0);

			// Set difficulty modifiers
			if (expertSel) {
				lives = 0;
				ghostSpeed = speed * 1.4;
				rate = 0.00001;
			}
			else if (hardSel) {
				ghostSpeed = speed * 1.4;
				lives = 3;
				rate = 0.00001;
			}
			else if (normalSel || easySel) {
				ghostSpeed = speed * 1.15;
				lives = 3;
				if (easySel) {
					rate = 0.00003;
				}
				if (normalSel) {
					rate = 0.00002;
				}

			}

			// Reset spawn positions in case this is a restart after a defeat or victory
			pacman->setX(spawnX);
			pacman->setY(spawnY);
			inky->setX(iX);
			inky->setY(iY);
			blinky->setX(bX);
			blinky->setY(bY);
			pinky->setX(pX);
			pinky->setY(pY);
			clyde->setX(cX);
			clyde->setY(cY);

			// Randomly spawn the powerup at a different point of the map
			powerPellet->setX(randPUpX[rand() % 4]);
			powerPellet->setY(randPUpY[rand() % 4]);

			diffSel = false;
			gameRunning = true;
			paused = false;
		}

		// While game is running
		if (gameRunning) {
			if (pacman->getX() > speedBoost->getX() && flipped == false) {
				speedBoost->flip();
				flipped = true;
			}
			if (pacman->getX() <= speedBoost->getX() && flipped == true) {
				speedBoost->flip();
				flipped = false;
			}
			// Grab dots
			if (classic) {
				for (i = 0; i < k; i++) {
					if (pacman->getX() + (pacman->getW() / 2) <= dots[i].getX() + 0.01 && pacman->getX() + (pacman->getW() / 2) >= dots[i].getX() - 0.01
						&& pacman->getY() - (pacman->getH() / 2) <= dots[i].getY() + 0.01 && pacman->getY() - (pacman->getH() / 2) >= dots[i].getY() - 0.01) {
						if (dotsGrabbed[i] == false) {
							progress->setW(progress->getW() + (0.8909 / k));
							progress->setR(progress->getR() - (0.8909 / k));
							progress->setG(progress->getG() + (0.8909 / k));
						}
						dotsGrabbed[i] = true;
					}
				}
			}

			float mx = pacman->getX();
			float my = pacman->getY();

			if (paused) {
				if (ghostMove) {
					temp = modifier;
				}
				ghostMove = false;
				moving = false;
				modifier = 0;
			}
			if (!paused) {
				if (!ghostMove) {
					modifier = temp;
				}
				ghostMove = true;
				moving = true;
				paused = false;
				temp = 0;
			}

			//**************************************PROGRESS BAR****************************************		(@MM)
			if (survival) {
				progress->setW(progress->getW() + (modifier * rate));
				progress->setR(progress->getR() - (modifier * rate));
				progress->setG(progress->getG() + (modifier * rate));
			}
			if (progress->getX() + progress->getW() >= finishLine->getX() + (finishLine->getW() / 2)) {					// Width = 0.89 for victory
				std::cout << progress->getW() << std::endl;
				victory = true;
				gameRunning = false;
				moveDown = false;
				moveUp = false;
				moveLeft = false;
				moveRight = false;
				paused = true;
				waka->stop();
			}
			//******************************************************************************************

			// Pick up the Power Pellet
			if (pacman->getX() <= (powerPellet->getX() + size / 2) && pacman->getX() >= (powerPellet->getX() - size / 2) &&
				pacman->getY() <= (powerPellet->getY() + size / 2) && pacman->getY() >= (powerPellet->getY() - size / 2) && powerup) {
				powerupUsed = true;
			}
			// Pick up the Speed Boost
			if (pacman->getX() <= (speedBoost->getX() + size / 2) && pacman->getX() >= (speedBoost->getX() - size / 2) &&
				pacman->getY() <= (speedBoost->getY() + size / 2) && pacman->getY() >= (speedBoost->getY() - size / 2) && powerup && boostGrabbed == false) {
				tempSpeed = speed;
				boost = true;
				boostGrabbed = true;
			}
			if (boost) {
				if (boostCounter < 2500) {
					boostCounter += 1;
					if (speed <= 0.001) {
						speed = 2 * speed;
					}
				}
				else if (boostCounter >= 2500) {
					speed = tempSpeed;
					tempSpeed = 0;
					boost = false;
					boostCounter = 0;
				}
			}
			//**************************************PLAYER MOVEMENT*************************************		(@MM)
			if (moving) {
				// Center Pacman in any pathway taken
				if (moveUp && upCmd) {
					if (pacman->getX() >= bX - pathBuff && pacman->getX() <= bX + pathBuff) {
						pacman->setX(bX);
					}
					else if (pacman->getX() >= iX - pathBuff && pacman->getX() <= iX + pathBuff) {
						pacman->setX(iX);
					}
					else if (pacman->getX() >= pX - pathBuff && pacman->getX() <= pX + pathBuff) {
						pacman->setX(pX);
					}
					else if (pacman->getX() >= cX - pathBuff && pacman->getX() <= cX + pathBuff) {
						pacman->setX(cX);
					}
					pacman->setY(my + speed);
					//sB1->setY(sB1->getY() - bgSpeed);
					//sB2->setY(sB2->getY() - bgSpeed);
					if (!soundPlaying) {
						soundPlaying = true;
						waka->play();
					}
				}
				if (moveDown && downCmd) {
					if (pacman->getX() >= bX - pathBuff && pacman->getX() <= bX + pathBuff) {
						pacman->setX(bX);
					}
					else if (pacman->getX() >= iX - pathBuff && pacman->getX() <= iX + pathBuff) {
						pacman->setX(iX);
					}
					else if (pacman->getX() >= pX - pathBuff && pacman->getX() <= pX + pathBuff) {
						pacman->setX(pX);
					}
					else if (pacman->getX() >= cX - pathBuff && pacman->getX() <= cX + pathBuff) {
						pacman->setX(cX);
					}
					pacman->setY(my - speed);
					//sB1->setY(sB1->getY() + bgSpeed);
					//sB2->setY(sB2->getY() + bgSpeed);
					if (!soundPlaying) {
						soundPlaying = true;
						waka->play();
					}
				}
				if (moveLeft && leftCmd) {
					if (pacman->getY() >= bY - pathBuff && pacman->getY() <= bY + pathBuff) {
						pacman->setY(bY);
					}
					else if (pacman->getY() >= pY - pathBuff && pacman->getY() <= pY + pathBuff) {
						pacman->setY(pY);
					}
					else if (pacman->getY() >= iY - pathBuff && pacman->getY() <= iY + pathBuff) {
						pacman->setY(iY);
					}
					else if (pacman->getY() >= cY - pathBuff && pacman->getY() <= cY + pathBuff) {
						pacman->setY(cY);
					}
					else if (pacman->getY() >= cY2 - pathBuff && pacman->getY() <= cY2 + pathBuff) {
						pacman->setY(cY2);
					}
					pacman->setX(mx - speed);
					//sB1->setX(sB1->getX() + bgSpeed);
					//sB2->setX(sB2->getX() + bgSpeed);
					if (!soundPlaying) {
						soundPlaying = true;
						waka->play();
					}
				}
				if (moveRight && rightCmd) {
					if (pacman->getY() >= bY - pathBuff && pacman->getY() <= bY + pathBuff) {
						pacman->setY(bY);
					}
					else if (pacman->getY() >= pY - pathBuff && pacman->getY() <= pY + pathBuff) {
						pacman->setY(pY);
					}
					else if (pacman->getY() >= iY - pathBuff && pacman->getY() <= iY + pathBuff) {
						pacman->setY(iY);
					}
					else if (pacman->getY() >= cY - pathBuff && pacman->getY() <= cY + pathBuff) {
						pacman->setY(cY);
					}
					else if (pacman->getY() >= cY2 - pathBuff && pacman->getY() <= cY2 + pathBuff) {
						pacman->setY(cY2);
					}
					pacman->setX(mx + speed);
					//sB1->setX(sB1->getX() - (bgSpeed));
					//sB2->setX(sB2->getX() - (bgSpeed));
					if (!soundPlaying) {
						soundPlaying = true;
						waka->play();
					}
				}

				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PLAYER COLLISION DETECTION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				// BLOCK DOWNWARD MOVEMENTS
				if (abs((pacman->getY() - pacman->getH()) - (stageBorder2->getY() - stageBorder2->getH())) <= 4 * buffer ||

					abs((pacman->getY() - pacman->getH()) - (pinkColumn->getY())) <= 4 * buffer &&
					pacman->getX() <= (pinkColumn->getX() + pinkColumn->getW()) &&
					pacman->getX() >= (pinkColumn->getX() - pacman->getW()) ||

					pacman->getX() <= (pinkColumn->getX() + pinkColumn->getW()) &&
					pacman->getX() + pacman->getW() >= pinkColumn->getX() &&
					pacman->getY() <= pinkColumn->getY() - pinkColumn->getH() &&
					pacman->getY() - pacman->getH() >= redColumn->getY() ||

					pacman->getX() <= (yellowColumn->getX() + yellowColumn->getW()) &&
					pacman->getX() >= (yellowColumn->getX() - pacman->getW()) ||

					pacman->getX() <= (blueColumn->getX() + blueColumn->getW()) &&
					pacman->getX() >= (blueColumn->getX() - pacman->getW()))
				{
					downCmd = false;
					if (moveDown) {
						pacman->pause();
						moving = false;
					}
				}
				else {
					downCmd = true;
					pacman->resume();
				}
				// BLOCK UPWARD MOVEMENTS
				if (abs(pacman->getY() - stageBorder2->getY()) <= 5 * buffer ||

					abs(pacman->getY() - redColumn->getY() <= 4 * buffer) &&
					pacman->getX() <= (redColumn->getX() + redColumn->getW()) &&
					pacman->getX() >= (redColumn->getX() - pacman->getW()) ||

					pacman->getX() <= (pinkColumn->getX() + pinkColumn->getW()) &&
					pacman->getX() + pacman->getW() >= pinkColumn->getX() &&
					pacman->getY() <= pinkColumn->getY() - pinkColumn->getH() &&
					pacman->getY() - pacman->getH() >= redColumn->getY() ||

					pacman->getX() <= (yellowColumn->getX() + yellowColumn->getW()) &&
					pacman->getX() >= (yellowColumn->getX() - pacman->getW()) ||

					pacman->getX() <= (blueColumn->getX() + blueColumn->getW()) &&
					pacman->getX() >= (blueColumn->getX() - pacman->getW()))
				{
					upCmd = false;
					if (moveUp) {
						pacman->pause();
						moving = false;
					}
				}
				else {
					upCmd = true;
					pacman->resume();
				}
				// BLOCK LEFT MOVEMENTS
				if (abs(pacman->getX() - stageBorder2->getX()) <= 4 * buffer ||

					pacman->getY() >= (blueColumn->getY() - blueColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (blueColumn->getY()) &&
					pacman->getX() >= blueColumn->getX() + blueColumn->getW() ||

					pacman->getY() >= (pinkColumn->getY() - pinkColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (pinkColumn->getY()) &&
					pacman->getX() >= pinkColumn->getX() + pinkColumn->getW() &&
					pacman->getX() <= blueColumn->getX() ||

					pacman->getY() >= (redColumn->getY() - redColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (redColumn->getY()) &&
					pacman->getX() >= redColumn->getX() + redColumn->getW() &&
					pacman->getX() <= blueColumn->getX() ||

					pacman->getY() >= (yellowColumn->getY() - yellowColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (yellowColumn->getY()) &&
					pacman->getX() >= yellowColumn->getX() + yellowColumn->getW() &&
					pacman->getX() + pacman->getW() <= pinkColumn->getX() ||

					pacman->getY() >= (orangeColumn->getY() - orangeColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (orangeColumn->getY()) &&
					pacman->getX() >= orangeColumn->getX() + orangeColumn->getW() &&
					pacman->getX() + pacman->getW() <= pinkColumn->getX() - 2 * buffer)
				{
					leftCmd = false;

				}
				else {
					leftCmd = true;
					pacman->resume();
				}
				// BLOCK RIGHT MOVEMENTS
				if (abs((pacman->getX() + pacman->getW()) - (stageBorder2->getX() + stageBorder2->getW())) <= 4 * buffer ||

					pacman->getY() >= (blueColumn->getY() - blueColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (blueColumn->getY()) &&
					pacman->getX() + pacman->getW() <= blueColumn->getX() &&
					pacman->getX() >= pinkColumn->getX() + pinkColumn->getW() ||

					pacman->getY() >= (pinkColumn->getY() - pinkColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (pinkColumn->getY()) &&
					pacman->getX() + pacman->getW() <= pinkColumn->getX() &&
					pacman->getX() >= orangeColumn->getX() + orangeColumn->getW() ||

					pacman->getY() >= (redColumn->getY() - redColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (redColumn->getY()) &&
					pacman->getX() + pacman->getW() <= redColumn->getX() &&
					pacman->getX() >= yellowColumn->getX() + yellowColumn->getW() ||

					pacman->getY() >= (yellowColumn->getY() - yellowColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (yellowColumn->getY()) &&
					pacman->getX() + pacman->getW() <= yellowColumn->getX() ||

					pacman->getY() >= (orangeColumn->getY() - orangeColumn->getH()) &&
					(pacman->getY() - pacman->getH()) <= (orangeColumn->getY()) &&
					pacman->getX() + pacman->getW() <= orangeColumn->getX())
				{
					rightCmd = false;
					if (moveRight) {
						pacman->pause();
						moving = false;
					}
				}
				else {
					rightCmd = true;
					pacman->resume();
				}
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}
			//******************************************************************************************

			//***************************************GHOST MOVEMENT*************************************		(@BP & MM)
			if (powerupUsed) {
				bDead->setX(blinky->getX());
				bDead->setY(blinky->getY());
				pDead->setX(pinky->getX());
				pDead->setY(pinky->getY());
				iDead->setX(inky->getX());
				iDead->setY(inky->getY());
				cDead->setX(clyde ->getX());
				cDead->setY(clyde->getY());
				bDead->reappear();
				pDead->reappear();
				iDead->reappear();
				cDead->reappear();
				blinky->disappear();
				pinky->disappear();
				inky->disappear();
				clyde->disappear();
				if (ghostTimer <= 5000) {
					ghostTimer += 1;
				}
				else if (ghostTimer >= 5000) {
					blinky->reappear();
					pinky->reappear();
					inky->reappear();
					clyde->reappear();
					bDead->disappear();
					pDead->disappear();
					iDead->disappear();
					cDead->disappear();
					powerupUsed = false;
					ghostTimer = 0;
					powerup = false;
				}
			}

			if (ghostMove) {
				// PINKY
				if (gameRunning) {
					// Move pinky down
					if ((pinky->getX() >= pX - buffer && pinky->getX() <= pX + buffer) &&
						(pinky->getY() >= pY - buffer && pinky->getY() <= pY + buffer)) {
						if (pinkyPos == 1) {
							pinky->setY(pY);
						}
						pinkyPos = 2;
					}
					// Move pinky right
					if ((pinky->getX() >= pX - buffer && pinky->getX() <= pX + buffer) &&
						(pinky->getY() >= bY - buffer && pinky->getY() <= bY + buffer)) {
						if (pinkyPos == 2) {
							pinky->setY(bY);
						}
						pinkyPos = 3;
					}
					// Move pinky up
					if ((pinky->getX() >= iX - buffer && pinky->getX() <= iX + buffer) &&
						(pinky->getY() >= bY - buffer && pinky->getY() <= bY + buffer)) {
						if (pinkyPos == 3) {
							pinky->setX(iX);
						}
						pinkyPos = 4;
					}
					// Move pinky left
					if ((pinky->getX() >= iX - buffer && pinky->getX() <= iX + buffer) &&
						(pinky->getY() >= pY - buffer && pinky->getY() <= pY + buffer)) {
						if (pinkyPos == 4) {
							pinky->setY(pY);
						}
						pinkyPos = 1;
					}

					if (pinkyPos == 1) {
						pinky->setX(pinky->getX() - (ghostSpeed));
					}
					if (pinkyPos == 2) {
						pinky->setY(pinky->getY() - (ghostSpeed));

					}
					if (pinkyPos == 3) {
						pinky->setX(pinky->getX() + (ghostSpeed));
					}
					if (pinkyPos == 4) {
						pinky->setY(pinky->getY() + (ghostSpeed));
					}
				}
				// BLINKY
				if (gameRunning) {
					// Move blinky up
					if ((blinky->getX() >= bX - buffer && blinky->getX() <= bX + buffer) &&
						(blinky->getY() >= bY - buffer && blinky->getY() <= bY + buffer)) {
						if (blinkyPos == 4) {
							blinky->setX(bX);
						}
						blinkyPos = 1;
					}
					// Move blinky left
					if ((blinky->getX() >= bX - buffer && blinky->getX() <= bX + buffer) &&
						(blinky->getY() >= pY - buffer && blinky->getY() <= pY + buffer)) {
						if (blinkyPos == 1) {
							blinky->setY(pY);
						}
						blinkyPos = 2;
					}
					// Move blinky down
					if ((blinky->getX() >= cX - buffer && blinky->getX() <= cX + buffer) &&
						(blinky->getY() >= pY - buffer && blinky->getY() <= pY + buffer)) {
						if (blinkyPos == 2) {
							blinky->setX(cX);
						}
						blinkyPos = 3;
					}
					// Move blinky right
					if ((blinky->getX() >= cX - buffer && blinky->getX() <= cX + buffer) &&
						(blinky->getY() >= bY - buffer && blinky->getY() <= bY + buffer)) {
						if (blinkyPos == 3) {
							blinky->setY(bY);
						}
						blinkyPos = 4;
					}
					if (blinkyPos == 1) {
						blinky->setY(blinky->getY() + (ghostSpeed));
					}
					if (blinkyPos == 2) {
						blinky->setX(blinky->getX() - (ghostSpeed));
					}
					if (blinkyPos == 3) {
						blinky->setY(blinky->getY() - (ghostSpeed));
					}
					if (blinkyPos == 4) {
						blinky->setX(blinky->getX() + (ghostSpeed));
					}
				}
				// INKY
				if (gameRunning) {
					// Move inky down
					if ((inky->getX() >= iX - buffer && inky->getX() <= iX + buffer) &&
						(inky->getY() >= iY - buffer && inky->getY() <= iY + buffer)) {
						if (inkyPos == 4) {
							inky->setX(iX);
						}
						inkyPos = 1;
						std::cout << "Moving up" << std::endl;
					}
					// Move inky right
					if ((inky->getX() >= iX - buffer && inky->getX() <= iX + buffer) &&
						(inky->getY() >= bY - buffer && inky->getY() <= bY + buffer)) {
						if (inkyPos == 1) {
							inky->setY(bY);
						}
						inkyPos = 2;
						std::cout << "Moving down" << std::endl;
					}
					// Move inky up
					if ((inky->getX() >= cX - buffer && inky->getX() <= cX + buffer) &&
						(inky->getY() >= bY - buffer && inky->getY() <= bY + buffer)) {
						if (inkyPos == 2) {
							inky->setX(cX);
						}
						inkyPos = 3;
						std::cout << "Moving down" << std::endl;
					}
					// Move inky left
					if ((inky->getX() >= cX - buffer && inky->getX() <= cX + buffer) &&
						(inky->getY() >= iY - buffer && inky->getY() <= iY + buffer)) {
						if (inkyPos == 3) {
							inky->setY(iY);
						}
						inkyPos = 4;
						std::cout << "Moving down" << std::endl;
					}
					if (inkyPos == 1) {
						inky->setY(inky->getY() - (ghostSpeed));
					}
					if (inkyPos == 2) {
						inky->setX(inky->getX() + (ghostSpeed));
					}
					if (inkyPos == 3) {
						inky->setY(inky->getY() + (ghostSpeed));
					}
					if (inkyPos == 4) {
						inky->setX(inky->getX() - (ghostSpeed));
					}
				}
				// CLYDE
				if (gameRunning) {
					// Move clyde left
					if ((clyde->getX() >= cX - buffer && clyde->getX() <= cX + buffer) &&
						(clyde->getY() >= cY - buffer && clyde->getY() <= cY + buffer)) {
						if (clydePos == 6) {
							clyde->setY(cY);
						}
						clydePos = 1;
					}
					// Move clyde down
					if ((clyde->getX() >= iX - buffer && clyde->getX() <= iX + buffer) &&
						(clyde->getY() >= cY - buffer && clyde->getY() <= cY + buffer)) {
						if (clydePos == 1) {
							clyde->setX(iX);
						}
						clydePos = 2;
					}
					// Move clyde left (2nd time)
					if ((clyde->getX() >= iX - buffer && clyde->getX() <= iX + buffer) &&
						(clyde->getY() >= cY2 - buffer && clyde->getY() <= cY2 + buffer)) {
						if (clydePos == 2) {
							clyde->setY(cY2);
						}
						clydePos = 3;
					}
					// Move clyde up
					if ((clyde->getX() >= pX - buffer && clyde->getX() <= pX + buffer) &&
						(clyde->getY() >= cY2 - buffer && clyde->getY() <= cY2 + buffer)) {
						if (clydePos == 3) {
							clyde->setX(pX);
						}
						clydePos = 4;
					}
					// Move clyde right
					if ((clyde->getX() >= pX - buffer && clyde->getX() <= pX + buffer) &&
						(clyde->getY() >= pY - buffer && clyde->getY() <= pY + buffer)) {
						if (clydePos == 4) {
							clyde->setY(pY);
						}
						clydePos = 5;
					}
					// Move clyde down (2nd time)
					if ((clyde->getX() >= cX - buffer && clyde->getX() <= cX + buffer) &&
						(clyde->getY() >= pY - buffer && clyde->getY() <= pY + buffer)) {
						if (clydePos == 5) {
							clyde->setX(cX);
						}
						clydePos = 6;
					}
					if (clydePos == 1) {
						clyde->setX(clyde->getX() - (ghostSpeed));
					}
					if (clydePos == 2) {
						clyde->setY(clyde->getY() - (ghostSpeed));
					}
					if (clydePos == 3) {
						clyde->setX(clyde->getX() - (ghostSpeed));
					}
					if (clydePos == 4) {
						clyde->setY(clyde->getY() + (ghostSpeed));
					}
					if (clydePos == 5) {
						clyde->setX(clyde->getX() + (ghostSpeed));
					}
					if (clydePos == 6) {
						clyde->setY(clyde->getY() - (ghostSpeed));
					}
				}
			}

			//******************************************************************************************

			//-------------------------------GHOST COLLISION DETECTION----------------------------------		(@MM)
			if (!hit && gameRunning) {
				float ghostPos[4][2];																// Holds the positions of the ghosts
				float bounds = size * 0.75;															// Creates hitbox bounds based on current size of Pacman & ghosts
				ghostPos[0][0] = blinky->getX();
				ghostPos[0][1] = blinky->getY();

				ghostPos[1][0] = pinky->getX();
				ghostPos[1][1] = pinky->getY();

				ghostPos[2][0] = inky->getX();
				ghostPos[2][1] = inky->getY();

				ghostPos[3][0] = clyde->getX();
				ghostPos[3][1] = clyde->getY();

				for (int i = 0; i < 4; i++) {
					if (abs(pacman->getX() - (ghostPos[i][0])) <= bounds && abs(pacman->getY() - (ghostPos[i][1])) <= bounds && !powerupUsed) {
						hit = true;
						gameRunning = false;
					}
				}
			}
			if ((hit && !gameRunning) || fakeDeath) {
				soundLoop = true;
				moving = false;
				spawn = false;
				pacHit->setX(pacman->getX());
				pacHit->setY(pacman->getY());
				pacman->disappear();
				waka->stop();
				pacHit->reappear();
				pacHit->playOnce();
				dead->playOnce();
				soundLoop = false;
				fakeDeath = false;
				gameRunning = false;
				hit = true;
				paused = true;
				if (lives >= 0) {
					lives -= 1;
					gameRunning = true;
					if (lives >= 0) {
						std::cout << "Lives remaining: " << lives << std::endl;
					}
				}
			}
			//------------------------------------------------------------------------------------------
			if (lives < 0) {
				gameRunning = false;
				gameOver = true;
				paused = true;

				// Move the ghosts out of the map so the death animation and sound doesn't keep looping
				blinky->setX(2.0);
				inky->setX(2.0);
				clyde->setX(2.0);
				pinky->setX(2.0);
			}
		}
	}
}

void Game::draw() const {
	if (gameSel) {
		gametype->draw(0.0);
	}
	else if (diffSel) {
		selection->draw(0.0);
		difficulties->draw(0.0);
	}
	else {
		if (!diffSel && !gameOver && !victory) {
			movement->draw(0.5);
			if (easySel) {
				easy->draw(0.5);
			}
			if (normalSel) {
				normal->draw(0.5);
			}
			if (hardSel) {
				hard->draw(0.5);
			}
			if (expertSel) {
				expert->draw(0.5);
			}
			if (classic) {
				// DRAW DOTS (@MM)
				for (int j = 0; j < k; j++) {
					if (dotsGrabbed[j] == false) {
						dots[j].draw();
					}
				}
			}

			// DRAW PROGRESS BAR (@MM)
			finishLine->draw();
			progress->draw();

			// DRAW STAGE (@DB)
			yellowColumn->draw();
			pinkColumn->draw();
			redColumn->draw();
			blueColumn->draw();
			orangeColumn->draw();
			stageBorder1->draw();
			stageBorder2->draw();

			// DRAW PACMAN AND GHOSTS (@MM)
			if (!hit && gameRunning) {
				pacman->draw(0.5);
			}
			pacHit->draw(0.5);
			blinky->draw(0.6);
			bDead->draw(0.6);
			pinky->draw(0.7);
			pDead->draw(0.7);
			inky->draw(0.8);
			iDead->draw(0.8);
			clyde->draw(0.9);
			cDead->draw(0.9);

			// DRAW POWERUPS
			if (!powerupUsed && powerup) {
				powerPellet->draw(0.5);
			}
			if (!boostGrabbed) {
				speedBoost->draw(0.5);
			}

			if (!spawn && !soundLoop) {
				respawn->draw(1.0);
				spacebar->draw(1.0);
			}
			// DRAW LIVES (@MM)
			if (lives == 3) {
				life1->draw(1.0);
			}
			if (lives >= 2) {
				life2->draw(1.0);
			}
			if (lives >= 1) {
				life3->draw(1.0);
			}
		}

		// DRAW GAME OVER SCREEN (@MM)
		if (gameOver && (easySel || normalSel || hardSel || expertSel)) {
			lost->draw(1.0);
			spacebar->draw(1.0);
			pacHit->draw(0.5);
		}
		if (victory && (easySel || normalSel || hardSel || expertSel)) {
			won->draw(1.0);
			spacebar->draw(1.0);
			pacman->draw(0.5);
		}
	}
}

void Game::handleKeyDown(unsigned char key, float x, float y){
	if (gameSel) {
		if (key == '1') {
			classic = true;
		}
		else if (key == '2') {
			survival = true;
		}
	}
	if (diffSel) {
		if (key == '1') {
			easySel = true;
		}
		else if (key == '2') {
			normalSel = true;
		}
		else if (key == '3') {
			hardSel = true;
		}
		else if (key == '4') {
			expertSel = true;
		}
	}
	else  if (!diffSel) {
		if (key == 'p') {
			paused = true;
		}
		else if (key == 'r') {
			paused = false;
		}
		if (!hit && !gameOver || !hit && !victory) {
			if (key == 'w' && upCmd) {
				std::cout << "Moving up" << std::endl;
				pacman->up();
				moving = true;
				moveUp = true;
				moveLeft = false;
				moveRight = false;
				moveDown = false;
				if (!soundPlaying) {
					soundPlaying = true;
					waka->play();
				}
			}
			if (key == 'a' && leftCmd) {
				std::cout << "Moving left" << std::endl;
				pacman->left();
				moving = true;
				moveLeft = true;
				moveUp = false;
				moveRight = false;
				moveDown = false;
				if (!soundPlaying) {
					soundPlaying = true;
					waka->play();
				}
			}
			if (key == 'd' && rightCmd) {
				std::cout << "Moving right" << std::endl;
				pacman->right();
				moving = true;
				moveRight = true;
				moveUp = false;
				moveLeft = false;
				moveDown = false;
				if (!soundPlaying) {
					soundPlaying = true;
					waka->play();
				}
			}
			if (key == 's' && downCmd) {
				std::cout << "Moving down" << std::endl;
				pacman->down();
				moving = true;
				moveDown = true;
				moveUp = false;
				moveLeft = false;
				moveRight = false;
				if (!soundPlaying) {
					soundPlaying = true;
					waka->play();
				}
			}
			if (key == ']') {
				if (speed <= 0.01 - 0.0001) {
					speed += 0.0001;
				}
			}
			if (key == '[') {
				if (speed >= 0.0001) {
					speed -= 0.0001;
				}
			}
			if (key == ' ') {
				moving = false;
				soundPlaying = false;
				waka->stop();
			}
			if (key == 'k') {
				fakeDeath = true;
			}
		}
		else if (hit && !gameOver && !soundLoop || hit && !victory && !soundLoop) {
			if (key == ' ') {
				moving = false;
				moveUp = false;
				moveLeft = false;
				moveRight = false;
				moveDown = false;
				gameOver = false;
				victory = false;
				waka->stop();
				hit = false;
				gameRunning = true;
				upCmd = true;
				leftCmd = true;
				rightCmd = true;
				downCmd = true;
				spawn = true;
				if (survival) {
					if (!easySel) {
						progress->setW(0);
					}
				}
				pacman->setX(spawnX);
				pacman->setY(spawnY);
				blinky->setX(bX);
				blinky->setY(bY);
				inky->setX(iX);
				inky->setY(iY);
				pinky->setX(pX);
				pinky->setY(pY);
				clyde->setX(cX);
				clyde->setY(cY);
				pacHit->disappear();
				pacman->reappear();
				if (!easySel && survival) {
					progress->setR(1.0);
					progress->setG(0.0);
				}
				paused = false;
			}
		}
		if ((lives < 0 && gameOver) || victory) {
			if (key == ' ') {
				classic = false;
				survival = false;
				easySel = false;
				normalSel = false;
				hardSel = false;
				expertSel = false;
				diffSel = false;
				gameSel = true;
			}
		}
	}
}

Game::~Game(){
    stop();
	// DELETE PROMPT SCREENS
	delete gametype;
	delete selection;
	delete difficulties;
	delete respawn;
	delete spacebar;
	delete progress;
	delete finishLine;
	delete lost;
	delete won;
	delete restart;
	delete easy;
	delete normal;
	delete hard;
	delete expert;
	delete movement;
	delete powerPellet;

	// DELETE PACMAN RELATED OBJECTS
	delete pacman;
	delete pacHit;
	delete waka;
	delete dead;
	delete life1;
	delete life2;
	delete life3;

	// DELETE GHOST OBJECTS
	delete pinky;
	delete blinky;
	delete inky;
	delete clyde;

	// DELETE STAGE PARTS
	delete stageBorder1;
	delete stageBorder2;
	delete yellowColumn;
	delete pinkColumn;
	delete redColumn;
	delete blueColumn;
	delete orangeColumn;
}
