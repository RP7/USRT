#include <usrttype.h>

extern "C" {
  int64 newKey( const char*name, int session, short *sn );
  int64 newE( const char*name, int session );
  int64 newV( const char*name, int session );
};
