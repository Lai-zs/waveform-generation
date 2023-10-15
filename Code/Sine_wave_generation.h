/*************************************** Copyright ******************************   
  * FileName   : Sine_wave_generation.h   
  * Version    : v1.0		
  * Author     : Lai-zs@qq.com		
  * Date       : 2022-12-22         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************/

#ifndef __SINE_WAVE_GENERATION_H_
#define __SINE_WAVE_GENERATION_H_

/*include----------------------------------------------------------------------*/
#include "main.h"

/*define-----------------------------------------------------------------------*/

typedef struct Wave_CH
{
	uint16_t ID;
	uint32_t phase_difference;
	struct Wave_CH* Next;	
}Wave_Channel_struct;


typedef struct
{
	uint32_t period;	//周期 us
	float freq;	//频率 Hz
	uint16_t Vpp_max;	//最大值
	uint16_t Vpp_min;	//最小值
	uint16_t* data_buf;	//缓冲区
	uint16_t data_buf_size;	//缓冲区大小
	uint32_t (*Get_time_us)(void);
	Wave_Channel_struct* Wave_Channel;
}Sine_Wave_struct;

/*variate statement------------------------------------------------------------*/

void set_wave_freq(Sine_Wave_struct* wave,float freq);		//设置波形频率								//
void set_wave_period(Sine_Wave_struct* wave,uint32_t period);                           //
void set_wave_VP(Sine_Wave_struct* wave,uint16_t Vpp_max,uint16_t Vpp_min);             //设置波形峰值 
void init_wave_buff(Sine_Wave_struct* wave,uint16_t size);                              //初始化缓冲区
void init_wave_buff_data(Sine_Wave_struct* wave);                                       //初始化数据
void wave_Channel_Add(Sine_Wave_struct* wave,uint16_t ID,uint32_t phase_difference);    //加一个输出通道
uint32_t Get_Channel_phase(Sine_Wave_struct* wave,uint16_t ID);                         //获取通道相位差
void Print_Channel(Sine_Wave_struct* wave);                                             //打印通道数据
void Printf_wave_buff_data(Sine_Wave_struct* wave);                                     //打印缓冲区的数据
uint16_t Get_wave_Value(Sine_Wave_struct* wave,uint16_t ID);                            //获取波形的值
uint32_t Calculate_wave_phase(Sine_Wave_struct* wave,float percent_phase);              //计算相位差
/*function statement-----------------------------------------------------------*/



#endif


 

