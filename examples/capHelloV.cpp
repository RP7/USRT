#define FUNCLASS capHelloV
#include <capabilityAPI.h>
#include <stdio.h>
#include <usrttype.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/time.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  struct timeval tv;
	gettimeofday(&tv,0);
	utime_t now = (utime_t)tv.tv_sec*1000000;
	now += (utime_t)tv.tv_usec;
  utime_t run = *(utime_t *)argv;
  fprintf(stderr,"%ld\n",run-now);
  return 0;
}



