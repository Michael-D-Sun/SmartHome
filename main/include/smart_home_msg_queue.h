#ifndef _SMART_HOME_MSG_QUEUE_H
#define _SMART_HOME_MSG_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif
 int cntl_led_from_cgi(int argv);
 int cntl_beep_from_cgi(int argv);
 int cntl_mpu6050_thread(void);
 int detector_mythread(int argv);

#ifdef __cplusplus
}
#endif

#endif
