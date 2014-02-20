#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define assert(test,message) {if(!(test)) {printf("Assertion failed in file %s, on line %i: %s\n", __FILE__, __LINE__, message); __asm( "int $3" );}}
