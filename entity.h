#ifndef ENTITY_H
#define ENTITY_H

#include "position.h"
#include "velocity.h"

enum entity_class
	{
	CLASS_TANK,
	CLASS_BULLET,
	CLASS_EXPLOSION,
	CLASS_PLAYER,
	CLASS_CNT
	};


class entity
	{
	protected:
		position start_pos;
		position last_pos;
		position pos;
		velocity vel;
		float heading;

	public:
		entity();
		virtual entity_class getClass()=0;
		inline const position & getStartPos() { return start_pos; };
		inline const position & getPos() { return pos; };
		inline const position & getLastPos() { return last_pos; };
		inline const velocity & getVel(){ return vel;};
		virtual float getSpeed();
		virtual float getTopSpeed()=0;
        float getHeading(){return heading;};

		virtual void calcState();

		virtual void setVel( const velocity & vel );
	};

#endif
