#include "game.h"
#include "game_state.h"

#include <algorithm>
#include <vector>

#include "position.h"

#include "entity.h"
#include "tank.h"
#include "bullet.h"
#include "explosion.h"

#include "grid.h"

void game_state::calculate_state( void )
{
for( size_t i = 0; i < entities.size(); ++i )
    {
    entities[i]->calcState();
    }
p.calcState();
}

void game_state::build_draw( draw_state & state )
{
static int framecount;

state.clear();
for( size_t i = 0; i < bullets.size(); ++i )
	{
	bullet_state s;
	s.last_pos = bullets[i]->getLastPos();
	s.pos = bullets[i]->getPos();
	s.vel = bullets[i]->getVel();
	state.bullets.push_back(s);
	}

for( size_t i = 0; i < tanks.size(); ++i )
	{
	tank_state s;
	s.last_pos = tanks[i]->getLastPos();
	s.pos = tanks[i]->getPos();
	s.vel = tanks[i]->getVel();
	s.heading = tanks[i]->getHeading();
	state.tanks.push_back(s);
	}

//add player tank
tank_state s;
s.last_pos = p.getLastPos();
s.heading = p.getHeading();
s.pos = p.getPos();
s.vel = p.getVel();
state.tanks.push_back(s);

for( size_t i = 0; i < explosions.size(); ++i )
	{
	explosion_state s;
	s.center = explosions[i]->getPos();
	s.r  = explosions[i]->getRadius();
	s.dr = explosions[i]->getRadiusRate();
	state.explosions.push_back(s);
	}

state.framestamp=framecount;
framecount++;
}

game_state::~game_state()
{
while( tanks.size() > 0 )
	{
	destroy_tank( tanks.size() - 1 );
	}

while( bullets.size() > 0 )
	{
	destroy_bullet( bullets.size() - 1 );
	}

while( explosions.size() > 0 )
	{
	destroy_explosion( explosions.size() - 1 );
	}
}

void game_state::spawn_tank( const position & p )
{
tank* m = new tank( p );
entities.push_back( m );
tanks.push_back( m );
}

void game_state::player_fire()
{
position pos( p.getPos(), p.getHeading(), 2*p.getRadius() );
bullet * b = new bullet( pos, p.getHeading() );
entities.push_back( b );
bullets.push_back( b );
}

void game_state::spawn_explosion( const position & start )
{
explosion * x = new explosion( start );
entities.push_back( x );
explosions.push_back( x );
}

static bool check_boundary( const position & pos )
{
return pos.x < 0 || pos.y < 0 || pos.x > GRID_W || pos.y > GRID_H;
}

void game_state::destroy_tank( size_t idx )
{
tank * ptr;
ptr = tanks[idx];
entities.erase( std::find( entities.begin(), entities.end(), ptr ) );
tanks.erase( tanks.begin() + idx );
delete ptr;
}

void game_state::check_boundaries( void )
{
for( size_t i = tanks.size(); i > 0; --i )
	{
	if( check_boundary( tanks[ i - 1 ]->getPos() ) )
		{
		destroy_tank( i - 1 );
		}
	}

for( size_t i = bullets.size(); i > 0; --i )
	{
	if( check_boundary( bullets[ i - 1 ]->getPos() ) )
		{
		bullets.erase( bullets.begin() + i - 1 );
		}
	}
}

void game_state::destroy_explosion( size_t idx )
{
explosion * ptr;
ptr = explosions[idx];
entities.erase( std::find( entities.begin(), entities.end(), ptr ) );
explosions.erase( explosions.begin() + idx );
delete ptr;
}

void game_state::check_explosions( void )
{
for( size_t i = explosions.size(); i > 0; --i )
	{
	if( explosions[i-1]->complete() )
		{
		destroy_explosion( i - 1 );
		}
	}
}

void game_state::destroy_bullet( size_t idx )
{
bullet * ptr;
ptr = bullets[idx];
entities.erase( std::find( entities.begin(), entities.end(), ptr ) );
bullets.erase( bullets.begin() + idx );
delete ptr;
}

bool collision( const entity & lhs, const entity & rhs )
{
return
	lhs.getRadius() + rhs.getRadius() > lhs.getPos().dist( rhs.getPos() );
}

void game_state::check_bullets( void )
{
for( size_t b = bullets.size(); b > 0; --b )
	{
	for( size_t t = tanks.size(); t > 0; --t )
		{
		if( collision( *bullets[b-1], *tanks[t-1] ) )
			{
			spawn_explosion( bullets[ b - 1 ]->getPos() );
			destroy_bullet( b - 1 );
			break;
			}
		}
	}
}

void game_state::check_collisions( void )
{
for( size_t i = explosions.size(); i > 0; --i )
	{
	position explosion_pos = explosions[i-1]->getPos();
	float explosion_radius = explosions[i-1]->getRadius();
	for( size_t j = tanks.size(); j > 0; --j )
		{
		if( explosion_pos.dist( tanks[ j - 1 ]->getPos() ) < explosion_radius )
			{
			spawn_explosion( tanks[ j - 1 ]->getPos() );
			destroy_tank( j - 1 );
			}
		}
	}
}
