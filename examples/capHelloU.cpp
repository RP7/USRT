#define FUNCLASS capHelloU
#include <capabilityAPI.h>
#include <stdio.h>
#include <usrttype.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <Log.h>
#include <trace.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  if( argv==NULL ) return -1;
  _trace_t * trace = (_trace_t *) argv;
  trace->end = __getNow();
  trace->tid = (long int)syscall(__NR_gettid);
  _trace_t *back = dupTrace(trace);
  trace->lunch = __getNow();
  pushOffLog(TRACELOG,back->off);
  return 0;
}



