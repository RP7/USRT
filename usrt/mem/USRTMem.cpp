#include <map>
#include <string>
#include <CPBuffer.h>
#include <USRTMem.h>

USRTMem::USRTMem():CPBuffer()
{
}
void USRTMem::dumpHead()
{
  printf("mem file name: %s\n",getName());
  printf("mem brk: %lld\n",head->_brk);
  if( head->lockM.slock==1 )
    printf("Mem lock is unlock\n");
  else
    printf("Mem lock is lock\n");
}
void USRTMem::attach( const char *n )
{
  newCPBuffer( n );
  head = (struct structMemHead*)CPBuffer::attach();
}

void USRTMem::newUSRTMem( const char *n, long long dataL, long long cpL, long long resL )
{
  newCPBuffer( dataL
    , cpL
    , resL
    , n
    );
  head = (struct structMemHead*)CPBuffer::attach();
}
void USRTMem::init()
{
  memset( ((unsigned char*)head)+sizeof(struct structCPBMeta)
    , 0
    , sizeof(struct structMemHead)-sizeof(struct structCPBMeta)
    );
}

const char* USRTMem::getName()
{
  return head->meta.name;
}

USRTMem::~USRTMem()
{ 
}

void USRTMem::start()
{ 
  __raw_spin_unlock(&(head->lockM));
}

void *USRTMem::allocMem( long long len )
{
  __raw_spin_lock(&(head->lockM));
  long long last = head->_brk;
  head->_brk+=len;
  __raw_spin_unlock(&(head->lockM));
  return getBuf(last,len);
}

void *USRTMem::allocMemAlign( long long len, int align )
{
  long long _align = (long long)align;
  __raw_spin_lock(&(head->lockM));
  long long last = ((head->_brk+_align-1)/_align)*_align;
  head->_brk=last+len;
  __raw_spin_unlock(&(head->lockM));
  return getBuf(last,len);
}

int64 USRTMem::getKey()
{
  return head->meta.key[0];
}
