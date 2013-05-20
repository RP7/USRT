#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <usrttype.h>
#include <Log.h>
#include <unistd.h>
#include <trace.h>

using namespace std;
int main(int argc, char *argv[])
{
  int level=TRACELOG;
  int i;
  int num;
  attachLog(level);
  _trace_t *log;
  sscanf(argv[1],"%d",&num);
  int cnt=0;
  while( num-- ) {
    while( (log = (_trace_t *)getLog(level))==NULL );
    dumpTrace(stdout,log);  
    if( (num&0xfff)==0 ) fprintf(stderr,"%d cnt %d\n",num,cnt);
    cnt++;
  }
  releaseLog(level);
  return 0;    
}
