//312350192 Omer Eckstein
#include <stdio.h>
#include "ex1.h"
/**creating int i = 1 wich is 0x1 and then creating a char pointer to point the first byte of i
 * if it was little endian the first byte should be 0x01 = 1
 * if it was big endian the first byte should be 0x00 = 0
 *
 * @return 0 if our system is Littel Endian else return 1
 */
int is_big_endian() {
	unsigned int i = 1;
	char * firstMem = (char *) &i;
	if ((int)*firstMem == 1) {
		// Little endian
		return 0;
	}
	// Big endian
	return 1; 
}

/**by shifting left and right or right and left by 32 bits we deleting half of the long bits
 * sizeof(unsigned long) = 8 bytes
 *
 * @param x a long number
 * @param y a long number
 * @return half of x and half of y
 */
unsigned long merge_bytes(unsigned long x, unsigned long int y){
	// long is 16 bytes witch is 64 bit
	// get the left half bytes of x -> 32 left bits
	// move right shift 32 bits and then 32 right shift to get only 32 lrft bits
	unsigned long halfLeftX = x;
	halfLeftX = halfLeftX >> sizeof(unsigned long)*4;
	halfLeftX = halfLeftX << sizeof(unsigned long)*4;

	// get the right half bytes of x -> 32 right bits
	// move right shift 32 bits and then 32 left shift to get only 32 lrft bits
	unsigned long halfRighty = y;
	halfRighty = halfRighty << sizeof(unsigned long)*4;
	halfRighty = halfRighty >> sizeof(unsigned long)*4;

	// build the result
	unsigned long ret = halfLeftX + halfRighty;
	return ret;
}

/** if we are pointing the first memory of long in big endian the first char is in place 0
 * in little endian the first char will be 7 = sizeof(unsigned long) - 1
 * so we will creating a char pointer into the first memory of x
 * moving the pointer i (big or little endian like the first and second line) chars (1 byte)
 * replace the value of the pointing into b
 *
 * @param x the long
 * @param b the char we need to replace
 * @param i the index of byte we need to replace
 * @return the replaced x with b in i chars
 */
unsigned long put_byte(unsigned long x, unsigned char b, int i){
    // creating char pointer pointing the first bytes
    char * ptrChar = (char *) & x;
    //  only for little endian
    if(is_big_endian() == 0){
        i = sizeof(unsigned long) - 1 - i;
    }
    // for big endian keep thr original i
    //mooving the pointer i chars
    ptrChar += i;
    //replace the value of thr pointer into b
    *ptrChar = b;
    return x;
}