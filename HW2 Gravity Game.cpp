
//all necessary inclusions
#include <iostream>
#include<Box2D/Box2D.h>
#include <chrono>
#include <iomanip>
#include"game.h"
#include <conio.h>
using namespace std;



int main()
{
    //start by creating the world, initialize it with gravity
    b2Vec2 gravity(0.0f, -9.8f);
    b2World* world = new b2World(gravity);

    //create a body for the player - make them dynamic for movement purposes
    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    //placed at the origin for simplicity
    playerBodyDef.position.Set(0.0f, 0.0f);
    b2Body* playerBody = world->CreateBody(&playerBodyDef);

    //box for player bounds
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f); 

    //create a fixture for the player so they can interact with the world
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    playerBody->CreateFixture(&fixtureDef); 

    //height and width of play area for walls initialization
    float width = 20.0f;
    float height = 20.0f;

    //create a body for the walls
    b2BodyDef wallsDef;
    wallsDef.position.Set(0.0f, 0.0f); 
    b2Body* wallsBody = world->CreateBody(&wallsDef); 

    //create a shape and fixture for the walls
    b2PolygonShape wallShape;
    b2FixtureDef wallFixturesDef;
    wallFixturesDef.shape = &wallShape;
    //friction added for a more interesting simulation
    wallFixturesDef.friction = 1.0f;

    //set up each of the walls to bound the play area

    //floor
    wallShape.SetAsBox(width / 2.0f, 1.0f, b2Vec2(0.0f, -height / 2), 0.0f); 
    wallsBody->CreateFixture(&wallFixturesDef); 

    //ceiling
    wallShape.SetAsBox(width / 2.0f, 1.0f, b2Vec2(0.0f, height / 2), 0.0f);
    wallsBody->CreateFixture(&wallFixturesDef);

    //left wall
    wallShape.SetAsBox(1.0f, height / 2.0f, b2Vec2(-width / 2.0f, 0.0f), 0.0f);
    wallsBody->CreateFixture(&wallFixturesDef);

    //right wall
    wallShape.SetAsBox(1.0f, height / 2.0f, b2Vec2(width / 2.0f, 0.0f), 0.0f);
    wallsBody->CreateFixture(&wallFixturesDef);

    //create a vector to represent the target
    b2Vec2 target(1.5f, 1.2f); 

    //initialize the timer and get it all set up for the update function later
    float timer = 0;
    const float timeStep = 1.0f / 60.0f;
    std::chrono::high_resolution_clock clock;
    std::chrono::steady_clock::time_point previousTime = clock.now();
    auto currentTime = clock.now();

    //checks to see if player has pressed escape
    bool isESCPressed = false;
    //stores the amount of targets the player has hit
    int targetsHit = 0;
    //my special scoring system - rates the player based on how many key presses it took them to reach the target (rewards efficiency)
    int keyPresses = 0;
    //stores the last key pressed to prevent the jump button from being held down constantly
    //NOTE: I didn't include ground collision checking here because the homework writeup made the jump option seem more like a temporary but
    //large increase in y velocity, not necessarily something you could only use while grounded. So, I used this jump delay to still apply a cooldown,
    //but not require the player's y position to completely reset to floor level before it could be used again. (Especially since the constant upwards
    //option exists, I figured this was what was intended)
    char jumpDelay = 0;

    cout << "Hello and welcome to the Gravity Game! Move the player left and right with the a and d keys, and up with the w key. Use the j key";
    cout << " to perform a jump! Try to catch the target twice to win the game. Try to aim for as few key presses as possible to complete your goal." << endl;

    //so long as the player hasn't chosen to exit and they haven't hit both targets, keep playing
    while (!isESCPressed && targetsHit < 2)
    {
        //update the clock - I did this here instead of in Update so as to save myself a million function parameters
        previousTime = currentTime;
        currentTime = clock.now();
        auto deltaSeconds = std::chrono::duration_cast<
            std::chrono::duration<float>>
            (currentTime - previousTime);

        float deltaTime = deltaSeconds.count();

        timer += deltaTime;

        //call update to update the world
        timer = Update(world, timer, timeStep); 

        //display the target and player positions
        Display(target, playerBody); 

        //check to see if a key was pressed - this will allow the program to run constantly without waiting on a key input
        //if you wanna test with it waiting on a key input, just remove the () from kbhit and it will run frame by frame (I had 
        //to do this while testing so as not to kill my PC)
        if (_kbhit()) {
            //if there was, get the key pressed
            char key = _getch();
            //if the key was escape, end the game
            if (key == 27) {
                isESCPressed = true; 
            }
            //otherwise increase key presses and apply appropriate force to the player body
            else {
                keyPresses++; 
                jumpDelay = ApplyForces(key, playerBody, jumpDelay);
            }
        }

        //gets position of the player to check against target
        b2Vec2 playerPos = playerBody->GetPosition();

        //simple distance calc to see how close player and target are
        float distance = (playerPos - target).Length();

        //if player is sufficiently close to target, increase hit count and relocate target
        if (distance < 0.5f) {
            targetsHit++;
            cout << "Hit the target!" << endl;
            b2Vec2 newTargetPos = MoveTarget(); 
            target.Set(newTargetPos.x, newTargetPos.y); 
        }
    }

    cout << "Congratulations! You beat the gravity game! You took " << keyPresses << " total key presses to catch both targets!";

    //make sure to delete the world to prevent memory leaks
    delete world;
    world = nullptr;


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
