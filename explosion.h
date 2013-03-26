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
		static const float EXPLOSION_SPEED=20.0f;
		static const float EXPLOSION_LIFETIME=1;

    public:
		void calcState( void );
		explosion( const position & p );
		virtual ~explosion();
		inline entity_class getClass()const{return CLASS_EXPLOSION;};
		float getTopSpeed()const{return 0.0f;};
		inline float getRadius()const{return radius;};
		inline float getRadiusRate()const{return EXPLOSION_SPEED;};
		bool complete( void );
    };

#endif
