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
void keyPss(int64 key[2])
{
	key[1]=0x7e95373c35421368LL;
	key[0]=0x1LL;
}
void keyPssAntMap(int64 key[2])
{
	key[1]=0x7e95373c35421368LL;
	key[0]=0x2LL;
}
void keyUeDownCoding(int64 key[2])
{
	key[1]=0x7e95373c35421368LL;
	key[0]=0x3LL;
}
void keyUeDownMod(int64 key[2])
{
	key[1]=0x7e95373c35421368LL;
	key[0]=0x4LL;
}
void keyUeDownAntMap(int64 key[2])
{
	key[1]=0x7e95373c35421368LL;
	key[0]=0x5LL;
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
