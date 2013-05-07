#define FUNCLASS capHello
#include <capabilityAPI.h>
#include <stdio.h>
#include <usrttype.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  fprintf(stderr,"Hello world\n");
  return 0;
}



