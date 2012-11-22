#include <unistd.h>
#include "time.h"

#include "periodic_controller.h"

periodic_controller::~periodic_controller()
{
}

periodic_controller::periodic_controller( double the_rate )
{
rate = the_rate;
expire_time = get_time()+rate;
}

bool periodic_controller::wait()
{
double cur_time;
cur_time = get_time();
if( cur_time > expire_time )
	{
	//bail early if we're too late
	}
else
	{
	while( expire_time - cur_time >= 1.0 )
		{
		//nice, low-res wait
		sleep( (unsigned int)(expire_time - cur_time) );
		cur_time = get_time();
		}

	//mediumres timer
	while( expire_time - cur_time >= 0.001 )
		{
		usleep( 1000 * ( expire_time - cur_time ) );
		cur_time = get_time();
		}

	//highres, but expensive timer
	do
		{
		cur_time = get_time();
		}while( cur_time < expire_time );

//	expire_time = get_time() + rate;
	}
expire_time += rate;
return true;
}
