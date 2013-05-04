#include <stdio.h>
#include <USRTCapabilityBearer.h>
#include <usrttype.h>
#include <MapMem.h>

int main(int argc, char *argv[])
{
  long long int key;
  sscanf(argv[1],"%llx",&key);
  USRTCapabilityBearer *tut = new USRTCapabilityBearer(key);
  if( tut->isValid() ) {
    printf("libs[%s]=%s\n",argv[1],tut->getName());
    initMem("task0");
    start("task0");
    task_t *t = allocTask("task0",1LL);
    tut->run(&(t->mem));
  }
  delete tut;
}