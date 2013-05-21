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
  sscanf(argv[1],"%d",&num);
  attachLog(level);
  _trace_t *log;
  map<int,int> s;
  map<int,int>::iterator iter;
  int read;
  int cnt=0;
  int all=num;
  while( num-- ) {
      while( (log = (_trace_t *)getLog(level))==NULL );
      int read=0-(log->end-log->lunch-log->delay);
      iter = s.find(read);
      if( iter==s.end() )
        s[read]=1;
      else
        s[read]+=1;
      if( (num&0xfff)==0 ) {
        fprintf(stderr,"%d read %d cnt %d\n",num,read,cnt);
        if( read<0 ) dumpTrace(stderr,log);
      }
      cnt++;
  }
  cnt=0;
  int valid=0;
  for( iter=s.begin();iter!=s.end();iter++ ) {
    fprintf(stdout,"%lf %le %d\n",(double)iter->first/2667.0,(double)cnt/(double)all,iter->second);
    if( iter->first>-2667*3 )
      valid+=iter->second;
    cnt+=iter->second;
  }
  fprintf(stderr,"total:%d valid:%lf\n",cnt,(double)valid/(double)cnt);
  releaseLog(level);
  return 0;    
}
