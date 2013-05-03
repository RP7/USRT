#ifndef __capability_H
#define __capability_H

class CCapability {
	public:
		CCapability(){};
		~CCapability(){};
		virtual long long int getKey() { return -1LL; };
		virtual int run( void *argv ) { return -1; };
		virtual int destroy() { return -1; };
};
#endif		