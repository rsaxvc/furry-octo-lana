#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdio>

#include "grid.h"

#define glError() { \
	GLenum err = glGetError(); \
	while (err != GL_NO_ERROR) { \
		fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err = glGetError(); \
	} \
}

void grid_draw()
{
glBegin( GL_LINES );

for( int y = 0; y <= GRID_H; ++y )
	{
	GLfloat v0[] = { 0.0, (float)y, 0.0f };
	glVertex3fv( v0 );

	GLfloat v1[] = { GRID_W, (float)y, 0.0f };
	glVertex3fv( v1 );
	}

for( int x = 0; x <= GRID_W; ++x )
	{
	GLfloat v0[] = { (float)x, 0.0f, 0.0f };
	glVertex3fv( v0 );

	GLfloat v1[] = { (float)x, GRID_H, 0.0f };
	glVertex3fv( v1 );
	}

glEnd();
glError();
}
