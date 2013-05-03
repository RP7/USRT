#ifndef capability_Bearer_H
#define capability_Bearer_H
#include <capability.h>
#include <usrttype.h>
#include <dlfcn.h>

class USRTCapabilityBearer {
  typedef CCapability(*factroyFunc)();
  typedef void(*getKeyFunc)(CCapability* item,long long int *k);
  typedef void(*runFunc)(CCapability* item,void *argv);
  typedef void(*destroyFunc)(CCapability* item);
  private:
    void init();
    int getHandle(const char* lib);
    char mName[256];
    int mValid;
    factroyFunc mFactroy;
    getKeyFunc mGetKey;
    runFunc mRun;
    destroyFunc mDistroy;
    void *handle;
    int64 key;
    CCapability* item;
  public:
    USRTCapabilityBearer( const char* lib );
    USRTCapabilityBearer( int64 key );
    void run(generalized_memory_t* argv);
    int64 getKey() { return key; };
    int isValid() { return mValid; };
    ~USRTCapabilityBearer();
};

#endif //Capability_Bearer_H

