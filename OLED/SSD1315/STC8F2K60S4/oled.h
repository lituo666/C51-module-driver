#ifndef __OLED_H_
#define __OLED_H_	

#include "STC8F.H"

sbit OLED_SCK=P4^5;     // SCK
sbit OLED_SDA=P4^6;     // SDA

#define OLED_SCK_Clr() OLED_SCK=0
#define OLED_SCK_Set() OLED_SCK=1

#define OLED_SDA_Clr() OLED_SDA=0
#define OLED_SDA_Set() OLED_SDA=1

//向OLED写命令开始
void OLED_WR_CMD_Sart(void);
//向OLED写数据
void OLED_WR_DATA_Sart(void);
// 向OLED发送一个字节
void OLED_WR_OneByte(unsigned char c);
// OLED发送结束信号
void OLED_WR_Stop(void);
//初始化oled屏幕
void OLED_Init(void);
//清屏,a=0x00 全黑，a=0xff 全亮
void OLED_Full(unsigned char a);

//清除某行，全黑
void OLED_FullCow(unsigned char n);
//开启OLED显示    
void OLED_Display_On(void);
//关闭OLED显示     
void OLED_Display_Off(void);
//设置光标
void OLED_Set_Pos(unsigned char x, unsigned char y);


//在指定位置显示一个英文字符
//x:0~127
//y:0~7
//mode:0,反白显示;1,正常显示
//size:选择字体(字宽)，8、6
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size);

//显示数字，需要指定位数。
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小,字宽 8、6
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295); 		  
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned int len,unsigned char size2);

//显示一个字符号串
void OLED_ShowString(unsigned char x,unsigned char y,char *chr,unsigned char Char_Size);

//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned int no);

#endif
