#include <stdio.h>
#include <USRTCapabilityBearer.h>
int main(int argc, char *argv[])
{
  long long int key;
  sscanf(argv[1],"%llx",&key);
  USRTCapabilityBearer *tut = new USRTCapabilityBearer(key);
  if( tut->isValid() )
    printf("libs[%s]=%s\n",argv[1],tut->getName());
  delete tut;
}