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
  utime_t now = __getNow();
  utime_t run = *(utime_t *)argv;
  fprintf(stdout,"%lld %lld %lld %lf\n",run-now,run,now,(double)now/2.667e9);
  return 0;
}



