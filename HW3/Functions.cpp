#include "Functions.h"
#include <iostream>
#include <GravityGame.h>
using namespace std;

void display(b2Body* player)
{
	cout << "Target: (" << getTargetPosition().x << ", " << getTargetPosition().y << ")  ";
	cout << "Player: (" << player->GetPosition().x << ", " << player->GetPosition().y << ")  " << endl;
}

//I had to modify this a bit to reimplement my special scoring system from HW2 - hopefully this is all ok, I couldn't think of 
//any non-time-based scoring system that wouldn't requrie some modification of the base functions
int handleInput(int keyPresses)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		keyPresses++;
		applyForceToPlayer(b2Vec2(0.0f, 20.0f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		keyPresses++;
		applyForceToPlayer(b2Vec2(-20.0f, 0.0f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		keyPresses++;
		applyForceToPlayer(b2Vec2(20.0f, 0.0f));
	}

	return keyPresses; 
}
