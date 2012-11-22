#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

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

#include "draw_state.h"
#include "game_state.h"

#define glError() {GLenum err = glGetError();while (err != GL_NO_ERROR) {fprintf(stderr, "glError: %s caught at %s:%u\n",(char *)gluErrorString(err), __FILE__, __LINE__);err = glGetError();}}

int score = 0;
size_t update_index;

enum
	{
	OBJECT_UPDATE_NOTYET,  //not updated yet
	OBJECT_UPDATE_CURRENT, //updating now(id==local_id())
	OBJECT_UPDATE_DONE     //update has happened
	};

timer all_the_time(true);

template <typename T>
static T clamp( const T & val, const T & min, const T & max )
{
assert( max > min );

if( val > max )return max;
if( val < min )return min;
return val;
}


static void quit_tutorial( int code )
{
    SDL_Quit();
    exit( code );
}

static void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym ) {
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
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
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

static void draw_screen( const draw_state & state, double time )
{
//printf("draw:%f - %f\n",get_time(), time);

/* Clear the color and depth buffers. */
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glError();

/* We don't want to modify the projection matrix. */
glMatrixMode( GL_MODELVIEW );
glError();
glLoadIdentity( );
glError();
/* Move down the z-axis. */
glTranslatef( -GRID_W/2, -GRID_H/2, -15.0 ); glError();

grid_draw();
draw( state, time );

/*
 * Swap the buffers. This this tells the driver to
 * render the next frame from the contents of the
 * back-buffer, and to set all rendering operations
 * to occur on what was the front-buffer.
 */
SDL_GL_SwapBuffers( );
}

static void setup_opengl( int width, int height )
{
float ratio = (float) width / (float) height;

/* Our shading model--Gouraud (smooth). */
//    glShadeModel( GL_LINE_SMOOTH );

/* Culling. */
glCullFace( GL_BACK );
glFrontFace( GL_CCW );
glEnable( GL_CULL_FACE );

/* Set the clear color. */
glClearColor( 0, 0, 0, 0 );

/* Setup our viewport. */
glViewport( 0, 0, width, height );

/*
 * Change to the projection matrix and set
 * our viewing volume.
 */
glMatrixMode( GL_PROJECTION );
glLoadIdentity( );

/*
 * EXERCISE:
 * Replace this with a call to glFrustum.
 */
gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}

int main( void )
{
    /* Information about the current video settings. */
    const SDL_VideoInfo* info = NULL;
    /* Dimensions of our window. */
    int width = 0;
    int height = 0;
    /* Color depth in bits of our window. */
    int bpp = 0;
    /* Flags we will pass into SDL_SetVideoMode. */
    int flags = 0;

    /* First, initialize SDL's video subsystem. */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        quit_tutorial( 1 );
    }

    /* Let's get some video information. */
    info = SDL_GetVideoInfo( );

    if( !info ) {
        /* This should probably never happen. */
        fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
        quit_tutorial( 1 );
    }

    /*
     * Set our width/height to 640/480 (you would
     * of course let the user decide this in a normal
     * app). We get the bpp we will request from
     * the display. On X11, VidMode can't change
     * resolution, so this is probably being overly
     * safe. Under Win32, ChangeDisplaySettings
     * can change the bpp.
     */
    width = 800;
    height = 600;
//    width = 1280;
//    height = 768;
    bpp = info->vfmt->BitsPerPixel;

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
     */
//    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
//    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
//    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
//    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /*
     * We want to request that SDL provide us
     * with an OpenGL window, in a fullscreen
     * video mode.
     *
     * EXERCISE:
     * Make starting windowed an option, and
     * handle the resize events properly with
     * glViewport.
     */
    flags = SDL_OPENGL | SDL_FULLSCREEN;
    flags = SDL_OPENGL;

    /*
     * Set the video mode
     */
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
        /*
         * This could happen for a variety of reasons,
         * including DISPLAY not being set, the specified
         * resolution not being available, etc.
         */
        fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
        quit_tutorial( 1 );
    }

	/*
	 * At this point, we should have a properly setup
	 * double-buffered window for use with OpenGL.
	 */
	setup_opengl( width, height );

	periodic_controller periodic( PHYSICS_TIMESTEP * 1.0 / MINICHUNK );
	draw_state state;

	game_state gstate;

	position p;
	const position g( GRID_W / 2, 0 );

	p.x=rand()%GRID_W;
	p.y = GRID_H-1;

	//main loop
	do
		{
		static int ctr;

		if( ctr % MINICHUNK == 0 )
			{
			p.x=rand()%GRID_W;
			p.y = GRID_H-3;

	        process_events();//Process incoming events.

			gstate.spawn_missile( p );
			gstate.spawn_bullet( g, p );

			gstate.check_boundaries();
			gstate.check_bullets();
			gstate.check_collisions();
			gstate.check_explosions();

			gstate.calculate_state();

			//printf("physics:%f\n",get_time());
			gstate.build_draw(state);
			}

		draw_screen(state, ( ctr % MINICHUNK ) * 1.0f / MINICHUNK );//Draw the screen.

		ctr++;
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
