# 多路信号生成器

## 介绍

多路信号生成器，具备生成多路具有不同相位差的信号的能力。用户可以自由调整代码中的参数，以控制信号的频率、峰值和波形。

## 使用方法

### 1.文件结构：

**Sin_w_gen_Config**：中间层代码，负责波形配置，应用层应调用该文件函数。

**Sine_wave_generation**：底层代码，负责波形生成的具体实现。

### 2.使用：

（0）配置定时器

-配置合适的定时器用来获得时基，SIN_Init()中使用htim2作为基准定时器

```c
HAL_TIM_Base_Start(&htim2);
```

-修改Get_Time_us()函数

```c
/***********************************************************
*@fuction	:Get_Time_us
*@brief		:时间获取函数
***********************************************************/
uint32_t Get_Time_us()	//返回计数器时间 单位us
{
	return __HAL_TIM_GET_COUNTER(&htim2);	
}
```

（1）调用函数

```c
SIN_Init();
```

（2）开启PWM输出

```c
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
```

（3）获取当前通道PWM占空比并配置

```c
uint16_t V1 = Get_CH1_Val();
uint16_t V2 = Get_CH2_Val();
uint16_t V3 = Get_CH3_Val();
uint16_t V4 = Get_CH4_Val();
__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,V1);
__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,V2);
__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,V3);
__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,V4);
```

### 3.如何设置波形：

新建波形：

```c
Sine_Wave_struct sin_W = {0};
```

频率幅值设置：

```c
set_wave_freq(&sin_W,5);			//设置频率
set_wave_VP(&sin_W,2500,500);	//设置峰值 谷值
```

波形设置：

```c
init_wave_buff(&sin_W,400);		//申请缓冲区,缓冲区越大波形越精细
init_wave_buff_data(&sin_W);	//初始化缓冲区数据
//默认sine，可以通过重写init_wave_buff_data函数设置不同波形形状
```

增加一个通道：

```c
wave_Channel_Add(&sin_W,1,Calculate_wave_phase(&sin_W,10));//添加通道，ID:1,相位差10°
//需要使用Calculate_wave_phase函数计算相位差，单位为角度。
```

## 代码原理

根据Get_Time_us()函数从定时器获取到时间，然后根据波形频率从缓冲区读取波形数据。

通道数据使用链表进行存储，每加入一个通道就在链尾加入一个数据。

本代码针对sin波形进行优化，只储存了1/4个周期的波形数据，再根据所在象限判断符号，以实现更高的空间利用率和精度。

如果需要更改其他波形，需要重写 Get_wave_Value() 和 init_wave_buff_data() 函数。





















