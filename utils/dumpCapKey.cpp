#include <stdio.h>
#include <USRTCapabilityBearer.h>
int main(int argc, char *argv[])
{
  USRTCapabilityBearer *tut = new USRTCapabilityBearer(argv[1]);
  if( tut->isValid() )
    printf("key[%s]=%llx\n",argv[1],tut->getKey());
  delete tut;
}