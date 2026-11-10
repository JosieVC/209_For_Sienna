#include <Box2D/Box2D.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <conio.h>
using namespace std;

//updates the world after a certain amount of time has passed
float Update(b2World* myworld, float timer, float timestep) {
    //check to see if enough time has passed
    if (timer >= timestep)
    {
        //if so, update the world
        myworld->Step(timestep, 6, 2);
        //decrease timer so as to reset the counter
        timer -= timestep;
    }

    //returns the timer so that the timer can be updated in the main function
    return timer;
}

//displays the target's and player's positions to the console
void Display(b2Vec2 target, b2Body* player) {
    b2Vec2 playerPos = player->GetPosition();

    cout << "Target" << target.x << ", " << target.y << "-->";
    cout << "Player" << playerPos.x << ", " << playerPos.y << endl;

}

//Applies appropriate forces to the player based on key press
char ApplyForces(unsigned char key, b2Body* player, char lastKey) {
    //start by getting the velocity of the player to modify
    b2Vec2 velocity = player->GetLinearVelocity();
    //store values for desired velocities to update
    float desiredVeloX = 0;
    float desiredVeloY = 0;
    //max speed for the player to move at
    float speed = 5.0f;

    //if a key is pressed, move the player left
    if (key == 97) {
        desiredVeloX = -speed;
        desiredVeloY = 0;
        lastKey = 97;
    }
    //if d key is pressed, move the player right
    else if(key == 100){
        desiredVeloX = speed;
        desiredVeloY = 0;
        lastKey = 100;
    }
    //if j key is pressed, massively increase y velocity (simulating a jump) - but only do so once before a cooldown occurs
    if (key == 106 && lastKey != 106) {
        desiredVeloY = 20.0f;
        lastKey = 106; 
    }
    //if w key is pressed, move the player up
    if (key == 119) {
        desiredVeloX = 0;
        desiredVeloY = speed;
        lastKey = 119;
    }
    //if no key is pressed, keep velocity stagnant
    else {
        desiredVeloX = 0.0f; 
        desiredVeloY = 0;
        lastKey = 0;
    }

    //update the player's velocity appropriately
    velocity.x = desiredVeloX; 
    velocity.y = desiredVeloY; 
    player->SetLinearVelocity(velocity); 

    //return the last key pressed to facilitate the jump delay
    return lastKey; 
}

//selects a random position for the target to be moved to
b2Vec2 MoveTarget() {
    //this generates a random number between 1 and 10 and then subtracts 5, so that the lowest number can be -5 and the highest 5
    float randomX = ((rand() % 10) + 1) - 5;
    float randomY = ((rand() % 10) + 1) - 5;

    //return the vector to update the target in main
    return b2Vec2(randomX, randomY); 
}