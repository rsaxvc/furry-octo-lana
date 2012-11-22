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

static void draw_bullet( const bullet_state & state, double time_fract )
{
position f( state.last_pos, state.next_pos, time_fract );
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

position f2( state.last_pos, state.next_pos, time_fract + .3 );
GLfloat v4[] = {  f.x,  f.y, 0.0f };
GLfloat v5[] = { f2.x, f2.y, 0.0f };
line( v4, v5 );

GLfloat v6[] = { state.start_pos.x, state.start_pos.y, 0.0f };
line( v4, v6 );

#undef LEN
#undef line
}


static void draw_missile( const missile_state & state, double time_fract )
{
position f( state.last_pos, state.next_pos, time_fract );

#define LEN .4f
#define line( _pt1,_pt2 ) glVertex3fv( _pt1 );glVertex3fv( _pt2 )
GLfloat v0[] = { f.x-LEN, f.y-LEN, 0.0f };
GLfloat v1[] = { f.x+LEN, f.y+LEN, 0.0f };
GLfloat v2[] = { f.x-LEN, f.y+LEN, 0.0f };
GLfloat v3[] = { f.x+LEN, f.y-LEN, 0.0f };

line( v0, v1 );
line( v1, v2 );
line( v2, v3 );
line( v3, v0 );

position f2( state.last_pos, state.next_pos, time_fract + .3 );
GLfloat v4[] = {  f.x,  f.y, 0.0f };
GLfloat v5[] = { f2.x, f2.y, 0.0f };
line( v4, v5 );

GLfloat v6[] = { state.start_pos.x, state.start_pos.y, 0.0f };
line( v4, v6 );

#undef LEN
#undef line
}

static void draw_explosion( const explosion_state & state, float time_fract )
{
#define line( _pt1,_pt2 ) glVertex3fv( _pt1 );glVertex3fv( _pt2 )
float r = state.r + time_fract * state.dr;
float dAng= M_PI/20;

GLfloat center[] = { state.center.x, state.center.y, 0.0f };
GLfloat vprev[] = { state.center.x+r, state.center.y, 0.0f };

for( float ang = 0.0f; ang < M_PI*2 + dAng; ang += dAng )
	{
	GLfloat vedge[] =
		{
		state.center.x + r * cosf( ang ),
		state.center.y + r * sinf( ang ),
		0.0f };
	line( center, vedge );
	line( vprev, vedge );
	memcpy( vprev, vedge, sizeof( vprev ) );
	assert( sizeof( vprev ) == sizeof( vedge ) );
	}
#undef line
}

static void draw_building( const building_state & state )
{
float bottom = state.bottom_left.y;
float left = state.bottom_left.x;
float right = state.top_right.x;
float top = bottom + ( state.top_right.y - bottom ) * state.health / 100.0f;

#define line( _pt1,_pt2 ) glVertex3fv( _pt1 );glVertex3fv( _pt2 )
GLfloat v0[] = { top, left, 0.0f };
GLfloat v1[] = { top, right, 0.0f };
GLfloat v2[] = { bottom, right, 0.0f };
GLfloat v3[] = { bottom, left, 0.0f };

line( v0, v1 );
line( v1, v2 );
line( v2, v3 );
line( v3, v0 );

#undef line
}

void draw( const draw_state & state, double time_fract )
{
assert( time_fract >= 0.0 );
assert( time_fract <= 1.0 );

glPushMatrix();
glError();

glBegin( GL_LINES );

for( size_t i = 0; i < state.buildings.size(); ++i )
	{
	draw_building( state.buildings[ i ] );
	}

for( size_t i = 0; i < state.missiles.size(); ++i )
	{
	draw_missile( state.missiles[i], time_fract );
	}

for( size_t i = 0; i < state.bullets.size(); ++i )
	{
	draw_bullet( state.bullets[i], time_fract );
	}

for( size_t i = 0; i < state.explosions.size(); ++i )
	{
	draw_explosion( state.explosions[i], time_fract );
	}

glEnd();
glError();

glPopMatrix();
glError();
}
