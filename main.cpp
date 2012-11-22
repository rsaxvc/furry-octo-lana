#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <SDL/SDL.h>

#include <algorithm>
#include <deque>
#include <vector>

#include "draw.h"
#include "game.h"
#include "grid.h"
#include "time.h"
#include "timer.h"
#include "periodic_controller.h"

#include "bullet.h"
#include "missile.h"

#include "draw_manager.h"
#include "draw_state.h"
#include "game_state.h"

int score = 0;
size_t update_index;

timer all_the_time(true);

template <typename T>
static T clamp( const T & val, const T & min, const T & max )
{
assert( max > min );

if( val > max )return max;
if( val < min )return min;
return val;
}


void quit_tutorial( int code )
{
    draw_manager_stop();
    exit( code );
}

static void handle_key_down( SDL_keysym* keysym )
{
switch( keysym->sym )
	{
	case SDLK_ESCAPE:
		quit_tutorial( 0 );
		break;

	default:
		break;
	}
}

static void handle_key_up( SDL_keysym* keysym )
{
switch( keysym->sym )
	{
	default:
		break;
	}
}

static void process_events( void )
{
/* Our SDL event placeholder. */
SDL_Event event;

/* Grab all the events off the queue. */
while( SDL_PollEvent( &event ) )
	{
	switch( event.type )
		{
		case SDL_KEYUP:
			handle_key_up( &event.key.keysym );
			break;

		case SDL_KEYDOWN:
			/* Handle key presses. */
			handle_key_down( &event.key.keysym );
			break;

		case SDL_QUIT:
			/* Handle quit requests (like Ctrl-c). */
			quit_tutorial( 0 );
			break;
		}
	}
}

int main( void )
{
	draw_manager_start();

	periodic_controller periodic( PHYSICS_TIMESTEP );
	draw_state state;

	game_state gstate;

	position p;
	const position g( GRID_W / 2, 0 );

	p.x=rand()%GRID_W;
	p.y = GRID_H-1;

	//main loop
	do
		{
		p.x=rand()%GRID_W;
		p.y = GRID_H-3;

        process_events();//Process incoming events.

		static long double last_missile_time = get_time();
		if( get_time() - last_missile_time > 1.0 )
			{
			gstate.spawn_missile( p );
			last_missile_time = get_time();
			}

		gstate.spawn_bullet( g, p );

		gstate.check_explosions();
		gstate.check_collisions();
		gstate.check_boundaries();
		gstate.check_bullets();

		gstate.calculate_state();

		gstate.build_draw( state );

		draw_manager_update( state );
		printf("physics\n");
    	}while( periodic.wait() );

    /*
     * EXERCISE:
     * Record timings using SDL_GetTicks() and
     * and print out frames per second at program
     * end.
     */

    /* Never reached. */
    return 0;
}
