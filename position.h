#ifndef POSITION_H
#define POSITION_H

#include <cmath>
#include <cstdlib>

#include "velocity.h"

struct position
	{
	float x;
	float y;
	inline bool operator==(const position & p2 )const{ return x==p2.x && y == p2.y; }
	inline float dist( const position & pos )const{ return sqrt( (float)(x-pos.x)*(x-pos.x)+(y-pos.y)*(y-pos.y) ); }
	inline float tdist( const position & pos )const{ return fabs(x-pos.x)+fabs(y-pos.y); }

	position(){};
	position(float tx, float ty){x=tx;y=ty;};
	position( const position & start, const position & end )
		{
		x = start.x - end.x;
		y = start.y - end.y;
		}

	position( const position & start, float heading, float distance )
		{
		x = start.x + cos( heading ) * distance;
		y = start.y + sin( heading ) * distance;
		}

	position( const position & start, const position & end, double interp )
		{
		float dx;
		float dy;

		dx = ( end.x - start.x );
		dy = ( end.y - start.y );

		x = start.x + ( interp * dx );
		y = start.y + ( interp * dy );
		}

	position( const position & start, const velocity & vel, double dtime )
		{
		x = start.x + ( dtime * vel.dx );
		y = start.y + ( dtime * vel.dy );
		}

	float normalized_dot_product3( const position & end, const position & pt3 )
		{
		position ds( *this, end );
		position d3( *this, pt3 );
		return ( ds.x * d3.x + ds.y * d3.y ) / ( ds.x * ds.x + ds.y * ds.y );
		}
	};

#endif
