#define FUNCLASS capHello
#include <capabilityAPI.h>
#include <stdio.h>
#include <usrttype.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  fprintf(stderr,"Hello world form %ld\n",(long int)syscall(__NR_gettid));
  return 0;
}



