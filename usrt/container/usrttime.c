#include <usrttype.h>

time_t getHardware()
{
	return 1;	
}

time_t getSubFrameDuration()
{
	return 1000;	
}
time_t getNow()
{
	struct timeval tv;
	gettimeofday(&tv,0);
	time_t now = (time_t)tv.tv_sec*1000000;
	now += (time_t)tv.tv_usec;
	return now;
}
time_t getFrameTiming(int frame, int subframe )
{
	time_t timing = getNow();
	timing += (time_t)frame*10000+(time_t)subframe*1000;
	return timing;	
}
