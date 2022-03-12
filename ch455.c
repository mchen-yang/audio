#include "ch455.h"

unsigned char id；
unsigned char key_id = 0;
 
void CH455_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(CH455_I2C_SCL_GPIO_CLK | CH455_I2C_SDA_GPIO_CLK, ENABLE);	/* 打开GPIO时钟 */
	//SCL
	GPIO_InitStructure.GPIO_Pin = CH455_I2C_SCL_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  	/* 开漏输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(CH455_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	//SDA
	GPIO_InitStructure.GPIO_Pin = CH455_I2C_SCL_PIN ;
	GPIO_Init(CH455_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	CH455_Write( CH455_SYSON_8 );		//八段式
}

void CH455_I2c_Start( void )  // 操作起始
{

	CH455_SDA_D_OUT;   /* 设置SDA为输出方向 */
	CH455_SCL_SET;
	DELAY;
	CH455_SDA_SET;
	DELAY;
	CH455_SDA_CLR;
	DELAY;
	CH455_SCL_CLR;
}

void CH455_I2c_Stop( void )  // 操作结束
{
	
	CH455_SDA_D_OUT;   /* 设置SDA为输出方向 */
	CH455_SCL_CLR;
	DELAY;
	CH455_SDA_CLR;
	DELAY;
	CH455_SCL_SET;
	DELAY;
	CH455_SDA_SET;  /*发送I2C总线结束信号*/
	DELAY;

//	ENABLE_KEY_INTERRUPT;
}

void CH455_I2c_WrByte( unsigned char dat )	//写一个字节数据
{
	unsigned char i;
	CH455_SDA_D_OUT;   /* 设置SDA为输出方向 */
	for( i = 0; i != 8; i++ )  // 输出8位数据
	{
		if( dat & 0x80 ) 
		{
		    CH455_SDA_SET;
		}
		else 
		{
		    CH455_SDA_CLR;
		}
		DELAY;
		CH455_SCL_SET;
		DELAY;
		CH455_SCL_CLR;
		DELAY;
		dat <<= 1;
		
	}
	DELAY;
	
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_SET;  // 接收应答
	DELAY;
	CH455_SCL_CLR;
	DELAY;
}

unsigned char  CH455_I2c_RdByte( void )		//读一个字节数据
{
	unsigned char dat,i;
	CH455_SDA_D_IN;
	DELAY;
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_CLR;
	DELAY;
	dat = 0;
	for( i = 0; i != 8; i++ )  // 输入8位数据
	{
		CH455_SCL_SET;
		DELAY;  // 可选延时
		dat <<= 1;
		DELAY;  // 可选延时
		dat |= (CH455_SDA_IN);
		DELAY;
		CH455_SCL_CLR;
		DELAY;
	}
	CH455_SDA_D_OUT;
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_SET;  // 发出无效应答
	DELAY;
	CH455_SCL_CLR;
	DELAY;
	return dat;
}

 void CH455_Write(unsigned short cmd )	//写命令
{
	CH455_I2c_Start();               //启动总线
	CH455_I2c_WrByte(((unsigned char)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
	CH455_I2c_WrByte((unsigned char)cmd);               //发送数据
	CH455_I2c_Stop();                 //结束总线 
}

unsigned char CH455_Read( void )		//读取按键
{
	unsigned char keycode;
	CH455_I2c_Start();                //启动总线
	CH455_I2c_WrByte((unsigned char) ((CH455_GET_KEY>>7)&CH455_I2C_MASK)|0x01|CH455_I2C_ADDR);//计算结果是读取按键代码命令:4FH
	keycode=CH455_I2c_RdByte();      //读取数据
	CH455_I2c_Stop();                //结束总线
	return keycode;
}



void Read_keyboard(void)
{	
    unsigned char keyID; 
	
    keyID = CH455_Read();
	
    if(0xFF == keyID)
    {
        /* 无按键按下 */
        return;
    }
    else if(keyID & 0x40)     
    {
        /* 有按键按下，记录下按键键值 */
        keyID &= ~0x40;        
        key_id = keyID;
//			printf ("按键值第一次处理是%x\r\n",key_id);
    }
//    else                      
//    {
        /* 按键松开，处理键值 */
        if(keyID == key_id)
        {
 
             /* 处理函数 */
						printf ("按键值第二次处理是%x\r\n",key_id);
            key_id = 0xFF;
        }
        else
        {
            key_id = 0xFF;
        }
//    }    
    return; 
}


