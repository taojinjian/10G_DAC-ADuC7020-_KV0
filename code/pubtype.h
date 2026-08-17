/***********************************************************
** Semtech Co.Ltd CopyRight
**
**File: pubtype.h
**
**Des: 1-type def
**
**Author: Hani , created in 29/12/2014 Rev1.0
**
**Rev: xxxx, Modifed in ..................... Rev1.x
**
**
***********************************************************/
#ifndef _PUB_TYPE_H_
#define _PUB_TYPE_H_
 
#include <stdint.h>

typedef  char               Int8_PUB;
typedef  short              Int16_PUB;
typedef  int                Int32_PUB;   
typedef  unsigned char      uInt8_PUB;
typedef  unsigned short     uInt16_PUB;
typedef  unsigned int       uInt32_PUB;   

typedef  unsigned char               Bool_PUB;
typedef  unsigned char      Byte_PUB;

extern volatile uint16_t  I2CBusy;
    
#define TRUE_PUB			(1)		
#define FALSE_PUB			(0)
    
    
#ifndef NULL
#define NULL		(0)
#endif
#endif


