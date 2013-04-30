#ifndef __CPBUFFER_H
#define __CPBUFFER_H
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
  public:
    CPBuffer( long long int size, long long int cp, long long res, const char *name );
    ~CPBuffer();
    void *getBuf( long long from, long long len );
    void *getBuf( long long from, int len ) { return getBuf( from, (long long)len ); };
    int valid();
    void *attach();
    long long getSize();
    long long getOff( void *buf );
  };
#endif
