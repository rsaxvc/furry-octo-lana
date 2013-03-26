#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "tank.h"
#include "game.h"

#define glError() { \
	GLenum err = glGetError(); \
	while (err != GL_NO_ERROR) { \
		fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err = glGetError(); \
	} \
}

#define SPEED 1

float tank::getTopSpeed()
{
return SPEED;
}

tank::tank(const position & p)
{
vel.dx = (float)(rand()%5)/5 - .5;
vel.dy = -.5;

start_pos = p;
pos = p;
}

tank::~tank()
{
}

void tank::calcState()
{
int r;
last_pos = pos;

pos.x += vel.dx * PHYSICS_TIMESTEP;
pos.y += vel.dy * PHYSICS_TIMESTEP;

r = rand();
if( r&1 )
	{
	vel.dx = r&2?-1:1;
	vel.dy = 0;
	}
else
	{
	vel.dx = 0;
	vel.dy = r&2?-1:1;
	}
}
