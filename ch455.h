#ifndef __CH455_H
#define __CH455_H
#include "sys.h"
#include "delay.h"


#define		DELAY			{ delay_us(4); } 

/*定义引脚*/
#define CH455_I2C_SCL_GPIO_PORT				GPIOA			/* GPIO端口 */
#define CH455_I2C_SCL_GPIO_CLK			 	RCC_AHB1Periph_GPIOA		/* GPIO端口时钟 */
#define CH455_I2C_SCL_PIN							GPIO_Pin_8			/* 连接到SCL时钟线的GPIO */

#define CH455_I2C_SDA_GPIO_PORT				GPIOC			/* GPIO端口 */
#define CH455_I2C_SDA_GPIO_CLK			 	RCC_AHB1Periph_GPIOC		/* GPIO端口时钟 */
#define CH455_I2C_SDA_PIN							GPIO_Pin_9			/* 连接到SDA数据线的GPIO */

/* 2线接口的连接,与实际电路有关 */
#define   CH455_SCL        PAout(8) //SCL
#define   CH455_SDA        PCout(9) //SDA

/* 2线接口的位操作,与单片机有关 */
#define		CH455_SCL_SET				{CH455_SCL=1;}
#define		CH455_SCL_CLR				{CH455_SCL=0;}
#define		CH455_SCL_D_OUT	 		{GPIOA->MODER&=~(3<<(8*2));GPIOB->MODER|=1<<8*2;} 
#define		CH455_SDA_SET				{CH455_SDA=1;}
#define		CH455_SDA_CLR				{CH455_SDA=0;}
#define		CH455_SDA_IN				PCin(9)     //  读取SDA输入电平
#define		CH455_SDA_D_OUT	  	{GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=1<<9*2;} 
#define		CH455_SDA_D_IN			{GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=0<<9*2;}	

///* 与单片机有关,与中断连接方式有关 */
//#define		USE_CH455_KEY		1			// 使用了CH455的按键中断

//#ifdef USE_CH455_KEY
//	#define		DISABLE_KEY_INTERRUPT	{EX1=0;}
//	#define		ENABLE_KEY_INTERRUPT	{EX1=1;}
//	#define		CLEAR_KEY_INTER_FLAG	{IE1=0;}
//#else
//	#define		DISABLE_KEY_INTERRUPT	{}
//	#define		ENABLE_KEY_INTERRUPT	{}
//	#define		CLEAR_KEY_INTER_FLAG	{}
//#endif

/* ********************************************************************************************* */
// 设置系统参数命令

#define CH455_BIT_ENABLE	0x01		// 开启/关闭位
#define CH455_BIT_SLEEP		0x04		// 睡眠控制位
#define CH455_BIT_7SEG		0x08		// 7段控制位
#define CH455_BIT_INTENS1	0x10		// 1级亮度
#define CH455_BIT_INTENS2	0x20		// 2级亮度
#define CH455_BIT_INTENS3	0x30		// 3级亮度
#define CH455_BIT_INTENS4	0x40		// 4级亮度
#define CH455_BIT_INTENS5	0x50		// 5级亮度
#define CH455_BIT_INTENS6	0x60		// 6级亮度
#define CH455_BIT_INTENS7	0x70		// 7级亮度
#define CH455_BIT_INTENS8	0x00		// 8级亮度

#define CH455_SYSOFF	0x0400			// 关闭显示、关闭键盘
#define CH455_SYSON		( CH455_SYSOFF | CH455_BIT_ENABLE )	// 开启显示、键盘
#define CH455_SLEEPOFF	CH455_SYSOFF	// 关闭睡眠
#define CH455_SLEEPON	( CH455_SYSOFF | CH455_BIT_SLEEP )	// 开启睡眠
#define CH455_7SEG_ON	( CH455_SYSON | CH455_BIT_7SEG )	// 开启七段模式
#define CH455_8SEG_ON	( CH455_SYSON | 0x00 )	// 开启八段模式
#define CH455_SYSON_4	( CH455_SYSON | CH455_BIT_INTENS4 )	// 开启显示、键盘、4级亮度
#define CH455_SYSON_8	( CH455_SYSON | CH455_BIT_INTENS8 )	// 开启显示、键盘、8级亮度


// 加载字数据命令
#define CH455_DIG0		0x1400			// 数码管位0显示,需另加8位数据
#define CH455_DIG1		0x1500			// 数码管位1显示,需另加8位数据
#define CH455_DIG2		0x1600			// 数码管位2显示,需另加8位数据
#define CH455_DIG3		0x1700			// 数码管位3显示,需另加8位数据



// 读取按键代码命令
#define CH455_GET_KEY	0x0700					// 获取按键,返回按键代码


// CH455接口定义
#define		CH455_I2C_ADDR		0x40			// CH455的地址
#define		CH455_I2C_MASK		0x3E			// CH455的高字节命令掩码

// 对外子程序

 
// 对外子程序
void CH455_Init(void);           				//初始化函数
unsigned char CH455_Read( void );				// 从CH455读取按键代码
void CH455_Write( unsigned short cmd );	// 向CH455发出操作命令
unsigned char  CH455_I2c_RdByte( void );
void CH455_I2c_WrByte(unsigned char IIC_Byte);
void CH455_I2c_Stop(void);
void CH455_I2c_Start(void);

void Read_keyboard(void);

#endif	/* CH455_H */
