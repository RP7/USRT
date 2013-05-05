#ifndef capability_Bearer_H
#define capability_Bearer_H

#include <capability.h>
#include <usrttype.h>
#include <dlfcn.h>

class USRTCapabilityBearer {
  typedef CCapability*(*factroyFunc)();
  typedef int64(*getKeyFunc)(CCapability* item);
  typedef void(*runFunc)(CCapability* item,void *argv);
  typedef void(*destroyFunc)(CCapability* item);
  private:
    void init();
    const char *getFileName( const char *n);
    const char *getWorkingDir (void);
    void *getFunc(const char *sym);
    int getHandle(const char* lib);
    char mName[256];
    int mValid;
    factroyFunc mFactroy;
    getKeyFunc mGetKey;
    runFunc mRun;
    destroyFunc mDestroy;
    void *handle;
    int64 key;
    CCapability* item;
  public:
    USRTCapabilityBearer( const char* lib );
    USRTCapabilityBearer( int64 key );
    void runGP(generalized_memory_t* argv);
    void runLP(void* argv);
    int64 getKey() { return key; };
    int isValid() { return mValid; };
    const char *getName() { return mName; };
    ~USRTCapabilityBearer();
};

#endif //Capability_Bearer_H

