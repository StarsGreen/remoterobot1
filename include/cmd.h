//used to define some cmd type

#ifndef __CMD_H__
#define __CMD_H_
#endif

#define CMD_NUM 250
#define CMD_LENGTH 40

#define MOVE_DIRECT_UP 1
#define MOVE_DIRECT_DOWN 5
#define MOVE_DIRECT_RIGHT 3
#define MOVE_DIRECT_LEFT 7
#define MOVE_UP_LEFT 8
#define MOVE_UP_RIGHT 2
#define MOVE_DOWN_RIGHT 4
#define MOVE_DOWN_LEFT 6
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

#define SET_V_GET_ON 0X00005001
#define SET_V_GET_OFF 0X00005000
#define SET_V_SEND_ON 0X00001001
#define SET_V_SEND_OFF 0X00001000
#define INFO_GET_ON 0X00002001
#define INFO_GET_OFF 0X00002000
#define INFO_SEND_ON 0X00003001
#define INFO_SEND_OFF 0X00003000

#define MOVE_CTRL_OFF 0X00004000
#define MOVE_CTRL_ON 0X00004001

#define GET_MOVE_INFO 0X10000000
#define GET_MOVE_ACCEL 0X10001000
#define GET_MOVE_VEL 0X10002000
#define GET_MOVE_JOURNEY 0X10003000
#define GET_TEMPER 0X10004000
#define GET_DIST 0X10005000


#define GET_CTRL_CMD 0xffffffe0
#define HELP_CODE 0xfffffff0
#define ERROR_CODE 0xf0f0f0f0

#define CTRL_CMD_SIZE sizeof(Ctrl_Cmd)
#define CMD_INFO_SIZE sizeof(Ctrl_Cmd)
#define MAX_CMD_LENGTH 40
/////////////////////////////////////////////
typedef struct control_cmd
{
int video_get_func;
int video_send_func;
int info_get_func;
int info_send_func;
int move_ctrl_func;
}Ctrl_Cmd;
typedef Ctrl_Cmd* Ctrl_Pointer;
Ctrl_Cmd ctrl_cmd;
Ctrl_Pointer ctrl_pointer;
/////////////////////////////////////////////
struct Cmd
{
	int cmd_code;
	void (*func)(void);
	char* func_name;
};
typedef struct cmd_info
{
	struct Cmd cmd[CMD_NUM];
	int cmd_num;;
}Cmd_Info;
typedef Cmd_Info* Cmd_Info_Pointer;
Cmd_Info cmd_info;
Cmd_Info_Pointer cmd_pointer;
/////////////////////////////////////////////
