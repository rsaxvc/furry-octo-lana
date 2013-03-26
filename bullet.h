#ifndef bullet_H
#define bullet_H

#include "entity.h"
#include "position.h"
#include "velocity.h"

class bullet : public entity
    {
	private:

	public:
		bullet( const position & start, float heading );
		virtual ~bullet();
		inline entity_class getClass()const{return CLASS_BULLET;};
		float getTopSpeed()const;
		float getRadius()const;
    };

#endif
