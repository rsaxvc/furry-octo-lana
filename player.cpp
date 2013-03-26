#include "player.h"
#include "grid.h"

#define SPEED 15.0f

player::player()
{
pos.x = GRID_W / 2;
pos.y = GRID_H / 2;
last_pos.x = pos.x;
last_pos.y = pos.y;
vel.dx = 0.0f;
vel.dy = 0.0f;
}

float player::getTopSpeed()
{
return SPEED;
}

void player::setUp( void )
{
vel.dy += SPEED;
}

void player::setLeft( void )
{
vel.dx -= SPEED;
}

void player::setRight( void )
{
vel.dx += SPEED;
}

void player::setDown( void )
{
vel.dy -= SPEED;
}

