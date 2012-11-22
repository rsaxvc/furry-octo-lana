#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "missile.h"
#include "game.h"

#define glError() { \
	GLenum err = glGetError(); \
	while (err != GL_NO_ERROR) { \
		fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err = glGetError(); \
	} \
}

#define SPEED 1

float missile::getTopSpeed()
{
return SPEED;
}

missile::missile(const position & p)
{
vel.dx = (float)(rand()%5)/5 - .5;
vel.dy = -.5;

pos = p;
}

missile::~missile()
{
}
