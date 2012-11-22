#include <cmath>

#include "entity.h"
#include "game.h"
#include "velocity.h"
#include "position.h"

void entity::setVel( const velocity & invel )
{
vel = invel;
}

float entity::getSpeed()
{
return sqrt( vel.dx*vel.dx + vel.dy*vel.dy );
}

#include <iostream>

void entity::calcState()
{
last_pos = pos;

pos.x += vel.dx * PHYSICS_TIMESTEP;
pos.y += vel.dy * PHYSICS_TIMESTEP;

if( getClass() == CLASS_BULLET )
	{
	std::cout<<"pos:("<<pos.x<<","<<pos.y<<")\tlast_pos("<<last_pos.x<<","<<last_pos.y<<")"<<std::endl;
	}
}

