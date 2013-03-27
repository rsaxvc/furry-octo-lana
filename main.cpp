#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <SDL/SDL.h>

#include <X11/Xlib.h>

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
	case SDLK_SPACE:
		gstate.player_fire();
		break;

	case SDLK_ESCAPE:
		quit_tutorial( 0 );
		break;

	case SDLK_UP:
		gstate.player_setUp();
		break;

	case SDLK_DOWN:
		gstate.player_setDown();
		break;

	case SDLK_LEFT:
		gstate.player_setLeft();
		break;

	case SDLK_RIGHT:
		gstate.player_setRight();
		break;

	default:
		break;
	}
}

static void handle_key_up( SDL_keysym* keysym, game_state & gstate )
{
switch( keysym->sym )
	{
	case SDLK_UP:
		gstate.player_setDown();
		break;

	case SDLK_DOWN:
		gstate.player_setUp();
		break;

	case SDLK_LEFT:
		gstate.player_setRight();
		break;

	case SDLK_RIGHT:
		gstate.player_setLeft();
		break;

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
	XInitThreads();

	draw_manager_start();

	periodic_controller periodic( PHYSICS_TIMESTEP );
	draw_state state;

	game_state gstate;

	position p;

	fps_filter fps;

	for( int i = 0; i < 10; ++i )
		{
		p.x = rand()%GRID_W;
		p.y = rand()%GRID_H;

		gstate.spawn_tank( p );
		}

	//main loop
	do
		{
		process_events( gstate );//Process incoming events.

		gstate.check_explosions();
		gstate.check_collisions();
		gstate.check_boundaries();
		gstate.check_bullets();

		gstate.calculate_state();

		gstate.build_draw( state );

		draw_manager_update( state );

		fps.record_event();
		printf("physics:%f FPS\n", (double)fps.read() );
		}while( periodic.wait() && !gstate.over() );

    /*
     * EXERCISE:
     * Record timings using SDL_GetTicks() and
     * and print out frames per second at program
     * end.
     */

    /* Never reached. */
    return 0;
}
