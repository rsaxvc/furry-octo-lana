#include "player.h"
#include "grid.h"

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
return 1.0f;
}

void player::setUp( void )
{
vel.dx = 0;
vel.dy = 1;
}

void player::setLeft( void )
{
vel.dx = -1;
vel.dy = 0;
}

void player::setRight( void )
{
vel.dx = 1;
vel.dy = 0;
}

void player::setDown( void )
{
vel.dx = 0;
vel.dy = -1;
}

