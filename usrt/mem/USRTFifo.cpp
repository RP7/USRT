#include <map>
#include <string>
#include <USRTMem.h>
#include <USRTFifo.h>
#include <task.h>

void USRTFifo::dumpHead()
{
  USRTMem::dumpHead();
  printf("write point: %d\n",head->sp);
  printf("read point: %d\n",head->rp);    
  if( head->lockF.slock==1 )
    printf("stack lock is unlock\n");
  else
    printf("stack lock is lock\n");
  int bp=head->rp;
  int cnt=0;
  while( bp != head->sp ) {
    printf("%4.4llx ",head->fifo[bp]&0xffff);
    bp++;
    if( bp>=FIFOSIZE )
      bp=0;
    cnt++;
    if( (cnt&0x7)==0 )
      printf("\n");
  }
  printf("\n");
}

USRTFifo::USRTFifo():USRTMem()
{
}

void USRTFifo::attach( const char *n )
{
  USRTMem::attach( n );
  head = (struct structFifoHead*)CPBuffer::attach();
}

void USRTFifo::newUSRTFifo( const char *n, long long dataL, long long cpL, long long resL )
{
  newUSRTMem( n
    , dataL
    , cpL
    , resL
    );
  head = (struct structFifoHead*)CPBuffer::attach();
}
void USRTFifo::init()
{
  USRTMem::init();
  memset( ((unsigned char*)head)+sizeof(struct structMemHead)
    , 0
    , sizeof(struct structFifoHead)-sizeof(struct structMemHead)
    );
}


USRTFifo::~USRTFifo()
{ 
}

int USRTFifo::len()
{ 
  int ret = (head->sp-head->rp+FIFOSIZE)%FIFOSIZE;
//  fprintf(stderr,"Len(%s) = %d",head->mem.meta.name,ret);
  return ret;
}

void USRTFifo::start()
{ 
  __raw_spin_unlock(&(head->lockF));
  USRTMem::start();
}

void USRTFifo::pushOff( long long t )
{
  __raw_spin_lock(&(head->lockF));
  if( len()==FIFOSIZE-1 ) {
    head->rp++;
    if( head->rp>=FIFOSIZE )
      head->rp = 0;
  }
  head->fifo[head->sp]=t;
  head->sp++;
  if( head->sp>=FIFOSIZE )
    head->sp = 0;
  __raw_spin_unlock(&(head->lockF));
}


void USRTFifo::push( void* t )
{
  pushOff(getOff(t));
}

void* USRTFifo::get()
{
  void *ret = NULL;
  __raw_spin_lock(&(head->lockF));
  if( head->rp!=head->sp ) {
    ret = getBuf(head->fifo[head->rp],1);
    head->rp++;
    if( head->rp>=FIFOSIZE )
      head->rp = 0;
  }
  __raw_spin_unlock(&(head->lockF));
  return ret;
}
