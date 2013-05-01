#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <workers.h>

int compareByNoEarly(task_t* a, task_t* b)
{
  return (a->noE>b->noE);  
}

int compareByNoLater(task_t* a, task_t* b)
{
  return (a->noL>b->noL);  
}
namespace std {
int workers::heapCheck(struct structHeap& h, int debug )
{
  FuncCompare func=h.func;
  int err=0;
  int i;
  for( i=0;i<h.size;i++ ) {
    int left = i*2+1;
    int right = i*2+2;
    if( left<h.size )
      if( func(h.heap[i],h.heap[left]) ) {
        printf("error @%d-%d\n",i,left);
        dumpTaskTime(h.heap[i]);
        dumpTaskTime(h.heap[left]);
        err++;
      }
    if( right<h.size )
      if( func(h.heap[i],h.heap[right]) ) {
        printf("error @%d-%d\n",i,right);
        dumpTaskTime(h.heap[i]);
        dumpTaskTime(h.heap[right]);
        err++;
      }
    if( debug < i ) {
      printf("No %d: ",i);
      dumpTaskTime(h.heap[i]);
    }
  }
  fflush(stdout);
  return err;
}

void workers::dumpTaskTime( task_t * a ) {
  printf("noE: %lld -- noL: %lld\n",a->noE,a->noL);
}

task_t* workers::pop( struct structHeap& h )
{
  FuncCompare func=h.func;
  if( h.size==0 )
    return NULL;
  __raw_spin_lock(&(h.lock));
  task_t* ret = h.heap[0];
  h.heap[0]=h.heap[h.size-1];
  h.size--;
  down(h,0);
  __raw_spin_unlock(&(h.lock));
  return ret;
}
void workers::down(struct structHeap& h, int index )
{
  FuncCompare func=h.func;
  int left = index*2+1;
  int right = index*2+2;
  int small = index;   
  if( left<h.size )
    if( func(h.heap[small],h.heap[left]) )
      small=left;
  if( right<h.size )
    if( func(h.heap[small],h.heap[right]) )
      small=right;
  if( small==index )
    return;
  task_t* a=h.heap[index];
  h.heap[index]=h.heap[small];
  h.heap[small]=a;
  down(h,small);
}
int workers::insert( struct structHeap& h, task_t *a )
{
  FuncCompare func=h.func;
  if( h.size<HEAPSIZE ) {
    __raw_spin_lock(&(h.lock));
    h.heap[h.size]=a;
    h.size++;
    up(h,h.size-1);
    __raw_spin_unlock(&(h.lock));
    return 0;
  } else
    return -1;
}

void workers::up(struct structHeap& h, int index )
{
  FuncCompare func=h.func;
  if( index==0 )
    return;
  int _up = (index-1)/2;
  if( func(h.heap[_up],h.heap[index]) ) {
    task_t* a=h.heap[index];
    h.heap[index]=h.heap[_up];
    h.heap[_up]=a;
    up(h,_up);
  }
}

void workers::start() {
  __raw_spin_unlock(&(head->wait.lock));
  __raw_spin_unlock(&(head->ready.lock));
}

workers::workers( int i )
{
  char file[256];
  sprintf(file,"worker_%04d",i);
  buf=new CPBuffer(1
    , 1
    , (long long)sizeof(struct structWorkHead)
    , file
    );
  head = (struct structWorkHead*)buf->attach();
  memset( head, 0, sizeof(struct structWorkHead) );
  
  head->wait.func = compareByNoEarly;
  head->ready.func = compareByNoLater;
}
}
#ifdef __HEAPTEST
int main()
{
  std::workers *tut = new std::workers(0);
  task_t *tasks = new task_t[HEAPSIZE];
  int i;
  tut->start();
  for(i=0;i<HEAPSIZE;i++) {
    task_t* a=tasks+i;
    a->noE = (utime_t)(rand()&0xff);
    a->noL = (utime_t)(rand()&0xff);
    if( tut->insert(tut->head->wait,a)!=0 ) printf("Insert wait error %d\n",tut->head->wait.size);
    if( tut->insert(tut->head->ready,a) !=0 ) printf("Insert ready error %d\n",tut->head->ready.size);
    tut->heapCheck(tut->head->wait,256);
    tut->heapCheck(tut->head->ready,256);
  }
  task_t *p;
  int cnt=0;
  while( (p=tut->pop(tut->head->wait)) != NULL ) {
    printf( "wait %d: ",cnt );
    tut->dumpTaskTime( p );
    cnt++;
  }
  cnt=0;
  while( (p=tut->pop(tut->head->ready)) != NULL ) {
    printf( "ready %d: ",cnt );
    tut->dumpTaskTime( p );
    cnt++;
  }
}
#endif