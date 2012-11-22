#ifndef DRAW_MANAGER_H
#define DRAW_MANAGER_H

#include "draw_state.h"

void draw_manager_start( void );
void draw_manager_update( const draw_state & state );
void draw_manager_stop( void );
#endif
