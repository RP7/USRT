#include <usrttype.h>

#include"LTEDownLinkTransMock.h"

void *dummyArgv()
{
	return (void *)0;	
}
void *buildPss()
{
	return dummyArgv();
}
void *buildPssAntMap()
{
	return dummyArgv();
}
void *buildUeDownCoding(int i)
{
	return dummyArgv();
}
void *buildUeDownMod(int i)
{
	return dummyArgv();
}
void *buildUeDownAntMap(int i)
{
	return dummyArgv();
}
int64 keyPss()
{
	return 1;	
}
int64 keyPssAntMap()
{
	return 2;	
}
int64 keyUeDownCoding()
{
	return 3;	
}
int64 keyUeDownMod()
{
	return 4;	
}
int64 keyUeDownAntMap()
{
	return 5;	
}

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
