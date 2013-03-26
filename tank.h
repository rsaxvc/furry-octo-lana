#ifndef TANK_H
#define TANK_H

#include "entity.h"
#include "position.h"
#include "velocity.h"

class tank : public entity
	{
	private:

	public:
		tank(const position & p);
		virtual ~tank();
		inline entity_class getClass(){return CLASS_TANK;};
		float getTopSpeed();
		void calcState( void );
		void fire();
    };

#endif
