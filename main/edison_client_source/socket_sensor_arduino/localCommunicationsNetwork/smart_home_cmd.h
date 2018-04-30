#ifndef MYLED_BEEP_CNTL_H
#define MYLED_BEEP_CNTL_H

#include <asm-generic/ioctl.h>
#define LED 'L'
#define BEEP 'B'
#define PERMISSION 'P'



//权限标识
#define PERMITTED _IO(PERMISSION,1)
#define NO_PERMISSION _IO (PERMISSION,2)
#define PERMISSION_ERR _IO(PERMISSION,0)
#define NO_USR _IO(PERMISSION,3)
//struct msg_t  中type 、 device下设备文件和C++主线程均使用使用以下宏定义
//
//LED master
#define LED1_ON  _IO(LED, 0)
#define LED1_OFF _IO(LED, 1)
#define LED2_ON  _IO(LED, 2)
#define LED2_OFF _IO(LED, 3)
#define LED3_ON  _IO(LED, 4)
#define LED3_OFF _IO(LED, 5)
#define LED4_ON  _IO(LED, 6)
#define LED4_OFF _IO(LED, 7)
#define LEDALL_ON _IO(LED,8)
#define LEDALL_OFF _IO(LED,9)



//BEEP master
#define BEEP_ON  _IO(BEEP, 0)
#define BEEP_OFF _IO(BEEP, 1)

//temperature master
#define READ_TEMPERATURE _IO('S',3)
#define WRITE_TEMPERATURE _IO('S',4)

//light master
#define READ_LIGHT _IO('S',5)
#define WRITE_LIGHT _IO('S',6)

//gas master
#define READ_GAS _IO('S',7)

//shake master
#define READ_SHAKE _IO('S',8)

//humidity socket
#define READ_HUMDIDITY _IO('S',9)

//sound master
#define READ_SOUND _IO('S',10)

//vibration master
#define READ_VIBRATION _IO('R',1)

//touch master
#define READ_TOUCH _IO('R',2)

//socket master
#define SOCKET_CONNECT_CLOSE _IO('S',0)
#define SOCKET_CONNECT_SLEEP _IO('S',1)
#define SOCKET_CONNECT_REALTIME _IO('K',2)
#define SOCKET_CONNECT_SEND_CMD _IO('K',3)
#define SOCKET_CONNECT_READY_CMD _IO('K',4)
#endif
