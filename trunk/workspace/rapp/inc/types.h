/*
 * types.h
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#ifndef TYPES_H_
#define TYPES_H_
#include <stdio.h>
#include <syslog.h>

typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef long long          int64_t;
typedef unsigned long long uint64_t;
class App;
extern App *app_gp;

enum EMessageT
{
 HEADER_IMAGE = 100,
 HEADER_RESULT,
 RQ_OBJ_DETECT,
 DTD_OBJ_FACE,
 DTD_OBJ_NOBALL,
 DTD_OBJ_BALL,
 DTD_OBJ_M_BALL
};
struct Header
{
	EMessageT type;
	int size;
};

class RCircle
{
	public:
	
	RCircle():x(0),y(0),r(0),count(0)
	{}
	RCircle(int val):x(val),y(val),r(val),count(val)
	{}
		int x;
		int y;
		int r;
		int count;
};

struct MSG_Circle
{
	Header header;
	RCircle rCircle;
};


struct FacePos
{
	uint8_t _x;
	uint8_t _y;
	uint8_t _L;
	uint8_t _B;
};


/*
LOG_EMERG      system is unusable
LOG_ALERT      action must be taken immediately
LOG_CRIT       critical conditions
LOG_ERR        error conditions
LOG_WARNING    warning conditions
LOG_NOTICE     normal, but significant, condition
LOG_INFO       informational message
LOG_DEBUG      debug-level message
*/

/*
#define INFO_ON
//#define DEBUG_ON
//#define WARNING_ON
#define ERROR_ON

#ifdef INFO_ON
	#define PRINT_1(...) printf(__VA_ARGS__);printf("\n")
#else
	#define PRINT_1(...)
#endif

#ifdef DEBUG_ON
	#define PRINT_2(...) printf( __VA_ARGS__);printf("\n")
#else
	#define PRINT_2(...)
#endif

#ifdef WARNING_ON
	#define PRINT_3(...) printf(__VA_ARGS__);printf("\n")
#else
	#define PRINT_3(...)
#endif
#define PRINT_4(...) printf(__VA_ARGS__);printf("\n")
*/
#endif /* TYPES_H_ */
