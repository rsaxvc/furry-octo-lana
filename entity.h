#ifndef ENTITY_H
#define ENTITY_H

#include "position.h"
#include "velocity.h"

enum entity_class
	{
	CLASS_MISSILE,
	CLASS_BULLET,
	CLASS_EXPLOSION,
	CLASS_CNT
	};


class entity
	{
	protected:
		position last_pos;
		position pos;
		velocity vel;

	public:
		virtual entity_class getClass()=0;
		inline const position & getPos() { return pos; };
		inline const position & getLastPos() { return last_pos; };
		inline const velocity & getVel(){ return vel;};
		virtual float getSpeed();
		virtual float getTopSpeed()=0;

		void calcState();

		virtual void setVel( const velocity & vel );
	};

#endif
