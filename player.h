#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class player : public entity
	{
	public:
		player();
		void setUp( void );
		void setLeft( void );
		void setRight( void );
		void setDown( void );

		inline entity_class getClass(){return CLASS_PLAYER;};
		float getTopSpeed();

	};

#endif
