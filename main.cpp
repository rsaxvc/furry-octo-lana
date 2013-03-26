#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>

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
#include "tank.h"

#include "draw_manager.h"
#include "draw_state.h"
#include "game_state.h"

#include "fps_filter.h"

int score = 0;
size_t update_index;

timer all_the_time(true);

void quit_tutorial( int code )
{
    draw_manager_stop();
    exit( code );
}

static void handle_key_down( SDL_keysym* keysym, game_state & gstate )
{
printf("key\n");
switch( keysym->sym )
	{
	case SDLK_ESCAPE:
		quit_tutorial( 0 );
		break;

	case SDLK_UP:
		gstate.setUp();
		break;

	case SDLK_DOWN:
		gstate.setDown();
		break;

	case SDLK_LEFT:
		gstate.setLeft();
		break;

	case SDLK_RIGHT:
		gstate.setRight();
		break;

	default:
		break;
	}
}

static void handle_key_up( SDL_keysym* keysym, game_state & )
{
switch( keysym->sym )
	{
	default:
		break;
	}
}

static void process_events( game_state & gstate )
{
/* Our SDL event placeholder. */
SDL_Event event;

/* Grab all the events off the queue. */
while( SDL_PollEvent( &event ) )
	{
	switch( event.type )
		{
		case SDL_KEYUP:
			handle_key_up( &event.key.keysym, gstate );
			break;

		case SDL_KEYDOWN:
			/* Handle key presses. */
			handle_key_down( &event.key.keysym, gstate );
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

	fps_filter fps;

	//main loop
	do
		{
		p.x=rand()%GRID_W;
		p.y = GRID_H-(rand()%(GRID_H/2) );

		process_events( gstate );//Process incoming events.

		static long double last_missile_time = get_time();
		if( get_time() - last_missile_time > 1.0 )
			{
			gstate.spawn_tank( p );
			last_missile_time = get_time();
			}

		gstate.check_explosions();
		gstate.check_collisions();
		gstate.check_boundaries();
		gstate.check_bullets();

		gstate.calculate_state();

		gstate.build_draw( state );

		draw_manager_update( state );

		fps.record_event();
		printf("physics:%f FPS\n", (double)fps.read() );
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
