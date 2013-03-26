#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstring>

#include <stddef.h>

#include "draw_state.h"

#include <GL/gl.h>
#include <GL/glu.h>

#define glError() { \
    GLenum err = glGetError(); \
    while (err != GL_NO_ERROR) { \
        fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
        err = glGetError(); \
    } \
}

static void draw_bullet( const bullet_state & state, double dtime )
{
position f( state.pos, state.vel, dtime );
#define LEN .2f
#define line( _pt1,_pt2 ) glVertex3fv( _pt1 );glVertex3fv( _pt2 )
GLfloat v0[] = { f.x-LEN, f.y-LEN, 0.0f };
GLfloat v1[] = { f.x+LEN, f.y+LEN, 0.0f };
GLfloat v2[] = { f.x-LEN, f.y+LEN, 0.0f };
GLfloat v3[] = { f.x+LEN, f.y-LEN, 0.0f };

line( v0, v1 );
line( v1, v2 );
line( v2, v3 );
line( v3, v0 );

GLfloat v4[] = {  f.x,  f.y, 0.0f };
GLfloat v6[] = { state.start_pos.x, state.start_pos.y, 0.0f };
line( v4, v6 );

#undef LEN
#undef line
}


static void draw_tank( const tank_state & state, double dtime )
{
position f( state.pos, state.vel, dtime );

#define LEN .4f
#define line( _pt1,_pt2 ) glVertex3fv( _pt1 );glVertex3fv( _pt2 )

GLfloat v0[] = { f.x+LEN, f.y+LEN, f.z+LEN };
GLfloat v1[] = { f.x-LEN, f.y+LEN, f.z+LEN };
GLfloat v2[] = { f.x-LEN, f.y-LEN, f.z+LEN };
GLfloat v3[] = { f.x+LEN, f.y-LEN, f.z+LEN };

GLfloat v4[] = { f.x+LEN, f.y+LEN, f.z-LEN };
GLfloat v5[] = { f.x-LEN, f.y+LEN, f.z-LEN };
GLfloat v6[] = { f.x-LEN, f.y-LEN, f.z-LEN };
GLfloat v7[] = { f.x+LEN, f.y-LEN, f.z-LEN };

line( v0, v1 );
line( v1, v2 );
line( v2, v3 );
line( v3, v0 );

line( v4, v5 );
line( v5, v6 );
line( v6, v7 );
line( v7, v4 );

line( v0, v4 );
line( v1, v5 );
line( v2, v6 );
line( v3, v7 );

#undef LEN
#undef line
}

static void draw_explosion( const explosion_state & state, float dtime )
{
#define line( _pt1,_pt2 ) glVertex3fv( _pt1 );glVertex3fv( _pt2 )
float r = state.r + dtime * state.dr;
float dAng= M_PI/20;
float spin = dtime * M_PI * 2 / 4;

GLfloat center[] = { state.center.x, state.center.y, 0.0f };

for( float ang = 0.0f; ang < M_PI*2 + dAng; ang += dAng )
	{
	GLfloat vprev[] =
		{
		state.center.x + r * cosf( ang + spin - dAng ),
		state.center.y + r * sinf( ang + spin - dAng ),
		0.0f };
	GLfloat vedge[] =
		{
		state.center.x + r * cosf( ang + spin ),
		state.center.y + r * sinf( ang + spin ),
		0.0f };
	line( center, vedge );
	line( vprev, vedge );
	memcpy( vprev, vedge, sizeof( vprev ) );
	assert( sizeof( vprev ) == sizeof( vedge ) );
	}
#undef line
}

void draw( const draw_state & state, double dtime )
{
glPushMatrix();
glError();

glBegin( GL_LINES );

for( size_t i = 0; i < state.tanks.size(); ++i )
	{
	draw_tank( state.tanks[i], dtime );
	}

for( size_t i = 0; i < state.bullets.size(); ++i )
	{
	draw_bullet( state.bullets[i], dtime );
	}

for( size_t i = 0; i < state.explosions.size(); ++i )
	{
	draw_explosion( state.explosions[i], dtime );
	}

glEnd();
glError();

glPopMatrix();
glError();
}

