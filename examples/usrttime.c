#include <usrttype.h>
#include <sys/time.h>

utime_t getHardware()
{
	return 1;	
}

utime_t getSubFrameDuration()
{
	return 1000;	
}
utime_t getNow()
{
	struct timeval tv;
	gettimeofday(&tv,0);
	utime_t now = (utime_t)tv.tv_sec*1000000;
	now += (utime_t)tv.tv_usec;
	return now;
}
utime_t getFrameTiming(int frame, int subframe )
{
	utime_t timing = getNow();
	timing += (utime_t)frame*10000+(utime_t)subframe*1000;
	return timing;	
}
