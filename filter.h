#ifndef FILTER_H
#define FILTER_H

/*
http://en.wikipedia.org/wiki/Exponential_smoothing
*/

template <typename T>
class filter
	{
	private:
		T value;
		T scaler; /* should be between 0(light filter) and 1(heavy filter) */
	public:
		void set_val( T val ){ value = val;};
		void set_scaler( T scal ){ scaler = scal; };
		T read(){return value;};
		void insert(T new_val)
			{
			value = (1-scaler)*new_val + scaler * value;
			}
	};

#endif
