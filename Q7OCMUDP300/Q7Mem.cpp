#include <map>
#include <string>
#include <CPBuffer.h>
#include "Q7Mem.h"

Q7Mem::Q7Mem():CPBuffer()
{
}
void Q7Mem::dumpHead()
{
  printf("mem file name: %s\n",getName());
  printf("mem off: %lld\n",head->_off);
}
void Q7Mem::attach( const char *n )
{
  newCPBuffer( n );
  head = (struct structQ7MemHead*)CPBuffer::attach();
}

void Q7Mem::newQ7Mem( const char *n, long long dataL, long long cpL, long long resL )
{
  newCPBuffer( dataL
    , cpL
    , resL
    , n
    );
  head = (struct structQ7MemHead*)CPBuffer::attach();
}
void Q7Mem::init()
{
  memset( ((unsigned char*)head)+sizeof(struct structCPBMeta)
    , 0
    , sizeof(struct structQ7MemHead)-sizeof(struct structCPBMeta)
    );
}

const char* Q7Mem::getName()
{
  return head->meta.name;
}

Q7Mem::~Q7Mem()
{ 
}

void Q7Mem::start()
{ 
}

int64 Q7Mem::getKey()
{
  return head->meta.key[0];
}