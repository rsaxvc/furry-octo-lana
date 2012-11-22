#ifndef PERIODIC_CONTROLLER
#define PERIODIC_CONTROLLER

class periodic_controller
	{
	public:
		periodic_controller( double delay_between_calls_in_seconds );
		~periodic_controller();
		bool wait();//always return true
	private:
		double expire_time;
		double rate;
	};

#endif
