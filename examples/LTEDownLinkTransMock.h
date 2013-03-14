void *dummyArgv();
void *buildPss();
void *buildPssAntMap();
void *buildUeDownCoding(int i);
void *buildUeDownMod(int i);
void *buildUeDownAntMap(int i);
void keyPss(int64 key[2]);
void keyPssAntMap(int64 key[2]);
void keyUeDownCoding(int64 key[2]);
void keyUeDownMod(int64 key[2]);
void keyUeDownAntMap(int64 key[2]);
#include <usrttype.h>

utime_t getHardware();
utime_t getSubFrameDuration();
utime_t getNow();
utime_t getFrameTiming(int frame, int subframe );

