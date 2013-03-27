#include <SDL/SDL_thread.h>
#include <cassert>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "time.h"
#include "draw_state.h"
#include "grid.h"
#include "draw.h"
#include "main.h"
#include "periodic_controller.h"
#include "game.h"
#include "draw_manager.h"
#include "fps_filter.h"

#define glError() {GLenum err = glGetError();while (err != GL_NO_ERROR) {fprintf(stderr, "glError: %s caught at %s:%u\n",(char *)gluErrorString(err), __FILE__, __LINE__);err = glGetError();}}

bool stop_running = false;
SDL_Thread * thread = NULL;

static void draw_screen( const draw_state & state, double time )
{
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

//gluPerspective( 60.0, ratio, 1.0, 1024.0 );
glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0 );
}

draw_state   state;
long double  state_date;
SDL_mutex  * lock;

int render_func( void * )
{
draw_state local_state;
long double local_time;
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
    width = DISPLAY_W;
    height = DISPLAY_H;
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

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
//VSYNC	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,0);

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


setup_opengl( DISPLAY_W, DISPLAY_H );
periodic_controller periodic( DRAW_TIMESTEP );

fps_filter fps;
do
	{
	SDL_mutexP( lock );
	local_state = state;
	local_time = state_date;
	SDL_mutexV( lock );
	draw_screen( local_state, get_time() - local_time );
	fps.record_event();
//	printf("draw:%f FPS\n", (double)fps.read() );
	}while( !stop_running && periodic.wait() );


return( 0 );
}

void draw_manager_start( void )
{
assert( thread == NULL );
lock = SDL_CreateMutex();
thread = SDL_CreateThread(render_func, NULL);
if ( thread == NULL )
	{
	fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
	return;
	}
}

void draw_manager_stop( void )
{
stop_running = true;
SDL_WaitThread( thread, NULL );
SDL_DestroyMutex(lock);
lock = NULL;
thread = NULL;
SDL_Quit();
}

void draw_manager_update( const draw_state & inbound )
{
SDL_mutexP( lock );
state = inbound;
state_date = get_time();
SDL_mutexV( lock );
}
