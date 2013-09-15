#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstring>

#include <stddef.h>

#include "draw_state.h"

#include <GL/gl.h>
#include <GL/glu.h>

static inline void glError2( const char * filename, int line )
{
GLenum err = glGetError();
while (err != GL_NO_ERROR)
	{
	fprintf(stderr, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), filename, line);
	err = glGetError();
    }
}

#define glError() glError2( __FILE__, __LINE__ )

static void draw_bullet( const bullet_state & state, double dtime )
{
glBegin( GL_LINES );

position f( state.pos, state.vel, dtime );
#define LEN 1.5f
#define line( _pt1,_pt2 ) glVertex2fv( _pt1 );glVertex2fv( _pt2 )
GLfloat v0[] = { f.x-LEN, f.y-LEN };
GLfloat v1[] = { f.x+LEN, f.y+LEN };
GLfloat v2[] = { f.x-LEN, f.y+LEN };
GLfloat v3[] = { f.x+LEN, f.y-LEN };

line( v0, v1 );
line( v1, v2 );
line( v2, v3 );
line( v3, v0 );

GLfloat v4[] = {  f.x,  f.y, 0.0f };
GLfloat v6[] = { state.last_pos.x, state.last_pos.y, 0.0f };
line( v4, v6 );

#undef LEN
#undef line

glEnd();
}

void draw_tank( const tank_state & tank, double dtime )
{
static const GLfloat tank_width = 8.0f;
static const GLfloat tank_height = 10.0f;
static const GLfloat tread_width = 1.0f;

static const GLfloat tread_height = 1.0f;//thickness of a single tread
GLfloat tread_iter;

position f( tank.last_pos, tank.vel, dtime );

glPushMatrix();
glError();

glLineWidth( 1.0f );
glError();

glTranslatef( f.x, f.y, 0.0f );
glError();

glRotatef( 180 * tank.heading / 3.1415, 0.0f, 0.0f, 1.0f );
glError();

glColor4f(0.0f,1.0f,0.0f,1.0f); //line color

glBegin( GL_LINES );

	glVertex2f(  -tank_width/2,  -tank_height/2 );
	glVertex2f(  -tank_width/2,  tank_height/2 );

	glVertex2f(  -tank_width/2+tread_width,  -tank_height/2 );
	glVertex2f(  -tank_width/2+tread_width,  tank_height/2 );

	glVertex2f( tank_width/2, -tank_height/2 );
	glVertex2f( tank_width/2, tank_height/2 );

	glVertex2f( tank_width/2 -tread_width, -tank_height/2 );
	glVertex2f( tank_width/2 -tread_width, tank_height/2 );

	//top and bottom edges
	glVertex2f( -tank_width/2, -tank_height/2 );
	glVertex2f( tank_width/2, -tank_height/2 );

	glVertex2f( -tank_width/2, tank_height/2 );
	glVertex2f( tank_width/2, tank_height/2 );

	//draw treads

	float offset = 0;
		if ( tank.move_cycles < 1 == 0 )
		{
			offset = 1;
		}
		for( tread_iter = -tank_height/2; tread_iter < tank_height/2; tread_iter += tread_height )
		{
			glVertex2f( -tank_width/2, tread_iter + offset );
			glVertex2f( -tank_width/2 + tread_width, tread_iter + offset );

			glVertex2f( tank_width/2, tread_iter + offset );
			glVertex2f( tank_width/2-tread_width, tread_iter + offset );
		}

	//turret
	glVertex2f( -2.0f, -3.0f );
	glVertex2f( -2.0f, 3.0f );

	glVertex2f( -2.0f, 3.0f );
	glVertex2f( 2.0f, 3.0f );

	glVertex2f( 2.0f, 3.0f );
	glVertex2f( 2.0f, -3.0f );

	glVertex2f( 2.0f, -3.0f );
	glVertex2f( -2.0f, -3.0f );

	//barrel
	glVertex2f( -.5f, 3.0f );
	glVertex2f( -.5f, 6.0f );

	glVertex2f( .5f, 3.0f );
	glVertex2f( .5f, 6.0f );

	glVertex2f( -.5f, 6.0f );
	glVertex2f( .5f, 6.0f );

glEnd();
glError();

glPopMatrix();
glError();

}

static void draw_explosion( const explosion_state & state, float dtime )
{
#define line( _pt1,_pt2 ) glVertex2fv( _pt1 );glVertex2fv( _pt2 )
float r = state.r + dtime * state.dr;
float dAng= M_PI/20;
float spin = dtime * M_PI * 2 / 4;

GLfloat center[] = { state.center.x, state.center.y, 0.0f };

glBegin( GL_LINES );

for( float ang = 0.0f; ang < M_PI*2 + dAng; ang += dAng )
	{
	GLfloat vprev[] =
		{
		state.center.x + r * cosf( ang + spin - dAng ),
		state.center.y + r * sinf( ang + spin - dAng ),
		};
	GLfloat vedge[] =
		{
		state.center.x + r * cosf( ang + spin ),
		state.center.y + r * sinf( ang + spin ),
		};
	line( center, vedge );
	line( vprev, vedge );
	memcpy( vprev, vedge, sizeof( vprev ) );
	assert( sizeof( vprev ) == sizeof( vedge ) );
	}
#undef line

glEnd();
}

void draw_lines()
{
GLfloat fSizes[2];
GLfloat fCurrSize;

glGetFloatv( GL_LINE_WIDTH_RANGE, fSizes );

fCurrSize = fSizes[0];
for( GLfloat y = -90.0f; y < 90.0f; y+=20.0f )
	{
	glLineWidth( fCurrSize );

	glBegin( GL_LINES );

		glVertex2f( -80.0f, y );
		glVertex2f( 80.0f, y );

	glEnd();
	glError();
	fCurrSize += 1.0f;
	}
}

void draw( const draw_state & state, double dtime )
{
glPushMatrix();
glError();

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

glPopMatrix();
glError();
}
