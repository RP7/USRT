#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <usrttype.h>
#include <Log.h>
#include <unistd.h>

using namespace std;
int main(int argc, char *argv[])
{
  int level=6;
  int i;
  int num;
  int delay;
  sscanf(argv[1],"%d",&num);
  sscanf(argv[2],"%d",&delay);
  attachLog(level);
  const char *log;
  map<int,int> s;
  map<int,int>::iterator iter;
  int read;
  int cnt=0;
  while( num-- ) {
      while( (log = getLog(level))==NULL );
      int read;
      int ret=sscanf(log,"%d",&read);
      read+=delay;
 //     if( read>1000000 ) read=1000000;
 //     if( read<-100000 ) read=-100000;
      iter = s.find(read);
      if( iter==s.end() )
        s[read]=1;
      else
        s[read]+=1;
      if( (num&0xfff)==0 ) fprintf(stderr,"%d read %d cnt %d\n",num,read,cnt);
      cnt++;
  }
  cnt=0;
  for( iter=s.begin();iter!=s.end();iter++ ) {
    fprintf(stdout,"%d %d\n",iter->first,iter->second);
    cnt+=iter->second;
  }
  fprintf(stderr,"total:%d\n",cnt);
  return 0;    
}
