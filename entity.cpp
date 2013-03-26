#include <cmath>

#include "entity.h"
#include "game.h"
#include "velocity.h"
#include "position.h"

entity::entity()
{
heading = 0.0f;
}

void entity::setVel( const velocity & invel )
{
vel = invel;
}

float entity::getSpeed()
{
return sqrt( vel.dx*vel.dx + vel.dy*vel.dy );
}

void entity::calcState()
{
last_pos = pos;

pos.x += vel.dx * PHYSICS_TIMESTEP;
pos.y += vel.dy * PHYSICS_TIMESTEP;

if( vel.dx > 0.01f || vel.dx < -0.01f || vel.dy > .01f || vel.dy < -0.01f )
	{
	heading = atan2( vel.dy, vel.dx ) - 3.1415/2;
	}
}

