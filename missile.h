#ifndef MISSILE_H
#define MISSILE_H

#include "entity.h"
#include "position.h"
#include "velocity.h"

class missile : public entity
	{
	private:

	public:
		missile(const position & p);
		virtual ~missile();
		inline entity_class getClass(){return CLASS_MISSILE;};
		float getTopSpeed();

		bool fire();
    };

#endif
