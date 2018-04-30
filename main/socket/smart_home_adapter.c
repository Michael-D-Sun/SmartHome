
#include "smart_home_adapter.h"
#include "smart_home_cmd.h"
#include <string.h>

int flagnum = 5;
unsigned long  smart_home_adapter (const char *name,const  int request)
{
	permission_t ret;
	int i;

	for(i = 0 ;i <flagnum ;++i)
	{
		if(0 == strcmp(name,user_info[i].name))
		{
			if(user_info->permission == PERM_ALL)
			{
				return PERMITTED;
			}
			switch (user_info->permission){
			case LED1_ON:
				if(0 != (user_info->permission & PERM_LED1_ON))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;

			case LED1_OFF:
				if(0 != (user_info->permission & PERM_LED1_OFF))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			case LED2_ON:
				if(0 != (user_info->permission & PERM_LED2_ON))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			case LED2_OFF:
				if(0 != (user_info->permission & PERM_LED2_OFF))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			case LED3_ON:
				if(0 != (user_info->permission & PERM_LED3_ON))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			case LED3_OFF:
				if(0 != (user_info->permission & PERM_LED3_OFF))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			case LED4_ON:
				if(0 != (user_info->permission & PERM_LED4_ON))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;

			case LED4_OFF:
				if(0 != (user_info->permission & PERM_LED4_OFF))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			case BEEP_ON:
				if(0 != (user_info->permission & PERM_BEEP_ON))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			case BEEP_OFF:
				if(0 != (user_info->permission & PERM_BEEP_OFF))
					return PERMITTED;
				else 
					return NO_PERMISSION;
				break;
			default:
#ifndef DEBUG
				printf("Oops no such a device!");
#endif
				return PERMISSION_ERR;
			}
		}
	}


	if(i == flagnum)
	{
#ifdef DEBUG
		printf("Oops no such a user!");
#endif
		return NO_USR;
	}

	return PERMISSION_ERR;


}
