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

void entity::calcState()
{
last_pos = pos;

pos.x += vel.dx * PHYSICS_TIMESTEP;
pos.y += vel.dy * PHYSICS_TIMESTEP;
}

