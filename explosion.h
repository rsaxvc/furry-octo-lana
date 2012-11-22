#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "entity.h"
#include "position.h"
#include "velocity.h"

class explosion : public entity
    {
	private:
		long double start_time;
		float radius;

    public:
		void calcState( void );
		explosion( const position & p );
		virtual ~explosion();
		inline entity_class getClass(){return CLASS_EXPLOSION;};
		float getTopSpeed(){return 0.0f;};
		inline const float & getRadius(){return radius;};
		bool complete( void );
    };

#endif
