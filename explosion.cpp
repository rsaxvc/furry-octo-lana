#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "explosion.h"
#include "game.h"

#include "time.h"

#define glError() { \
	GLenum err = glGetError(); \
	while (err != GL_NO_ERROR) { \
		fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err = glGetError(); \
	} \
}

explosion::explosion(const position & p)
{
vel.dx = 0;
vel.dy = 0;

pos = p;
start_time = get_time();
}

void explosion::calcState( void )
{
#define SPEED 2
radius = ( get_time() - start_time ) * SPEED;
}

#define LIFETIME 2
bool explosion::complete( void )
{
return get_time() - start_time > LIFETIME;
}

explosion::~explosion()
{
}
