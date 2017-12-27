#include "framebuffer.h"
#include "TFTlcd.h"


int main(int argc,char **argv)
{
    TFTLCD_Init();
    BACK_COLOR = GREEN;
    POINT_COLOR = BLUE;

    LCD_Clear(RED);
	LCD_Displayhline(0,1376,0);
	LCD_Displayrline(50,10,400);
    LCD_ShowString(20,20,400,16,16,"You are big bad egg !!");
    LCD_Fill(20,40,100,130,YELLOW);
    Draw_Circle(100,120,43);
    TFTLCD_Close();

}
