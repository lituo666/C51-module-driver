#include "STC8F.H"
#include "intrins.h"
#include "oled.h"

void Delay2000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	i = 244;
	j = 130;
	k = 4;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
    OLED_Init();
    OLED_ShowString(0,0,"abcdefgfsdghadfa",6);
    OLED_FullCow(1);
    
    OLED_ShowNum(0,2,1234,4,8);
    
    OLED_ShowCHinese(0*16,6,0);
    OLED_ShowCHinese(1*16,6,1);
    OLED_ShowCHinese(2*16,6,2);
    OLED_ShowCHinese(3*16,6,3);
    OLED_ShowCHinese(4*16,6,4);
    OLED_ShowCHinese(5*16,6,5);
    OLED_ShowCHinese(6*16,6,6);
    OLED_ShowCHinese(7*16,6,6);
    
    Delay2000ms();
    OLED_Display_Off();
    Delay2000ms();
    OLED_Display_On();
    
    while(1){};
}
