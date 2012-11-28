#ifndef DRAW_MANAGER_H
#define DRAW_MANAGER_H

#define DISPLAY_W 800
#define DISPLAY_H 800

#include "draw_state.h"

void draw_manager_start( void );
void draw_manager_update( const draw_state & state );
void draw_manager_stop( void );
#endif
