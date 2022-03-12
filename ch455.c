#include "ch455.h"

unsigned char id��
unsigned char key_id = 0;
 
void CH455_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(CH455_I2C_SCL_GPIO_CLK | CH455_I2C_SDA_GPIO_CLK, ENABLE);	/* ��GPIOʱ�� */
	//SCL
	GPIO_InitStructure.GPIO_Pin = CH455_I2C_SCL_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  	/* ��©��� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(CH455_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	//SDA
	GPIO_InitStructure.GPIO_Pin = CH455_I2C_SCL_PIN ;
	GPIO_Init(CH455_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	CH455_Write( CH455_SYSON_8 );		//�˶�ʽ
}

void CH455_I2c_Start( void )  // ������ʼ
{

	CH455_SDA_D_OUT;   /* ����SDAΪ������� */
	CH455_SCL_SET;
	DELAY;
	CH455_SDA_SET;
	DELAY;
	CH455_SDA_CLR;
	DELAY;
	CH455_SCL_CLR;
}

void CH455_I2c_Stop( void )  // ��������
{
	
	CH455_SDA_D_OUT;   /* ����SDAΪ������� */
	CH455_SCL_CLR;
	DELAY;
	CH455_SDA_CLR;
	DELAY;
	CH455_SCL_SET;
	DELAY;
	CH455_SDA_SET;  /*����I2C���߽����ź�*/
	DELAY;

//	ENABLE_KEY_INTERRUPT;
}

void CH455_I2c_WrByte( unsigned char dat )	//дһ���ֽ�����
{
	unsigned char i;
	CH455_SDA_D_OUT;   /* ����SDAΪ������� */
	for( i = 0; i != 8; i++ )  // ���8λ����
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
	CH455_SCL_SET;  // ����Ӧ��
	DELAY;
	CH455_SCL_CLR;
	DELAY;
}

unsigned char  CH455_I2c_RdByte( void )		//��һ���ֽ�����
{
	unsigned char dat,i;
	CH455_SDA_D_IN;
	DELAY;
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_CLR;
	DELAY;
	dat = 0;
	for( i = 0; i != 8; i++ )  // ����8λ����
	{
		CH455_SCL_SET;
		DELAY;  // ��ѡ��ʱ
		dat <<= 1;
		DELAY;  // ��ѡ��ʱ
		dat |= (CH455_SDA_IN);
		DELAY;
		CH455_SCL_CLR;
		DELAY;
	}
	CH455_SDA_D_OUT;
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_SET;  // ������ЧӦ��
	DELAY;
	CH455_SCL_CLR;
	DELAY;
	return dat;
}

 void CH455_Write(unsigned short cmd )	//д����
{
	CH455_I2c_Start();               //��������
	CH455_I2c_WrByte(((unsigned char)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
	CH455_I2c_WrByte((unsigned char)cmd);               //��������
	CH455_I2c_Stop();                 //�������� 
}

unsigned char CH455_Read( void )		//��ȡ����
{
	unsigned char keycode;
	CH455_I2c_Start();                //��������
	CH455_I2c_WrByte((unsigned char) ((CH455_GET_KEY>>7)&CH455_I2C_MASK)|0x01|CH455_I2C_ADDR);//�������Ƕ�ȡ������������:4FH
	keycode=CH455_I2c_RdByte();      //��ȡ����
	CH455_I2c_Stop();                //��������
	return keycode;
}



void Read_keyboard(void)
{	
    unsigned char keyID; 
	
    keyID = CH455_Read();
	
    if(0xFF == keyID)
    {
        /* �ް������� */
        return;
    }
    else if(keyID & 0x40)     
    {
        /* �а������£���¼�°�����ֵ */
        keyID &= ~0x40;        
        key_id = keyID;
//			printf ("����ֵ��һ�δ�����%x\r\n",key_id);
    }
//    else                      
//    {
        /* �����ɿ��������ֵ */
        if(keyID == key_id)
        {
 
             /* ������ */
						printf ("����ֵ�ڶ��δ�����%x\r\n",key_id);
            key_id = 0xFF;
        }
        else
        {
            key_id = 0xFF;
        }
//    }    
    return; 
}


