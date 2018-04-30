#ifndef __SMART_HOME_ADAPTER_H__
#define __SMART_HOME_ADAPTER_H__

#include <stdio.h>

typedef unsigned long  permission_t;

typedef struct {
	long 		type;
	char        name[20];
	unsigned int cmd;
	permission_t permission;
}user_info_t;

user_info_t user_info[20];

#define PERM_TEMPERATURE_READ      1<<0
#define TEMPERATURE_WRITE          1<<1
#define PERM_BEEP_ON               1<<2      
#define PERM_BEEP_OFF              1<<3
#define PERM_GAS_READ              1<<4
#define PERM_GAS_WRITE             1<<5
#define PERM_LED1_ON               1<<6
#define PERM_LED1_OFF              1<<7
#define PERM_LED2_ON               1<<8
#define PERM_LED2_OFF              1<<9
#define PERM_LED3_ON               1<<10
#define PERM_LED3_OFF              1<<11
#define PERM_LED4_ON               1<<12
#define PERM_LED4_OFF              1<<13
#define PERM_ALL                   1<<31


#endif
