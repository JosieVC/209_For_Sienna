#pragma once
#include <Box2D/Box2D.h>

float Update(b2World* myworld, float timer, float timestep);
void Display(b2Vec2 target, b2Body* player);
char ApplyForces(unsigned char key, b2Body* player, char lastKey);
b2Vec2 MoveTarget();
