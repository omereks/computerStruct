//312350192 Omer Eckstein
#include <stdio.h>
#include "ex1.h"

int is_big_endian() {
	unsigned int i = 1;
	char *firstDig = (char*) &i;
	if (firstDig[0] == 1) {	
		// Litel endian
		return 0;
	}
	// Big endian
	return 1; 
}

unsigned long merge_bytes(unsigned long x, unsigned long int y){
	// long is 16 bytes witch is 64 bit
	// get the left half bytes of x -> 32 left bits
	// move right shift 32 bits and then 32 right shift to get only 32 lrft bits
	unsigned long halfLeftX = x;
	halfLeftX = halfLeftX >> 32;
	halfLeftX = halfLeftX << 32;

	// get the right half bytes of x -> 32 right bits
	// move right shift 32 bits and then 32 left shift to get only 32 lrft bits
	unsigned long halfRighty = y;
	halfRighty = halfRighty << 32;
	halfRighty = halfRighty >> 32;

	// build the result
	unsigned long ret = halfLeftX + halfRighty;
	return ret;
}





unsigned long put_byte(unsigned long x, unsigned char b, int i){
	
	unsigned long y = 0x12345678CDEF3456;
	y = y<<8
	y
	
	/**
	 * unsigned long left = x >> ((64 -i) * 8);
	printf("0x%lx\n", left);
	left = x << 8;
	printf("0x%lx\n", left);
	
	//place of i in bits
	//i = i*8;

	//shifting b to right place
	//unsigned long bInTheRightPlace = ((unsigned long)b) << (64-i);
	//printf("0x%lx\n", bInTheRightPlace);

	//getting the bits of left from b
	unsigned long right = x << (i+1)*8;
	right = right >> (i+1)*8;
	printf("0x%lx\n", right);
	//printf("0x%lx\n", right + bInTheRightPlace);

	/////////////////////////////////////////////////put_byte(0x12345678CDEF3456, 0xAB, 2);
	/////////////////////////////////////////////////--> 0x1234AB78CDEF3456
	/////////////////////////////////////////////////-->    AB  10
**/

	//printf("0x%lx\n", x);
	//unsigned long y = x<<16;
	//printf("0x%lx\n", y);
	//printf("0x%lx\n", (unsigned long)b);
	//printf("0x%lx\n", y+b);
}

