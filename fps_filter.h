#ifndef FPS_FILTER_H
#define FPS_FILTER_H

#include "filter.h"
#include "time.h"

class fps_filter
	{
	private:
		bool first_call;
		long double last_time;
		filter<float> f;
	public:
		fps_filter()
			{
			f.set_val(0);
			f.set_scaler(.80);
			first_call = true;
			last_time = 0;
			}
		float read(){return f.read();};
		void record_event()
			{
			long double cur_time = get_time();
			if( first_call == false )
				{
				f.insert( 1/( cur_time - last_time ) );
				}
			last_time = cur_time;
			first_call = false;
			}
	};

#endif
