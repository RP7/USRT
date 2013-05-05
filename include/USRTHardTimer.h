#ifndef USRT_Hard_Timer_H
#define USRT_Hard_Timer_H
#include <sys/time.h>
class USRTHardTimer {
  private: 
    long long int end;
    long long int getNow() {
	    struct timeval tv;
	    gettimeofday(&tv,0);
	    long long int now = (long long int)tv.tv_sec*1000000;
	    now += (long long int)tv.tv_usec;
	    return now;
    };
      
  public:
    USRTHardTimer( int time ) {end=getNow()+(long long int)time;};
    int expired() { return end<getNow(); };
};
#endif //USRT_Hard_Timer_H