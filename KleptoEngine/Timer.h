#ifndef TIMER_H
#define TIMER_H


class Timer
{
	private:
		//previousTiming gives the value
		//of milliseconds since the 
		//previous time check
		unsigned long long previousTiming;
		
		//Limit is the number of milliseconds
		//needed to reach the next cycle, i.e.
		//if the frame limit is 45 frames/second
		//the limit value would be 1000/45
		unsigned int limit;
	public:
		Timer();
		Timer(unsigned int firstLimit);
		void setLimit(unsigned int newLimit);
		bool updateTimer(void);
};

#endif