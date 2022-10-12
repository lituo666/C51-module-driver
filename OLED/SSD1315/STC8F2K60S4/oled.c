#include "oled.h"
#include "oledfont.h"
#include "intrins.h"

void IIC_Delay1us(void);
void IIC_Start(void);
void IIC_SendData(unsigned char c);
void IIC_Stop(void);

void IIC_Delay1us(void)		//@24.000MHz
{
    unsigned char i;
	i = 6;
	while (--i);
}

void IIC_Start(void)
{
    OLED_SDA_Set();
    OLED_SCK_Set();
    OLED_SDA_Clr();
    IIC_Delay1us();
}

void IIC_SendData(unsigned char c)
{
    unsigned char i;
    OLED_SCK_Clr();
    for(i=0;i<8;i++){
        if(c&0x80) 
            OLED_SDA_Set();
        else
            OLED_SDA_Clr();
        IIC_Delay1us();
        OLED_SCK_Set();
        IIC_Delay1us();
        OLED_SCK_Clr();
        c <<= 1;
    }
    
    OLED_SDA_Set();
    OLED_SCK_Set();     // 接收应答位，但是不做处理。
}

void IIC_Stop(void)
{
    OLED_SCK_Clr();
    IIC_Delay1us();
    OLED_SDA_Clr();
    IIC_Delay1us();
    OLED_SCK_Set();
    IIC_Delay1us();
    OLED_SDA_Set();
    IIC_Delay1us();
}

// 向OLED发送一个字节
void OLED_WR_OneByte(unsigned char c)
{
    IIC_SendData(c);
}

// OLED发送结束信号
void OLED_WR_Stop(void)
{
    IIC_Stop();
}

//向OLED写命令开始
void OLED_WR_CMD_Sart(void)
{
    IIC_Start();
    OLED_WR_OneByte(0x78);     // 从机地址 OLED模块后面应该有写
    OLED_WR_OneByte(0x00);
}
//向OLED写数据
void OLED_WR_DATA_Sart(void)
{
    IIC_Start();
    OLED_WR_OneByte(0x78);     // 从机地址 OLED模块后面应该有写
    OLED_WR_OneByte(0x40);     // 0100 0000
}

//初始化oled屏幕
void OLED_Init(void)
{
    P4PU = 0x60;    /* STC8F 内部有4.1K的上拉电阻，此处使能P4.5和P4.6 */

    OLED_WR_CMD_Sart();
    OLED_WR_OneByte(0xAE); //display off
	OLED_WR_OneByte(0x20); //Set Memory Addressing Mode
	OLED_WR_OneByte(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WR_OneByte(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	OLED_WR_OneByte(0xc8); //Set COM Output Scan Direction
	OLED_WR_OneByte(0x00); //---set low column address
	OLED_WR_OneByte(0x10); //---set high column address
	OLED_WR_OneByte(0x40); //--set start line address
	OLED_WR_OneByte(0x81); //--set contrast control register
	OLED_WR_OneByte(0xff); //亮度调节 0x00~0xff
	OLED_WR_OneByte(0xa1); //--set segment re-map 0 to 127
	OLED_WR_OneByte(0xa6); //--set normal display
	OLED_WR_OneByte(0xa8); //--set multiplex ratio(1 to 64)
	OLED_WR_OneByte(0x3F); //
	OLED_WR_OneByte(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WR_OneByte(0xd3); //-set display offset
	OLED_WR_OneByte(0x00); //-not offset
	OLED_WR_OneByte(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_WR_OneByte(0xf0); //--set divide ratio
	OLED_WR_OneByte(0xd9); //--set pre-charge period
	OLED_WR_OneByte(0x22); //
	OLED_WR_OneByte(0xda); //--set com pins hardware configuration
	OLED_WR_OneByte(0x12);
	OLED_WR_OneByte(0xdb); //--set vcomh
	OLED_WR_OneByte(0x20); //0x20,0.77xVcc
	OLED_WR_OneByte(0x8d); //--set DC-DC enable
	OLED_WR_OneByte(0x14); //
	OLED_WR_OneByte(0xaf); //--turn on oled panel
    OLED_WR_Stop();

    OLED_Full(0x00); //清屏全黑
    
}
//清屏,a=0x00 全黑，a=0xff 全亮
void OLED_Full(unsigned char a)
{
	unsigned char i,n;
	for(i=0;i<8;i++)  
	{
        OLED_WR_CMD_Sart();
		OLED_WR_OneByte(0xb0+i);
		OLED_WR_OneByte (0x00);
		OLED_WR_OneByte (0x10);
        OLED_WR_Stop();
        
        OLED_WR_DATA_Sart();
		for(n=0;n<128;n++){
            OLED_WR_OneByte(a);
        }
        OLED_WR_Stop();
	}
}
//清除某行，全黑
void OLED_FullCow(unsigned char n)
{
	unsigned char i;
    OLED_WR_CMD_Sart();
	OLED_WR_OneByte(0xb0+n);
	OLED_WR_OneByte (0x00); 
	OLED_WR_OneByte (0x10);
    OLED_WR_Stop();
    
	OLED_WR_DATA_Sart();
    for(i=0;i<128;i++)
		OLED_WR_OneByte(0);
    OLED_WR_Stop();
}
//开启OLED显示    
void OLED_Display_On(void)
{
    OLED_WR_CMD_Sart();
	OLED_WR_OneByte(0X8D);  //SET DCDC命令
	OLED_WR_OneByte(0X14);  //DCDC ON
	OLED_WR_OneByte(0XAF);  //DISPLAY ON
    OLED_WR_Stop();
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
    OLED_WR_CMD_Sart();
	OLED_WR_OneByte(0X8D);  //SET DCDC命令
	OLED_WR_OneByte(0X10);  //DCDC OFF
	OLED_WR_OneByte(0XAE);  //DISPLAY OFF
    OLED_WR_Stop();
}
// 设置指针位置，x是列，y是行
void OLED_Set_Pos(unsigned char x, unsigned char y)
{ 	
    OLED_WR_CMD_Sart();
	OLED_WR_OneByte(0xb0+y);
	OLED_WR_OneByte(((x&0xf0)>>4)|0x10);
	OLED_WR_OneByte(x&0x0f);
    OLED_WR_Stop();
} 

unsigned int oled_pow(unsigned int m,unsigned int n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

//在指定位置显示一个英文字符
//x:0~127
//y:0~7
//mode:0,反白显示;1,正常显示
//size:选择字体(字宽)，8、6
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值
    if(y>7) y = 0;
	if(x>128-1){
        x = 0;
        if(y+1>7)
            y = 0;
        else
            y += 1;
    }
	if(Char_Size == 8)
	{
		OLED_Set_Pos(x,y);
        OLED_WR_DATA_Sart();
		for(i=0;i<8;i++)
            OLED_WR_OneByte(F8X16[c*16+i]);
        OLED_WR_Stop();
        OLED_Set_Pos(x,y+1);
        OLED_WR_DATA_Sart();
		for(i=0;i<8;i++)
            OLED_WR_OneByte(F8X16[c*16+i+8]);
        OLED_WR_Stop();
	}
	else{
		OLED_Set_Pos(x,y);
        OLED_WR_DATA_Sart();
		for(i=0;i<6;i++)
            OLED_WR_OneByte(F6X8[c][i]);
        OLED_WR_Stop();
	}
}

//显示数字，需要指定位数。
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小,字宽 8、6
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned int len,unsigned char size2)
{
    unsigned char enshow=0;  // 不足位数前面补零，0 不补，1 补零
	unsigned char t;
	unsigned int temp;						   
	for(t=0;t<len;t++) {
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1)) {
			if(temp==0) {
				OLED_ShowChar(x+size2*t,y,' ',size2);
				continue;
			}else enshow=1;
		}
	 	OLED_ShowChar(x+size2*t,y,temp+'0',size2); 
	}
}

//显示一个字符号串
void OLED_ShowString(unsigned char x,unsigned char y,char *chr,unsigned char Char_Size)
{
	unsigned int j=0;
    if(y>7) y = 0;
	if(x>128-1){
        x = 0;
        if(y+1>7)
            y = 0;
        else
            y += 1;
    }
	while (chr[j]!='\0') {
        OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120){
            x=0;
            if(Char_Size == 8)
                y+=2;
            else
                y+=1;
        }
		j++;
	}
}

//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned int no)
{      			    
	unsigned char t;
	OLED_Set_Pos(x,y);
    
    OLED_WR_DATA_Sart();
    for(t=0;t<16;t++)
		OLED_WR_OneByte(Hzk[2*no][t]);
    OLED_WR_Stop();
    
	OLED_Set_Pos(x,y+1);
    
    OLED_WR_DATA_Sart();
    for(t=0;t<16;t++)
		OLED_WR_OneByte(Hzk[2*no+1][t]);
    OLED_WR_Stop();
}
