//used to define some cmd type
//#include "data_structure.h"
#ifndef __CMD_H__
#define __CMD_H_
#endif

#define CMD_NUM 250
#define CMD_LENGTH 40

#define MOVE_DIRECT_UP 7
#define MOVE_DIRECT_DOWN 3
#define MOVE_DIRECT_RIGHT 1
#define MOVE_DIRECT_LEFT 5
#define MOVE_UP_LEFT 6
#define MOVE_UP_RIGHT 8
#define MOVE_DOWN_RIGHT 2
#define MOVE_DOWN_LEFT 4
#define MOVE_DIRECT_STOP 0

#define INFO_SEND_ENABLE 1
#define INFO_SEND_DISABLE 0

#define INFO_GET_ENABLE 1
#define INFO_GET_DISABLE 0

#define VIDEO_SEND_ENABLE 1
#define VIDEO_SEND_DISABLE 0

#define VIDEO_GET_ENABLE 1
#define VIDEO_GET_DISABLE 0

#define MOVE_CTRL_ENABLE 1
#define MOVE_CTRL_DISABLE 0

#define SET_V_GET_ON 0X00005010
#define SET_V_GET_OFF 0X00005000
#define SET_V_SEND_ON 0X00001010
#define SET_V_SEND_OFF 0X00001000
#define INFO_GET_ON 0X00002010
#define INFO_GET_OFF 0X00002000
#define INFO_SEND_ON 0X00003010
#define INFO_SEND_OFF 0X00003000
#define SET_LED_RATE 0X00006000
#define MOVE_CTRL_OFF 0X00004000
#define MOVE_CTRL_ON 0X00004010

#define GET_MOVE_INFO 0X10000000
#define GET_MOVE_ACCEL 0X10001000
#define GET_MOVE_VEL 0X10002000
#define GET_MOVE_JOURNEY 0X10003000
#define GET_TEMPER 0X10004000
#define GET_DIST 0X10005000
#define GET_MOVE_POS 0X10006000
#define GET_GRA_CPT 0X10007000

#define MOVE_UP 0x20000010
#define MOVE_RIGHT 0x20000020
#define MOVE_DOWN 0x20000030
#define MOVE_LEFT 0x20000040
#define MOVE_STOP 0x200000f0


#define ALL_CTRL_FUNC_ON 0x0000f010
#define ALL_CTRL_FUNC_OFF 0x0000f000
#define GET_CTRL_CMD 0xffffffe0
#define HELP_CODE 0xfffffff0
#define ERROR_CODE 0xf0f0f0f0

#define MAX_CMD_LENGTH 40
/////////////////////////////////////////////
