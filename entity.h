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
		inline const position & getStartPos() const { return start_pos; };
		inline const position & getPos()      const { return pos; };
		inline const position & getLastPos()  const { return last_pos; };
		inline const velocity & getVel()      const { return vel;};
		inline float getHeading()             const {return heading;};

		        float getSpeed()              const;
		virtual float getTopSpeed()           const =0;
		virtual float getRadius()             const =0;
		virtual entity_class getClass()       const=0;

		virtual void calcState();

		virtual void setVel( const velocity & vel );
	};

#endif
