#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <USRTTaskQueue.h>
#include <MapMem.h>

namespace std {

int compareByNoEarly(task_t* a, task_t* b)
{
  return (a->noE>b->noE);  
}

int compareByNoLater(task_t* a, task_t* b)
{
  return (a->noL>b->noL);  
}

int USRTTaskQueue::heapCheck(struct structHeap& h, int debug )
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

void USRTTaskQueue::dumpTaskTime( task_t* a ) {
  printf("noE: %lld -- noL: %lld\n",a->noE,a->noL);
}

void USRTTaskQueue::dumpHeap( struct structHeap& h ) {
  int i;
  for( i=0;i<h.size;i++ ) {
    printf("No %d: ",i);
    dumpTaskTime(h.heap[i]);
  }
}

task_t* USRTTaskQueue::pop( struct structHeap& h )
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

void USRTTaskQueue::down(struct structHeap& h, int index )
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

int USRTTaskQueue::insert( struct structHeap& h, generalized_memory_t *a )
{
  task_t *task = (task_t *)G2L(a);
  return insert( h, task );  
}

int USRTTaskQueue::insert( struct structHeap& h, task_t *a )
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

void USRTTaskQueue::up(struct structHeap& h, int index )
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

void USRTTaskQueue::start() {
  __raw_spin_unlock(&(wait.lock));
  __raw_spin_unlock(&(ready.lock));
}

USRTTaskQueue::USRTTaskQueue( const char *name ):USRTFifo()
{
  newUSRTFifo( name
    , (long long)(sizeof(generalized_memory_t)*HEAPSIZE*16)
    , (long long)sizeof(generalized_memory_t)
    , (long long)sizeof(struct structWorkersHead)
    );
  head = (struct structWorkersHead*)CPBuffer::attach();
  init();
  wait.func = compareByNoEarly;
  ready.func = compareByNoLater;
}

void USRTTaskQueue::init()
{
  USRTFifo::init();
  memset( ((unsigned char*)head)+sizeof(struct structFifoHead)
    , 0
    , sizeof(struct structWorkersHead)-sizeof(struct structFifoHead)
    );
}
USRTTaskQueue::USRTTaskQueue():USRTFifo()
{
}

void USRTTaskQueue::attach( const char *name )
{
  USRTFifo::attach( name );
  head = (struct structWorkersHead*)CPBuffer::attach();

  wait.func = compareByNoEarly;
  ready.func = compareByNoLater;
}

void USRTTaskQueue::lpTask2G(generalized_memory_t* gp, task_t* pt)
{
  L2G(gp,(void*)pt);
  gp->len=(long long)sizeof(task_t);
}
void USRTTaskQueue::storeHeap()
{
  int i;
  head->waitSize = wait.size;
  for( i=0;i<wait.size;i++ ) {
    lpTask2G(head->gmWait+i,wait.heap[i]);
  }
  head->readySize = ready.size;
  for( i=0;i<ready.size;i++ ) {
    lpTask2G(head->gmReady+i,ready.heap[i]);
  }
}

void USRTTaskQueue::restoreHeap()
{
  int i;
  wait.size=head->waitSize;
  for( i=0;i<wait.size;i++ ) 
    wait.heap[i]=(task_t*)G2L(head->gmWait+i);
  ready.size = head->readySize;
  for( i=0;i<ready.size;i++ ) {
    dumpGM( head->gmReady[i] );
  }
}

}
#ifdef __HEAPTEST
int main()
{
  std::USRTTaskQueue *tut = new std::USRTTaskQueue("queue0");
  USRTMem *mem = new USRTMem();
  mem->newUSRTMem( "task0"
    , (long long)sizeof(task_t)*512
    , (long long)sizeof(task_t)
    , (long long)sizeof(struct USRTMem::structMemHead)
    );
  mem->init();
  int i;
  tut->start();
  mem->start();
  for(i=0;i<HEAPSIZE;i++) {
    task_t* a=(task_t*)mem->allocMem((long long)sizeof(task_t));
    a->mem.memKey=mem->getKey();
    a->mem.offset=mem->getOff((void *)a);
    a->mem.len=(long long)sizeof(task_t);
    a->noE = (utime_t)(rand()&0xff);
    a->noL = (utime_t)(rand()&0xff);
    if( tut->insert(tut->wait,&(a->mem))!=0 ) printf("Insert wait error %d\n",tut->wait.size);
    if( tut->insert(tut->ready,&(a->mem)) !=0 ) printf("Insert ready error %d\n",tut->ready.size);
    tut->heapCheck(tut->wait,254);
    tut->heapCheck(tut->ready,254);
  }
  task_t *p;
  int cnt=0;
  while( (p=tut->pop(tut->ready)) != NULL ) {
    generalized_memory_t g;
    L2G(&g,(void *)p);
    g.len = (long long)sizeof(task_t);
    printf( "ready %d: ",cnt );
    dumpGM(g);
    tut->dumpTaskTime( p );
    cnt++;
  }
  tut->storeHeap();
  delete mem;
  delete tut;
}
#endif