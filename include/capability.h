#ifndef __capability_H
#define __capability_H

class CCapability {
	public:
		CCapability(){};
		~CCapability(){};
		virtual int getKey( long long * k ) { return -1; };
		virtual int run( void *argv ) { return -1; };
		virtual int destroy() { return -1; };
};
#endif		