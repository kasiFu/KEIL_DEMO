#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

#define TRUE     1
#define FALSE    0
#define NULL_PTR ((void *)0)

#ifdef EN_ASSERT
#define ASSERT(xValue)\
do{\
	if(xValue)\
	{\
		while(1){}\
	}\
}while(0)
#else
#define ASSERT(xValue)
#endif

#endif
