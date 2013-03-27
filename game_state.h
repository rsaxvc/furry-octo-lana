#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>

#include "position.h"

#include "entity.h"
#include "bullet.h"
#include "tank.h"
#include "explosion.h"
#include "player.h"

#include "grid.h"

#include "draw_state.h"

class game_state
	{
	private:
		player				p;
		void spawn_explosion( const position & p );
		std::vector<entity*> entities;
		std::vector<bullet*> bullets;
		std::vector<tank*> tanks;
		std::vector<explosion*> explosions;

		void destroy_tank( size_t idx );
		void destroy_bullet( size_t idx );
		void destroy_explosion( size_t idx );

		bool player_dead;

	public:
		inline void player_setUp(){p.setUp();};
		inline void player_setDown(){p.setDown();};
		inline void player_setLeft(){p.setLeft();};
		inline void player_setRight(){p.setRight();};
		void player_fire();

		void calculate_state( void );
		void spawn_tank( const position & p );

		void check_boundaries( void );
		void check_bullets( void );
		void check_collisions( void );
		void check_explosions( void );

		~game_state( void );

		void build_draw( draw_state & state );

		size_t num_bullets( void ){return bullets.size();};
		size_t num_tanks( void ){return tanks.size();};

		bool over();
		bool player_victory();
	};
#endif
