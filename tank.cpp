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

#define SPEED 5.0f

float tank::getTopSpeed()
{
return SPEED;
}

tank::tank(const position & p)
{
vel.dx = 0.0f;
vel.dy = 0.0f;

heading = 0.0f;

start_pos = p;
pos = p;
}

tank::~tank()
{
}

void tank::calcState()
{
int r;

r = rand();
if( r&1 )
	{
	vel.dx = r&2?-SPEED:SPEED;
	}
else
	{
	vel.dy = r&2?-SPEED:SPEED;
	}

if( r&4 )
	{
	if( r&8 )
		vel.dx = 0;
	else
		vel.dy = 0;
	}

entity::calcState();
}
