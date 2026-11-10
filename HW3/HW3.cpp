#include <iostream>
#include <GravityGame.h>
#include <iomanip>
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Functions.h"
#include <conio.h>
using namespace std;

/* GravityGame.h is the header file for a dynamically linked library (dll) called
* GravityGameDLL.dll. The GravityGame library is a set of functions that satisfy the
* requirements of HW2 - Gravity Game. In order to allow you to complete HW3 without
* the need for a done version of HW2, this DLL has been created. Using it's functions,
* you can implement HW3 without the HW2 code. A full breakdown of it's functions
* can be found in the homework document.
*/

//MY SPECIAL CHANGES/IMPLEMENTATIONS
//Special Scoring System: I re-used my special scoring system from HW2, that being counting the number of key presses. I did have to modify
//one of the DLL files for this, but I genuinely couldn't conceive of any non-time-based scoring system that wouldn't require me to do that
//to some degree, but I've tried to keep it as non-invasive as possible
//Fun Feature: I added a state machine based menu system that displays instructions at the start of the game and displays the score once
//the player wins the game. I added this because I felt it would tie in nicely and help with game flow, and I've set up a custom enum to handle it

//EXTRA INFO:
//I am aware of the fact that coordinates are not translated properly between box2d and sfml - I made a genuine attempt to try to get this
//to work, which I have documented below. Between having to travel across the country, the errors with my PCs file system, and the death in the family, I've just been pretty 
//overwhelmed lately and coordinate math really isn't my strongsuit, so I couldn't get it fully operational. I did still give it some genuine
//effort, so hopefully it's clear I'm not trying to avoid doing work. I absolutely understand if I can't get the full amount of points,
//but hopefully my efforts at least count for something

//in order to draw things properly, we need to set up a ratio to convert box2ds meter units to drawable pixels - I saw that 
//30 tends to be a fairly standard conversion so I'm using that
const float SCALE_RATIO = 30.0f;

//enum to control different game states
enum GameState {
	Menu,
	Game,
	GameOver
};

int main()
{
	//Improves formatting of console output.
	cout << fixed << showpoint << setprecision(2);

	//Sets the possible bounds of where the target can spawn (-4, -4) to (4, 4) here.
	//This function is from the DLL.
	setTargetBounds(-4.0f, 4.0f, -4.0f, 4.0f);

	//Create the physics world, and all the objects.
	//These functions are from the DLL.
	initVariables(b2Vec2(0.0f, -9.8f));
	//convert from pixel coords here so that the sprites can match up with the box2d shapes
	createFloor(b2Vec2(0.0f*SCALE_RATIO, -6.0f*SCALE_RATIO), 10.0f*SCALE_RATIO, 1.0f*SCALE_RATIO);
	createCeiling(b2Vec2(0.0f*SCALE_RATIO, 6.0f*SCALE_RATIO), 10.0f*SCALE_RATIO, 1.0f*SCALE_RATIO);
	createLeftWall(b2Vec2(-6.0f*SCALE_RATIO, 0.0f*SCALE_RATIO), 1.0f*SCALE_RATIO, 10.0f*SCALE_RATIO);
	createRightWall(b2Vec2(6.0f*SCALE_RATIO, 0.0f*SCALE_RATIO), 1.0f*SCALE_RATIO, 10.0f*SCALE_RATIO);

	//Create the player in the physics world. Return a pointer.
	b2Body* player = createPlayer(b2Vec2(0.0f*(SCALE_RATIO/2), 5.0f*(SCALE_RATIO/2)), 0.5f*(SCALE_RATIO/2), 0.5f*(SCALE_RATIO/2));
	//I need to get the location of the target to actually draw it later
	b2Vec2 targetLoc = getTargetPosition(); 

	//just some simple variables to track player action
	int targetsCollected = 0;
	int keyPresses = 0;

	//create basic sprites for all of the shapes that need to be rendered
	//adjust all of their positions to account for Box2d and SFML coordinate conversions
	//Using hard-coded values here because I can't actually access the wall objects since they are declared in a different function
	//Also make sure to change the origins of the shapes to account for the difference in coordinates
	//this can be done by moving the origin to a location half of the shape's width and height
	//Basically, the way I was converting between the two is to get them set to the same scale ratio, so they appear to be the same size
	//Change the origin when drawing so they're drawn at the same position
	//And flip the y value where needed to account for the differences in the way that's handled
	sf::RenderWindow window(sf::VideoMode(800, 800), "Gravity Game");
	sf::RectangleShape floor({ 10.0f * SCALE_RATIO, 1.0f * SCALE_RATIO });
	floor.setPosition(0,275); 
	floor.setOrigin(10, 1);
	sf::RectangleShape ceiling({ 10.0f * SCALE_RATIO, 1.0f * SCALE_RATIO });
	ceiling.setPosition(0,0);
	ceiling.setOrigin(10, 1);
	sf::RectangleShape leftWall({ 1.0f * SCALE_RATIO, 10.0f * SCALE_RATIO });
	sf::RectangleShape rightWall({ 1.0f * SCALE_RATIO, 10.0f * SCALE_RATIO }); 
	leftWall.setPosition(0,0);
	leftWall.setOrigin(1, 10);
	rightWall.setPosition(275,0); 
	rightWall.setOrigin(1, 10); 

	//create sprites for player and target
	//don't set position yet because these are subject to change
	//we will move the player though
	//I'm halving the scale ratio here because it was a bit too large otherwise - I also modified it in the
	//player declaration so it's consistent
	sf::CircleShape playerObj({ 1.0f*SCALE_RATIO/2 }); 
	playerObj.setFillColor(sf::Color::Green);
	playerObj.setOrigin(0.5f, 0.5f); 
	sf::CircleShape target({ 1.0f*SCALE_RATIO/2 }); 
	target.setFillColor(sf::Color::Red); 

	//sets up the font & text object for use later
	sf::Font font;
	//loads a relative pathway to the file
	font.loadFromFile("Debug\\arial.ttf");
	sf::Text myText;
	myText.setFont(font); 
	sf::Text scoreDisplay;
	scoreDisplay.setFont(font);

	//game always starts in the menu state 
	GameState currentState = Menu; 

	while (window.isOpen()) {

		//always be checking this so as to allow people to exit the game
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//change what gets displayed based on the game state
		//in the menu state, display the instructions
		if (currentState == Menu) {
			window.clear(sf::Color::White);
			myText.setString(" Welcome to the gravity game!\n Use the WASD Keys to move your player \n and try to collect the target!\n Collect the target twice to win! Press Enter to start");
			myText.setFillColor(sf::Color::Black); 
			window.draw(myText);
			window.display();

			//check for keyboard input from the user to determine when to start the game
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				currentState = Game;
			}
		}
		if(currentState == Game){
			// clear the window with black color
			window.clear(sf::Color::Black);
			//we always want to show the walls and target, so we'll draw them regardless of if the frame is a physicsFrame or not
			window.draw(floor);
			window.draw(ceiling);
			window.draw(leftWall);
			window.draw(rightWall);
			//make sure to update the target's location
			//we can't use negative values in SFML, so if the value is negative, flip it
			if (targetLoc.x < 0) {
				targetLoc.x = -targetLoc.x; 
			}
			target.setPosition(SCALE_RATIO * targetLoc.x, -(SCALE_RATIO * targetLoc.y));
			window.draw(target);

			if (targetsCollected < 2)
			{
				//Process input and apply forces to the player.
				//This function is in Functions.h.
				keyPresses = handleInput(keyPresses);

				//This function advances the physics world every fixed timestep (1/60th of a second)
				//This function is from the DLL.
				bool physicsFrame = updateWorld();

				//Display the current player position. Only displays on
				//physics frames, since those are the only ones where things move.
				if (physicsFrame)
				{
					//make sure to convert the player's position to SFML coords
					//again, we can't have negative y coords, so correct them, and make sure to translate by the ratio
					playerObj.setPosition(SCALE_RATIO*player->GetPosition().x, -(SCALE_RATIO*player->GetPosition().y));
					//the only object that will actually be moving is the player, so we'll draw them when they move
					window.draw(playerObj);

					// end the current frame
					window.display();
				}

				//This function checks if the player is close to the target. 
				//It takes in a maximum collision distance, and assumes bott the target and player are CIRCLES.
				//This function is from the DLL.
				if (checkCollisionAndMoveTarget(0.5f))
				{
					targetsCollected++;
					//update targetLoc variable with new position 
					targetLoc = getTargetPosition();

					//if collecting the target would have you win the game, transition to the game over screen
					if (targetsCollected == 2) {
						currentState = GameOver;
					}
				}
				//if it isn't a physics frame, this will still display the walls
				window.display();
			}
		//game over screen displays the player's score
			if (currentState == GameOver) {
				window.clear(sf::Color::White);
				myText.setString("Congratulations! You beat the game with this many key presses: ");
				scoreDisplay.setString(to_string(keyPresses));
				//we want to make sure the score displays lower than the main message
				scoreDisplay.setPosition(50, 50);
				window.draw(myText);
				window.draw(scoreDisplay);
				window.display();
			}
		}

		
	}

	releaseVariables();
	}
	//release variables at the end
