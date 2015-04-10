#ifndef TIMER_H
#define TIMER_H

#include <ctime>

#include <ostream>

class Timer
{
private:
	clock_t _clockSum;
	clock_t _clockLastStart;
	clock_t _clockTmp;
	clock_t _clockLastPeriod;

public:
	Timer():_clockSum(0), _clockLastStart(),_clockTmp(0),_clockLastPeriod(0){}

	void start()
	{
		_clockLastStart = clock();	
	}

	void stop()
	{
		_clockTmp = clock();
		_clockLastPeriod = _clockTmp-_clockLastStart;
		_clockSum+=_clockLastPeriod;
		_clockLastStart = _clockTmp;
	}

	void print(const char * lpszMessage, std::ostream & os)
	{
		os<<lpszMessage<< ((double) _clockSum)/CLOCKS_PER_SEC<<std::endl;
	}

	void printLastPeriod(const char * lpszMessage, std::ostream & os)
	{
		os<<lpszMessage<< ((double) _clockLastPeriod)/CLOCKS_PER_SEC<<std::endl;
	}


};

#endif