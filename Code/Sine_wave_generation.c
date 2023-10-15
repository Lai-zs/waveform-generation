/*************************************** Copyright ****************************** 
  * FileName   : Sine_wave_generation.c   
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
********************************************************************************/
 
/*include----------------------------------------------------------------------*/
#include "Sine_wave_generation.h"
#include <stdlib.h>
#include <math.h>

/*define-----------------------------------------------------------------------*/
#define Pi 3.1415926f
#define Pi2 6.2831853f

/*variate----------------------------------------------------------------------*/


/*statement--------------------------------------------------------------------*/


/*Function prototype Begin*****************************************************/
/***********************************************************
*@fuction	:set_wave_freq
*@brief		:设置频率
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void set_wave_freq(Sine_Wave_struct* wave,float freq)
{
	wave->freq = freq;
	wave->period = 1000000 / freq; //计算周期
}

/***********************************************************
*@fuction	:set_wave_period
*@brief		:设置周期
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void set_wave_period(Sine_Wave_struct* wave,uint32_t period)
{
	wave->freq = 1000000 / period;
	wave->period = period; //20ms
}

/***********************************************************
*@fuction	:set_wave_VP
*@brief		:设置波形峰值 
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void set_wave_VP(Sine_Wave_struct* wave,uint16_t Vpp_max,uint16_t Vpp_min)
{
	wave->Vpp_max =Vpp_max;
	wave->Vpp_min =Vpp_min;
}


/***********************************************************
*@fuction	:init_wave_buff
*@brief		:初始化缓冲区
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void init_wave_buff(Sine_Wave_struct* wave,uint16_t size)
{
	if(wave->data_buf == NULL)
	{
		wave->data_buf_size =size;
		wave->data_buf = (uint16_t*)malloc(sizeof(uint16_t)*wave->data_buf_size);
		if(wave->data_buf == NULL)
		{
			printf_uart1("init_wave_buff ERR\n");
			while(1);
		}
	}
}

/***********************************************************
*@fuction	:init_wave_buff_data
*@brief		:初始化数据
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void init_wave_buff_data(Sine_Wave_struct* wave)
{
	if(wave->data_buf != NULL)
	{
		if(wave->freq!=0 && wave->period!=0 )
		{
			uint16_t VP = (wave->Vpp_max-wave->Vpp_min)/2;
			uint16_t offset = wave->Vpp_min + VP;
			float unit = Pi2/4/(wave->data_buf_size);		//只需要1/4个周期数据
			for(uint16_t i=0;i<wave->data_buf_size;i++)
			{
				wave->data_buf[i] = (uint16_t) (sinf(unit*i)*VP + offset);
			}
		}
	}
}

/***********************************************************
*@fuction	:wave_Channel_Add
*@brief		:加一个输出通道
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void wave_Channel_Add(Sine_Wave_struct* wave,uint16_t ID,uint32_t phase_difference)
{
	if(wave->Wave_Channel == NULL)
	{
		wave->Wave_Channel = (Wave_Channel_struct*)malloc(sizeof(Wave_Channel_struct));
		wave->Wave_Channel->ID = ID;
		wave->Wave_Channel->phase_difference = phase_difference;
		wave->Wave_Channel->Next = NULL;
	}
	else
	{
		Wave_Channel_struct *L = NULL;
		Wave_Channel_struct *p = NULL;
		L = wave->Wave_Channel;
		while (L->Next != NULL)
		{
			L = L->Next;//遍历找到尾节点 
		}
		p = (Wave_Channel_struct *) malloc (sizeof(Wave_Channel_struct));//开辟新的节点 
		if(p == NULL)
		{
			printf_uart1("wave_Channel_Add ERR\n");
			while(1);
		}
		p->ID = ID;//赋值 
		p->phase_difference = phase_difference;
		p->Next = NULL;//设置为尾节点 
		L->Next = p;//挂在原来尾节点之后 
	}
}

/***********************************************************
*@fuction	:Get_Channel_phase
*@brief		:获取通道相位差
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
uint32_t Get_Channel_phase(Sine_Wave_struct* wave,uint16_t ID)
{
	Wave_Channel_struct *p = wave->Wave_Channel;
	while(p != NULL)//遍历打印节点值 
	{
		if(p->ID == ID) break;
		p = p->Next;
	}
	if(p != NULL)
	{
		return p->phase_difference;
	}
	return 0xffffffff;
}

/***********************************************************
*@fuction	:Print_Channel
*@brief		:打印通道数据
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void Print_Channel(Sine_Wave_struct* wave)
{
	Wave_Channel_struct *p = wave->Wave_Channel;
	while(p != NULL)//遍历打印节点值 
	{
		printf_uart1("ID:%d phase:%d\n", p->ID,p->phase_difference);
		p = p->Next;
	}
}


/***********************************************************
*@fuction	:Printf_wave_buff_data
*@brief		:打印缓冲区的数据
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
void Printf_wave_buff_data(Sine_Wave_struct* wave)
{
	if(wave->data_buf != NULL)
	{
		for(uint16_t i=0;i<wave->data_buf_size;i++)
		{
			printf_uart1("Data:%d\n",wave->data_buf[i]);
		}
	}
}


/***********************************************************
*@fuction	:Get_wave_Value
*@brief		:获取波形的值
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
uint16_t Get_wave_Value(Sine_Wave_struct* wave,uint16_t ID)
{
	if(wave->data_buf != NULL)
	{
		Wave_Channel_struct *p = wave->Wave_Channel;
		while(p != NULL)//遍历打印节点值 
		{
			if(p->ID == ID) break;
			p = p->Next;
		}
		if(p != NULL)
		{
			uint32_t T = wave->Get_time_us()+p->phase_difference;
			T = T % wave->period;
			uint32_t i = (uint32_t)(((float)T/wave->period) * wave->data_buf_size*4);
			uint16_t x = i/wave->data_buf_size;
			uint16_t Val = 0;
			switch(x)
			{
				case 0: Val	= wave->data_buf[i];			break;		//0-90°
				case 1: i = 2*wave->data_buf_size - i; 
								Val = wave->data_buf[i];				break;	//90-180°
				case 2: i = i - 2*wave->data_buf_size;
								Val = (wave->Vpp_min + wave->Vpp_max) - wave->data_buf[i]; 	break;	//180-270°
				case 3: i = 4*wave->data_buf_size - i;
								Val = (wave->Vpp_max + wave->Vpp_min) - wave->data_buf[i];				break;	//270-360°
				default :printf_uart1("i = %d\n",i); break ;
			}
			 
			return Val;
		}	
	}
	return 0;
}

/***********************************************************
*@fuction	:Calculate_wave_phase
*@brief		:计算相位差,0~180
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-23
***********************************************************/
uint32_t Calculate_wave_phase(Sine_Wave_struct* wave,float percent_phase)
{
	uint32_t phase;
	percent_phase = percent_phase/180;
	if(percent_phase>0)
	{
		phase = (uint32_t)((float)wave->period * percent_phase);
	}
	else
	{
		phase = (uint32_t)(wave->period + (float)wave->period * percent_phase);
	}
	return phase;
}


/*Function prototype End*******************************************************/


