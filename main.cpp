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

#include "fps_filter.h"

int score = 0;
size_t update_index;

timer all_the_time(true);

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

static float mouse_x;
static float mouse_y;

static void handle_mouse_down( game_state & gstate, SDL_MouseButtonEvent * event )
{
position target;
target.x = mouse_x;
target.y = mouse_y;

static long double last_bullet_time = get_time();

static const position gun( GRID_W / 2, 0 );
if( get_time() - last_bullet_time > .1 )
	{
	gstate.spawn_bullet( gun, target );
	last_bullet_time = get_time();
	}
}

static void handle_mouse_move( SDL_MouseMotionEvent * event )
{
mouse_x = GRID_W - ((float)event->x*GRID_W)/DISPLAY_W;
mouse_y = GRID_H - ((float)event->y*GRID_H)/DISPLAY_H;
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
		case SDL_MOUSEBUTTONDOWN:
			handle_mouse_down( gstate, &event.button );
			break;

		case SDL_MOUSEMOTION:
			handle_mouse_move( &event.motion );
			break;

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
			gstate.spawn_missile( p );
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
