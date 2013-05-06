#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <USRTCapabilityBearer.h>
#include <usrttype.h>
#include <MapMem.h>
#include <md5api.h>
#include <unistd.h>

using namespace std;
int main(int argc, char *argv[])
{
  const char *tName;
  int64 key;
  int64 capKey;
  int num;
  char cap[256];
  char task0[]="task0";
  char buf[256];
  int bufLen=0; 
  tName=task0;
  
  strcpy( cap, "capWorkers" );
  strcat( cap, argv[1] );
  capKey = md5first( cap );
  
  int opt;
  while((opt=getopt(argc-1,argv+1,"t:k:c:n:"))!=-1) {
    switch(opt) {
      case 't':
        tName=optarg;
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
      default:
        fprintf(stderr,"Usage: %s capability [-t taskMem] [-k key] [-c string] [-n num] \n",
                    argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
  }
  fprintf(stderr,"Run: %llx -t %s \n",capKey,tName);            
  attach(tName);
  void *a = allocMem(tName,((bufLen+16)/16)*16);
  memset(a,0,bufLen+16);
  memcpy(a,buf,bufLen);
  task_t *task = allocTask(tName,0LL);
  L2G(&(task->argv),a);
  task->key = capKey;
  pushTask(tName,task);
  release(tName);
}