#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>

#include "position.h"

#include "entity.h"
#include "bullet.h"
#include "missile.h"
#include "explosion.h"

#include "grid.h"

#include "draw_state.h"

class game_state
	{
	private:
		void spawn_explosion( const position & p );
		std::vector<entity*> entities;
		std::vector<bullet*> bullets;
		std::vector<missile*> missiles;
		std::vector<explosion*> explosions;

		void destroy_missile( size_t idx );
		void destroy_bullet( size_t idx );
		void destroy_explosion( size_t idx );

//		std::vector<building*> buildings;

	public:
		void calculate_state( void );
		void spawn_missile( const position & p );
		void spawn_bullet( const position & start, const position & end );

		void check_boundaries( void );
		void check_bullets( void );
		void check_collisions( void );
		void check_explosions( void );

		~game_state( void );

		void build_draw( draw_state & state );

		size_t num_bullets( void ){return bullets.size();};
		size_t num_missiles( void ){return missiles.size();};

	};
#endif
