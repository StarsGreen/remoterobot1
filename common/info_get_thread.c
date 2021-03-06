#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include "data_structure.h"
#include "data_config.h"
//#include "cmd.h"

extern float temper_read();
extern float dist_read();
extern float xa_read(int fd);
extern float ya_read(int fd);
extern float za_read(int fd);
extern float xl_read(int fd);
extern float yl_read(int fd);
extern float zl_read(int fd);
extern int init_mpu6050();
//extern void init_dist_sensor();
//extern void get_move_info();

extern float kalman_filter(float last_result,float last_value,float cur_value,
float time,float* p_next,float Q_offset,float R_offset);
extern float slide_filter(float cur_value,float last_value);
extern void mlist_add(M_Node node);
extern void mlist_clear(void);
extern void* get_ll_shmid(key_t key,int size);
extern int rebuild_mlist();
extern sem_t sensor_start,sensor_mid,sensor_stop;

float pxl_conv=0.5;
float pyl_conv=0.5;
float pzl_conv=0.5;
float pxa_conv=0.5;
float pya_conv=0.5;
float pza_conv=0.5;
const float Q_offset=0.5;
const float R_offset=0.25;
static float dt=0;

M_Node M_info;
M_Pointer M_info_pointer;
//extern int cond;
//extern pthread_mutex_t thread_mutex;
//extern pthread_cond_t thread_cond;
///////////////////////////////////////////////
static void sensor_cleanup_handler(void *arg)
{
//	sem_destroy(&sensor_start);

//	sem_destroy(&sensor_mid);

//	sem_destroy(&sensor_stop);
}

////////////////////////////////////////////////
void* temper_get_thread(void)
{
	float value=0;
	int num=0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
while(1)
	{
	start:
		pthread_testcancel();
//		sem_wait(&sensor_start);
//		while(ctrl_cmd.info_get_func==INFO_GET_DISABLE);
                usleep(50000);
		value=temper_read();
	if(value!=0)
		{
		M_info.temper=value;

//		printf("the temper is %6.3f\n",value);
  value=0;
		}
	else
		{
		num++;
		if(num==10)
		  {
		printf("can not get the temper data\n");
		num=0;
		goto nothing;
		  }
		goto start;
		}
//		sem_post(&sensor_mid);
//printf("temper get finished!\n");
//		pthread_testcancel();
	}
nothing:
//	sem_post(&sensor_mid);
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
////////////////////////////////////////////////
void* dist_get_thread(void)
{
	float value;
	int num=0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
//	init_dist_sensor();
while(1)
	{
	start:
		pthread_testcancel();
//		sem_wait(&sensor_mid);
		usleep(50000);
		value=dist_read();
//		printf("the dist is %6.3f\n",value);

		if(value>0)
		{
		M_info.dist=value;
  value=0;
//		printf("the dist is %6.3f\n",value);
		}
		else
		{
		num++;
		if(num==10)
		  {
		num=0;
		printf("can not get the dist data\n");
		goto nothing;
		  }
		goto start;
		}
//		sem_post(&sensor_stop);
//printf("dist get finished!\n");
//		pthread_testcancel();
	}
nothing:
//	sem_post(&sensor_start);
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
//////////////////////////////////////////////
void* accel_get_thread(void)
{
	int num=0;
	int fd;
	static float gra_x=0;
        static float gra_y=0;
        static float gra_z=0;
	mll_ptr p=NULL;
	M_Pointer tail=NULL;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler,NULL);
        struct timeval t1,t2;  // 结构体，可以记录秒和微秒两部分值
	memset(&t1,0,sizeof(t1));
        memset(&t2,0,sizeof(t2));
        gettimeofday(&t1, NULL);
	memcpy(&t2,&t1,sizeof(t1));
start:
	fd=init_mpu6050();
	gra_x=xl_read(fd);
	gra_y=yl_read(fd);
	gra_z=zl_read(fd);
//printf("\nthe initial gravity component is: %f|%f|%f\n",gra_x,gra_y,gra_z);
while(1)
	{
	pthread_testcancel();
	usleep(50000);
//		sem_wait(&sensor_stop);
//		usleep(100000);
//	struct timeval t1;  // 结构体，可以记录秒和微秒两部分值
//	gettimeofday(&t1, NULL);
	gettimeofday(&t1, NULL);
	long start, stop;//换算为微秒
	start = t2.tv_sec*1000000+t2.tv_usec; // 开始时刻
	stop = t1.tv_sec*1000000+t1.tv_usec;  // 结束时刻计算距离
	dt=(float)(stop - start)/1000000;
	memcpy(&t2,&t1,sizeof(t1));

	M_info_pointer=(M_Pointer)malloc(M_NODE_SIZE);
	memset(M_info_pointer,0,M_NODE_SIZE);

	p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	memcpy(M_info_pointer,tail,M_NODE_SIZE);
	pthread_mutex_unlock(&p->move_ll_lock);

	M_info.sample_time=dt;

	if(fd!=-1)
	    {
	M_info.vel_info.xa_vel=slide_filter(xa_read(fd),
	M_info_pointer->vel_info.xa_vel);
	M_info.vel_info.ya_vel=slide_filter(ya_read(fd),
	M_info_pointer->vel_info.ya_vel);
	M_info.vel_info.za_vel=slide_filter(za_read(fd),
	M_info_pointer->vel_info.za_vel);

	M_info.accel_info.xl_accel=slide_filter(xl_read(fd)-gra_x,
	M_info_pointer->accel_info.xl_accel);
	M_info.accel_info.yl_accel=slide_filter(yl_read(fd)-gra_y,
        M_info_pointer->accel_info.yl_accel);
	M_info.accel_info.zl_accel=slide_filter(zl_read(fd)-gra_z,
        M_info_pointer->accel_info.zl_accel);
	    }
	else
	    {
	num++;
	if(num==10)
	      {
	printf("the dev is not found\n");
	goto nothing;
	      }
	goto start;
     	    }
   if(M_info_pointer!=NULL){

M_info.vel_info.xl_vel=M_info_pointer->vel_info.xl_vel+
M_info.accel_info.xl_accel*dt;
M_info.vel_info.yl_vel=M_info_pointer->vel_info.yl_vel+
M_info.accel_info.yl_accel*dt;
M_info.vel_info.zl_vel=M_info_pointer->vel_info.zl_vel+
M_info.accel_info.zl_accel*dt;

M_info.jour_info.xa=M_info_pointer->jour_info.xa+
M_info.vel_info.xa_vel*dt;
M_info.jour_info.ya=M_info_pointer->jour_info.ya+
M_info.vel_info.ya_vel*dt;
M_info.jour_info.za=M_info_pointer->jour_info.za+
M_info.vel_info.za_vel*dt;

M_info.pos_info.roll=M_info_pointer->pos_info.roll+
M_info.vel_info.xa_vel*dt;
M_info.pos_info.pitch=M_info_pointer->pos_info.pitch+
M_info.vel_info.ya_vel*dt;
M_info.pos_info.yaw=M_info_pointer->pos_info.yaw+
M_info.vel_info.za_vel*dt;

M_info.gra_cpt.gra_x=gra_x;
M_info.gra_cpt.gra_y=gra_y;
M_info.gra_cpt.gra_z=gra_z;

M_info.jour_info.xl=kalman_filter(M_info_pointer->jour_info.xl,
M_info_pointer->accel_info.xl_accel,
M_info.accel_info.xl_accel,
dt,&pxl_conv,Q_offset,R_offset);

M_info.jour_info.yl=kalman_filter(M_info_pointer->jour_info.yl,
M_info_pointer->accel_info.yl_accel,
M_info.accel_info.yl_accel,
dt,&pyl_conv,Q_offset,R_offset);

M_info.jour_info.zl=kalman_filter(M_info_pointer->jour_info.zl,
M_info_pointer->accel_info.zl_accel,
M_info.accel_info.zl_accel,
dt,&pzl_conv,Q_offset,R_offset);
		}
	mlist_add(M_info);
//		get_move_info();
if(p->count==MAX_NODE_NUM)
    {
rebuild_mlist();
mlist_add(M_info);
//printf("another 200 node\n");
    }
	free(M_info_pointer);
	shmdt(p);
	shmdt(tail);	}
nothing:
	free(M_info_pointer);
	shmdt(tail);
	shmdt(p);
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
///////////////////////////////////////////////

////////////////////////////////////////////////
