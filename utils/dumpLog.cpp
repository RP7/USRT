#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <usrttype.h>
#include <Log.h>
#include <unistd.h>

using namespace std;
int main(int argc, char *argv[])
{
  int mask=0xffff;
  int num=1;
  int opt;
  while((opt=getopt(argc,argv,"m:n:"))!=-1) {
    switch(opt) {
      case 'm':
        sscanf(optarg,"%x",&mask);
        break;
      case 'n':
        sscanf(optarg,"%d",&num);
        break;
      default:
        fprintf(stderr,"Usage: %s(%c) [-m mask] [-n times]\n",
                    argv[0],opt);
        exit(EXIT_FAILURE);
        break;
    }
  }
  int i;
  for( i=0;i<MAXLogLevel;i++ )
  {
    if( (1<<i)&mask ) {
      attachLog(i);
    }
  }
  while( num-- ) {
    for( i=0;i<MAXLogLevel;i++ )
    {
      if( (1<<i)&mask ) {
//        fprintf(stderr,"Log %d:\n",i);
        dumpLog2stdout(i);
      }
    }
  }
  for( i=0;i<MAXLogLevel;i++ )
  {
    if( (1<<i)&mask ) {
      releaseLog(i);
    }
  }
  return 0;    
}
