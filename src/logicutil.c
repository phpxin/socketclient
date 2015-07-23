#include "apue.h"
#include "logicutil.h"

int bl_or_ll(){
     union{
          short value ;
          char union_bytes[sizeof(short)] ;
     } test;

     test.value = 0x0102;

     if( (test.union_bytes[0]==1) && (test.union_bytes[1]==2) ){

    	 return 1; /* big */
     }

     else if( (test.union_bytes[1]==1) && (test.union_bytes[0]==2) ){

    	 return 2; /* little */
     }

     return 0;
}

int int_to_net(int value){
	if(bl_or_ll()==1){
		return value;
	}

	return int_bl_endian(value);
}

int net_to_int(int value){
	if(bl_or_ll()==1){
		return value;
	}

	return int_bl_endian(value);
}

int int_bl_endian(int x)
{
    int tmp;
    tmp = (((x)&0xff)<<24) + (((x>>8)&0xff)<<16) + (((x>>16)&0xff)<<8) + (((x>>24)&0xff));
    return tmp;
}

