#ifndef __TFT_LCD__H_
#define __TFT_LCD__H_


#define U16 unsigned short
#define U32 unsigned int 
#define U8 unsigned char



//LCD����
typedef struct  
{										    
	U16 width;			//LCD ��
	U16 height;			//LCD ��
	U16 id;				//LCD ID 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	


extern unsigned long POINT_COLOR; //������ɫ
extern unsigned long BACK_COLOR; //����ɫ 

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)



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
