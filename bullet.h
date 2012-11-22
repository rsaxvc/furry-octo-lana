#ifndef bullet_H
#define bullet_H

#include "entity.h"
#include "position.h"
#include "velocity.h"

class bullet : public entity
    {
	private:
		position pos_start;
		position pos_end;
	public:
		bullet( const position & start, const position & end );
		virtual ~bullet();
		bool target_reached();
		inline entity_class getClass(){return CLASS_BULLET;};
		float getTopSpeed();
    };

#endif
