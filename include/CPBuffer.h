#ifndef __CPBUFFER_H
#define __CPBUFFER_H
#include <usrttype.h>
#include <md5api.h>

struct structCPBLen {
  long long resLen;
  long long dataLen;
  long long cpLen;
  long long version;
};
struct structCPBMeta {
  char name[256-sizeof(struct structCPBLen)-sizeof(int64)*2];
  struct structCPBLen cpbLen;
  int64 key[2];
};

class CPBuffer {
  private:
    long long int mSize;
    long long int mCP;
    long long int mRes;
    void *mpStart;
    void *mpCP;
    void *mpRes;
    char mName[256];
    int allocMem( const char *name );
    long long alSize( long long sz);
    int checkFile( const char *name );
    int mValid;
    const char *getTmpDir (void);
    int checkMeta( struct structCPBMeta* meta, int mode );
    const char *getFileName( const char *n);
    void init( long long int size, long long int cp, long long res, const char *name );

  public:
    CPBuffer();
    void newCPBuffer( long long int size, long long int cp, long long res, const char *name );
    void newCPBuffer( const char *name );
    ~CPBuffer();
    void *getBuf( long long from, long long len );
    void *getBuf( long long from, int len ) { return getBuf( from, (long long)len ); };
    int valid();
    void *attach();
    long long getSize();
    long long getOff( void *buf );
    const char *findByKey( int64 key );
  };
#endif
