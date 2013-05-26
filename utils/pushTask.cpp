#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <USRTCapabilityBearer.h>
#include <USRTTaskQueue.h>

#include <usrttype.h>
#include <MapMem.h>
#include <md5api.h>
#include <unistd.h>
#include <callback.h>
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
  int intl = 1;
  int cnt = 1;
  strcpy( cap, "cap" );
  strcat( cap, argv[1] );
  capKey = md5first( cap );
  
  int opt;
  USRTTaskQueue q;
  utime_t now = q.getNow();
  _trace_t *aLog = NULL;
  int log = 0;
  while((opt=getopt(argc-1,argv+1,"j:i:t:k:c:n:d:rml"))!=-1) {
    switch(opt) {
      case 'm':
        initMem("task1");
        start("task1");
        release("task1");
        exit(0);
        break;
 
      case 't':
        tName=optarg;
        break;
      case 'r':
        *(int64 *)buf=now;
        bufLen=8;
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
      case 'i':
        sscanf(optarg,"%d",&intl);
        break;
      case 'j':
        sscanf(optarg,"%d",&cnt);
        break;
      case 'd':
        sscanf(optarg,"%d",&delay);
        break;
      case 'l':
        log=1;
        break;
      default:
        fprintf(stderr,"Usage: %s(%c) capability [-t taskMem] [-k key] [-c string] [-n num] [-d delay] [-i intl] [-j cnt] [-r] [-m]\n",
                    argv[0],opt);
        exit(EXIT_FAILURE);
        break;
    }
  }
  fprintf(stderr,"Run: %llx -t %s \n",capKey,tName);
  q.attach(tName);
  attach("task1");
  task_t *task = allocTask("task1",1LL);
  if( log == 0 ) {
    void *a = allocMem("task1",((bufLen+16)/16)*16);
    memset(a,0,bufLen+16);
    memcpy(a,buf,bufLen);
    fprintf(stderr,"prepare args\n");
    L2G(&(task->argv),a);
  } 
  else {
    aLog = (_trace_t *)allocMem("task1",sizeof(_trace_t));
    aLog->lunch=__getNow();
    aLog->delay=(long long int)delay;
    L2G(&(task->argv),aLog);
    fprintf(stderr,"prepare trace\n");
  }
  task->key = capKey;
  task->noL = now+(long long int)delay;
  task->noE = task->noL-3LL*2667LL;
  fprintf(stderr,"prepare task %lld\n",task->noE);
  setCallBackRepeat(task,intl,cnt,abss);
  fprintf(stderr,"callback cnt=%d intl=%d\n",cnt,intl);
  void *vgp = q.allocMem(sizeof(generalized_memory_t));
  memcpy( vgp, &(task->mem), sizeof(generalized_memory_t) );
  q.push(vgp);
  fprintf(stderr,"push task\n");
  release("task1");
  
}
