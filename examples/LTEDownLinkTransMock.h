void *dummyArgv();
void *buildPss();
void *buildPssAntMap();
void *buildUeDownCoding(int i);
void *buildUeDownMod(int i);
void *buildUeDownAntMap(int i);
int64 keyPss();
int64 keyPssAntMap();
int64 keyUeDownCoding();
int64 keyUeDownMod();
int64 keyUeDownAntMap();
#include <usrttype.h>

utime_t getHardware();
utime_t getSubFrameDuration();
utime_t getNow();
utime_t getFrameTiming(int frame, int subframe );

