/*
 * types.h
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#ifndef TYPES_H_
#define TYPES_H_
#include <stdio.h>
typedef signed char        int8_t;
typedef unsigned char      uint8_t;

typedef short              int16_t;
typedef unsigned short     uint16_t;

//typedef long int              int32_t;
//typedef unsigned long      uint32_t;

typedef long long          int64_t;
typedef unsigned long long uint64_t;

#define INFO_ON
//#define DEBUG_ON
//#define WARNING_ON
#define ERROR_ON

#ifdef INFO_ON
	#define PRINT_1(...) printf(__VA_ARGS__);printf("\n")
#else
	#define PRINT_1(x)
#endif

#ifdef DEBUG_ON
	#define PRINT_2(...) printf( __VA_ARGS__);printf("\n")
#else
	#define PRINT_2(x)
#endif

#ifdef WARNING_ON
	#define PRINT_3(...) printf(__VA_ARGS__);printf("\n")
#else
	#define PRINT_3(x)
#endif
#define PRINT_4(...) printf(__VA_ARGS__);printf("\n")
#endif /* TYPES_H_ */
