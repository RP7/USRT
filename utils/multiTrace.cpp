#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <USRTCapabilityBearer.h>
#include <USRTTaskQueue.h>

#include <usrttype.h>
#include <MapMem.h>
#include <md5api.h>
#include <unistd.h>

#include <trace.h>

using namespace std;
int main(int argc, char *argv[])
{
  const char *tName;
  int64 key;
  int64 capKey;
  int num;
  char cap[256];
  char task0[]="queue0";
  char buf[256];
  int bufLen=0; 
  tName=task0;
  int delay = 0;
  
  strcpy( cap, "cap" );
  strcat( cap, argv[1] );
  capKey = md5first( cap );
  
  int opt;
  int repeat=1;
  USRTTaskQueue q;
  clearLog();
  attachLog(TRACELOG);
  while((opt=getopt(argc-1,argv+1,"t:k:c:n:d:r:"))!=-1) {
    switch(opt) {
      case 't':
        tName=optarg;
        break;
      case 'r':
        sscanf(optarg,"%d",&repeat);
        break;
      case 'k':
        sscanf(optarg,"%llx",&key);
        *(int64 *)buf=key;
        bufLen=8;
        break;
      case 'c':
        strcpy(buf,optarg);
        bufLen=strlen(optarg)+1;
        buf[bufLen-1]='\0';
        break;
      case 'n':
        sscanf(optarg,"%d",&num);
        *(int *)buf=num;
        bufLen=4;
        break;
      case 'd':
        sscanf(optarg,"%d",&delay);
        break;
      default:
        fprintf(stderr,"Usage: %s(%c) capability [-t taskMem] [-k key] [-c string] [-n num] [-d delay] [-r repeat]\n",
                    argv[0],opt);
        exit(EXIT_FAILURE);
        break;
    }
  }
  q.attach(tName);
  attach("task1");
  while( repeat-- ) {
    utime_t now = q.getNow();
    _trace_t *a = newTrace();
    a->lunch=now;
    a->delay=(long long int)delay;
    task_t *task = allocTask("task1",1LL);
    task->argv.memKey=keyOfTrace();
    task->argv.offset=a->off;
    task->key = capKey;
    task->noL = now+(long long int)delay;
    task->noE = task->noL-3LL*2667LL;
    void *vgp = q.allocMem(sizeof(generalized_memory_t));
    memcpy( vgp, &(task->mem), sizeof(generalized_memory_t) );
    while( (q.getNow()-now)<3LL*2667LL );
    q.push(vgp);
    if( (repeat&0xfff)==0 ) fprintf(stderr,".");
  }
  fprintf(stderr,"stop\n");
  release("task1");
}
