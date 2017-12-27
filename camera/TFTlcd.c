#include "TFTlcd.h"
#include "linux/fb.h"
#include "framebuffer.h"
#include "font.h"
//#include "stdio.h"

unsigned long POINT_COLOR; //
unsigned long BACK_COLOR;  //


//LCD
_lcd_dev lcddev;	

void TFTLCD_Init(void)
{
	frame_init();
	lcddev.width = vinfo.xres;
	lcddev.height = vinfo.yres;
	lcddev.id = LCD_Type;
}
void TFTLCD_Close(void)
{
    frame_close();
}
//void LCD_Clear(unsigned short c)
//void LCD_DrawPoint(unsigned short x,unsigned short y,unsigned short color)

void LCD_Clear(unsigned long color)
{
	unsigned int x,y ;
		
    for( y = 0 ; y < lcddev.height ; y++ )
    {
    	for( x = 0 ; x < lcddev.width ; x++ )
    	{
			LCD_DrawPoint(x,y,color);
    	}
    }
}



//==========写8*16字符  x 横坐标 y 纵坐标 c 需要显示的字符 fColor 字符颜色 bColor 字符背景颜色==========================
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char size,unsigned char mode)
{  							  
    unsigned char temp,t1,t;
	unsigned short y0=y;
	unsigned short colortemp=POINT_COLOR;      			     
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12) temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y,POINT_COLOR);	
				temp<<=1;
				y++;
				if(x>=240){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=240){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }    
	}else//叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y,POINT_COLOR); 
				temp<<=1;
				y++;
				if(x>=320){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=320){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
void LCD_ShowString(unsigned short x,unsigned short y,unsigned short width,unsigned short height,unsigned short size,char *p)
{         
/*
	char *p;
	unsigned char x0=x;
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    p = string;
*/   	
    unsigned short x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
    //va_end(ap);
}
void LCD_ShowStringsp(unsigned short x,unsigned short y,unsigned short width,unsigned short height,unsigned short size,char mode,char *p)
{       
/*    
	char *p;
	unsigned char x0=x;
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    p = string;
*/
    unsigned short x0=x;
    width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//??
        LCD_ShowChar(x,y,*p,size,mode);
        x+=size/2;
        p++;
    }  
	//va_end(ap);
}
unsigned int LCD_Pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}	
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(unsigned short x,unsigned short y,unsigned int num,unsigned char len,unsigned char size)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

//显示十六进制,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNumX(unsigned short x,unsigned short y,unsigned int num,unsigned char len,unsigned char size)
{
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(16,len-t-1))%16;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,'0',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp>9?(temp-10+'A'):(temp+'0'),size,0); 
	}
} 

//显示浮点数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//remain:显示的小数位数
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowFloat(unsigned int x,unsigned int y,double num,unsigned int len,unsigned int remain,unsigned int size)
{
	int temp;
	unsigned int t,m,n=0;
	unsigned int enshow=0;
	temp = (int)(num * LCD_Pow(10,remain));
	if(temp<0) 
	{
		LCD_ShowChar(x,y,'-',size,0);
		temp = -temp;
	}
	else LCD_ShowChar(x,y,' ',size,0);
	for(t=1;t<len+1;t++)
	{
		m=(temp/LCD_Pow(10,len-t))%10;
		if(enshow==0&&t<(len-1))
		{
			if(m==0&&t<len - remain)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}
			else if(m==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,'0',size,0);
				continue;
			}
			else
				enshow=1;
		 	 
		}
		if(t == len - remain + 1)
		{
			LCD_ShowChar(x+(size/2)*t,y,'.',size,0);
			n = 8;
		}
	 	LCD_ShowChar(x+(size/2)*t+n,y,m+'0',size,0); 
	}
}
//=====   画水平线函数   =====
void LCD_Displayhline(unsigned short x0, unsigned short x1, unsigned short y) 	 //两个横坐标  一个纵坐标  以及颜色
{  
	unsigned char  bak;
	if(x0>x1)        // 对x0、x1大小进行排列，以便画图
	{  
		bak = x1;
		x1 = x0;
		x0 = bak;
	}
	do
	{  
		LCD_DrawPoint(x0,y,POINT_COLOR);  // 从左到右逐点显示，描出垂直线
		x0++;
	}while(x1>=x0);
}


//=====   画垂直线函数   =====
void LCD_Displayrline(unsigned short x, unsigned short y0, unsigned short y1) 	//一个横坐标，两个纵坐标，颜色
{  
	unsigned char  bak;
	if(y0>y1)        // 对y0、y1大小进行排列，以便画图
	{  
		bak = y1;
		y1 = y0;
		y0 = bak;
	}
	do
	{  
		LCD_DrawPoint(x,y0,POINT_COLOR);   // 从上到下逐点显示，描出垂直线
		y0++;
	}while(y1>=y0);
}

void LCD_DrawLine(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1)

{  
	int dx;       		// 直线x轴差值变量
	int dy;       		// 直线y轴差值变量
	signed char dx_sym;      	// x轴增长方向，为-1时减值方向，为1时增值方向
	signed char dy_sym;      	// y轴增长方向，为-1时减值方向，为1时增值方向
	int dx_x2;       	// dx*2值变量，用于加快运算速度
	int dy_x2;       	// dy*2值变量，用于加快运算速度
	int di;       		// 决策变量
	
	if(x0==x1)         	//判断是否为垂直线
	{
		LCD_Displayrline(x0,y0,y1);    //画垂直线
		return;
	}
	if(y0==y1)         	//判断是否为水平线
	{
		LCD_Displayhline(x0,x1,y0);    //画水平线
		return;
	}
	dx = x1-x0;         // 求取两点之间的差值
	dy = y1-y0;
	//判断增长方向，或是否为水平线、垂直线、点 
	if(dx>0)         	// 判断x轴方向
		dx_sym = 1;        // dx>0，设置dx_sym=1
	else
		dx_sym = -1;      	// dx<0，设置dx_sym=-1
	if(dy>0)         			// 判断y轴方向
		dy_sym = 1;        		// dy>0，设置dy_sym=1
	else
		dy_sym = -1;      	// dy<0，设置dy_sym=-1
							//将dx、dy取绝对值 
	dx = dx_sym * dx;
	dy = dy_sym * dy;
							// 计算2倍的dx及dy值 
	dx_x2 = dx<<1;
	dy_x2 = dy<<1;
							// 使用Bresenham法进行画直线 
	if(dx>=dy)         		// 对于dx>=dy，则使用x轴为基准	即直线的斜率小于1
	{  
		di = dy_x2 - dx;
		while(x0!=x1)
		{  
			LCD_DrawPoint(x0, y0,POINT_COLOR);
			x0 += dx_sym;
			if(di<0)
			{  
				di += dy_x2;     // 计算出下一步的决策值
			}
			else
			{  
				di += dy_x2 - dx_x2;
				y0 += dy_sym;
			}
		}
	  LCD_DrawPoint(x0, y0,POINT_COLOR);    // 显示最后一点
	}
	else          // 对于dx<dy，则使用y轴为基准
	{ 
		di = dx_x2 - dy;
		while(y0!=y1)
		{  
			LCD_DrawPoint(x0, y0,POINT_COLOR);
			y0 += dy_sym;
			if(di<0)
			{ 
				di += dx_x2;
			}
			else
			{  
				di += dx_x2 - dy_x2;
				x0 += dx_sym;
			}
		}
		LCD_DrawPoint(x0, y0,POINT_COLOR);    // 显示最后一点
	} 
}


void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
	LCD_Displayhline(x1,x2,y1);
	LCD_Displayhline(x1,x2,y2);
	LCD_Displayrline(x1,y1,y2); 	//画垂直线函数
	LCD_Displayrline(x2,y1,y2); 	//画垂直线函数
}

void LCD_Fill(unsigned short sx,unsigned short sy,unsigned short ex,unsigned short ey,unsigned long color)		   				//填充单色
{
	unsigned short x,y;
	y = POINT_COLOR;
	POINT_COLOR = color;
	if(sy>ey)
	{
		x=sy;
		sy=ey;
		ey=x;
	}
	for(x=sy;x<=ey;x++)
	{
		LCD_Displayhline(sx,ex,x);
	}
	POINT_COLOR = y;
	
}

void Draw_Circle(unsigned short x0,unsigned short y0,unsigned short r)
{
	int a,b;
	int di;
	if(r==0) return;    //参数过滤，此液晶显示的最大圆半径为31
	a=0;
	b=r;
	di=3-(r<<1);       //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,POINT_COLOR);  //3           
		LCD_DrawPoint(x0+b,y0-a,POINT_COLOR);  //0           
		LCD_DrawPoint(x0-a,y0+b,POINT_COLOR);  //1       
		LCD_DrawPoint(x0-b,y0-a,POINT_COLOR);  //7           
		LCD_DrawPoint(x0-a,y0-b,POINT_COLOR);  //2             
		LCD_DrawPoint(x0+b,y0+a,POINT_COLOR);  //4               
		LCD_DrawPoint(x0+a,y0-b,POINT_COLOR);  //5
		LCD_DrawPoint(x0+a,y0+b,POINT_COLOR);  //6 
		LCD_DrawPoint(x0-b,y0+a,POINT_COLOR);             
		a++;
											//使用Bresenham算法画圆
		if(di<0)
		di +=4*a+6;
		else
		{
			di +=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b,POINT_COLOR);
	}
}
