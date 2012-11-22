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

#define SPEED 1.0

float bullet::getTopSpeed()
{
return SPEED;
}

bullet::bullet(const position & start, const position & end )
{
start_pos = start;
pos = start;

vel.dx = cos( atan2( start.y - end.y, start.x - end.x ) ) * SPEED;
vel.dy = -sin( atan2( start.y - end.y, start.x - end.x ) ) * SPEED;
pos_end = end;
}

bool bullet::target_reached( void )
{
return start_pos.normalized_dot_product3( pos_end, pos ) > 1.0;
}

bullet::~bullet()
{
}
