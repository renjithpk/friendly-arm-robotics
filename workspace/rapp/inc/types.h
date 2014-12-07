/*
 * types.h
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef signed char        int8_t;
typedef unsigned char      uint8_t;

typedef short              int16_t;
typedef unsigned short     uint16_t;

//typedef long int              int32_t;
//typedef unsigned long      uint32_t;

typedef long long          int64_t;
typedef unsigned long long uint64_t;

//#define DBG_ON

#ifdef DBG_ON
	#define DBG_PRINT(x) cout<<x<<endl
#else
	#define DBG_PRINT(x)
#endif

#endif /* TYPES_H_ */
