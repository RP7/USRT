#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <sys/time.h>
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
  fprintf(stderr,"noE: %lld -- noL: %lld\n",a->noE,a->noL);
}
void USRTTaskQueue::dumpHeap()
{
    fprintf(stderr,"Wait Heap\n");
    dumpHeap(wait);
    fprintf(stderr,"Ready Heap\n");
    dumpHeap(ready);
}
void USRTTaskQueue::dumpHeap( struct structHeap& h ) {
  int i;
  for( i=0;i<h.size;i++ ) {
    fprintf(stderr,"No %d: ",i);
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

int USRTTaskQueue::insert( generalized_memory_t *a )
{
  task_t *task = (task_t *)G2L(a);
  if( wait.size<HEAPSIZE-1 )
    return insert( wait, task );
  else
    return -1;  
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

int USRTTaskQueue::del( struct structHeap& h, task_t *a )
{
  int ret = -1;
  if( h.size==0 ) 
    return ret;
  __raw_spin_lock(&(h.lock));
  int i;
  for( i=0;i<h.size;i++ ) {
    if( h.heap[i]==a )
      break;
  }
  if( i!=h.size ) {
    h.size--;
    h.heap[i]=h.heap[h.size];
    down(h,i);
    ret = 0;
  }
  __raw_spin_unlock(&(h.lock));
  return ret;
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
  __raw_spin_unlock(&criticalArea);
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
int USRTTaskQueue::update() 
{
  int cnt=0;
  if( criticalArea.slock ) {
    __raw_spin_lock(&criticalArea);
    card.noE = getNow();
    insert(wait,&card);
    task_t *t;
    while( ready.size<HEAPSIZE-1 ) {
      t=pop(wait);
      if( t==NULL )
        break;
      if( t==&card )
        break;
      insert(ready,t);
      cnt++;
    }
    if( t!=&card )
      del(wait,&card);
    __raw_spin_unlock(&criticalArea);
  }    
  return cnt;
}
utime_t USRTTaskQueue::getNow()
{
	struct timeval tv;
	gettimeofday(&tv,0);
	utime_t now = (utime_t)tv.tv_sec*1000000;
	now += (utime_t)tv.tv_usec;
	return now;
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
  for(i=0;i<HEAPSIZE-1;i++) {
    task_t* a=(task_t*)mem->allocMem((long long)sizeof(task_t));
    a->mem.memKey=mem->getKey();
    a->mem.offset=mem->getOff((void *)a);
    a->mem.len=(long long)sizeof(task_t);
    a->noE = tut->getNow();
    a->noL = tut->getNow();
    if( tut->insert(&(a->mem))!=0 ) printf("Insert wait error %d\n",tut->wait.size);
    tut->heapCheck(tut->wait,253);
  }
  tut->storeHeap();
  int updateTask = tut->update();
  printf( "Update task %d\n",updateTask);
  tut->heapCheck(tut->ready,253);
  task_t *p;
  int cnt=0;
  while( (p=tut->pop()) != NULL ) {
    generalized_memory_t g;
    L2G(&g,(void *)p);
    g.len = (long long)sizeof(task_t);
    printf( "ready %d: ",cnt );
    dumpGM(g);
    tut->dumpTaskTime( p );
    cnt++;
  }
  delete mem;
  delete tut;
}
#endif