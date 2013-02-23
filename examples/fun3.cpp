#include<capability.h>
#include<stdio.h>
#include "fun3.h"

extern "C" {
	CCapability *newFun() {
		exampleFun3 *item = new exampleFun3();
		return (CCapability*)item;
	}
	void run( CCapability* item, void *argv ) {
		(exampleFun3 *)item->run( argv );
	}
	void getKey( CCapability* item,long long int *k){
		(exampleFun3 *)item->getKey( k );
	}
	void destroy( CCapability* item ) {
		(exampleFun3 *)item->destroy();
	}
};	


long long int exampleFun3::key[] = {0x1318bb259d65ee16LL,0x7e95373c35421368LL};
int exampleFun3::getKey( long long int *k ) {
	k[0]=key[0];
	k[1]=key[1];
	return 1;
}

int exampleFun3::run( void *argv ) {
	printf("I am 3\n");
	return 1;
}

int exampleFun3::destroy( ) {
	delete this;
	return 1;
}

