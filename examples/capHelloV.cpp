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
  fprintf(stderr,"%lld %lld %lld\n",run-now,run,now);
  return 0;
}



