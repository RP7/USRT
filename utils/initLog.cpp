#include <usrttype.h>
#include <stdio.h>
#include <Log.h>

int main( int argc, char *argv[] )
{
  int n;
  if( argc>1 ) {
    sscanf(argv[1],"%d",&n);
    initLog(n);
    releaseLog(n);
  }
  else {
    for( n=0;n<MAXLogLevel;n++ )
    initLog(n);
    releaseLog(n);
  }
  return 0;  
}