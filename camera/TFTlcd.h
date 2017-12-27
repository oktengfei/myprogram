#ifndef __TFT_LCD__H_
#define __TFT_LCD__H_


#define U16 unsigned short
#define U32 unsigned int 
#define U8 unsigned char



//LCD参数
typedef struct  
{										    
	U16 width;			//LCD 宽
	U16 height;			//LCD 高
	U16 id;				//LCD ID 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	


extern unsigned long POINT_COLOR; //画笔颜色
extern unsigned long BACK_COLOR; //背景色 

//画笔颜色
#define WHITE         	 0xFFFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F	//yanghong
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



void TFTLCD_Init(void);
void TFTLCD_Close(void);
void LCD_Clear(unsigned long color);
extern void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);
extern unsigned int LCD_GetPoint(unsigned int x,unsigned int y);
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char size,unsigned char mode);
void LCD_ShowString(unsigned short x,unsigned short y,unsigned short width,unsigned short height,unsigned short size,char *p);
void LCD_ShowStringsp(unsigned short x,unsigned short y,unsigned short width,unsigned short height,unsigned short size,char mode,char *p);		
void LCD_ShowNum(unsigned short x,unsigned short y,unsigned int num,unsigned char len,unsigned char size);
void LCD_ShowNumX(unsigned short x,unsigned short y,unsigned int num,unsigned char len,unsigned char size);
void LCD_ShowFloat(unsigned int x,unsigned int y,double num,unsigned int len,unsigned int remain,unsigned int size);
void LCD_Displayhline(unsigned short x0, unsigned short x1, unsigned short y);
void LCD_Displayrline(unsigned short x, unsigned short y0, unsigned short y1);
void LCD_DrawLine(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void LCD_Fill(unsigned short sx,unsigned short sy,unsigned short ex,unsigned short ey,unsigned long color);
void Draw_Circle(unsigned short x0,unsigned short y0,unsigned short r);




#endif
