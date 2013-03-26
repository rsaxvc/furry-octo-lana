#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "bullet.h"
#include "game.h"

#define glError() { \
	GLenum err = glGetError(); \
	while (err != GL_NO_ERROR) { \
		fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err = glGetError(); \
	} \
}

#define SPEED 55.0

float bullet::getTopSpeed()const
{
return SPEED;
}

float bullet::getRadius()const
{
return 1.0f;
}

bullet::bullet(const position & start, float heading )
{
start_pos = start;
pos = start;

vel.dx = -sin( heading ) * SPEED;
vel.dy =  cos( heading ) * SPEED;
}

bullet::~bullet()
{
}
