#include <time.h>
#include <sys/time.h>

#include "time.h"

static long double get_raw_time()
{
timeval tp;
gettimeofday( &tp, NULL);
return tp.tv_sec + (long double)tp.tv_usec / 1000000;
}

static long double get_base_time()
{
static bool init_done = false;
static long double base_time;

if( !init_done )
	{
	base_time = get_raw_time();
	init_done = true;
	}

return base_time;
}

long double get_time()
{
return get_raw_time() - get_base_time();
}

