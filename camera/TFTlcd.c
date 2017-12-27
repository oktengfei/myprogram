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



//==========д8*16�ַ�  x ������ y ������ c ��Ҫ��ʾ���ַ� fColor �ַ���ɫ bColor �ַ�������ɫ==========================
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char size,unsigned char mode)
{  							  
    unsigned char temp,t1,t;
	unsigned short y0=y;
	unsigned short colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12) temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y,POINT_COLOR);	
				temp<<=1;
				y++;
				if(x>=240){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=240){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y,POINT_COLOR); 
				temp<<=1;
				y++;
				if(x>=320){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=320){POINT_COLOR=colortemp;return;}//��������
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
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
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
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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

//��ʾʮ������,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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

//��ʾ��������,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//remain:��ʾ��С��λ��
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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
//=====   ��ˮƽ�ߺ���   =====
void LCD_Displayhline(unsigned short x0, unsigned short x1, unsigned short y) 	 //����������  һ��������  �Լ���ɫ
{  
	unsigned char  bak;
	if(x0>x1)        // ��x0��x1��С�������У��Ա㻭ͼ
	{  
		bak = x1;
		x1 = x0;
		x0 = bak;
	}
	do
	{  
		LCD_DrawPoint(x0,y,POINT_COLOR);  // �����������ʾ�������ֱ��
		x0++;
	}while(x1>=x0);
}


//=====   ����ֱ�ߺ���   =====
void LCD_Displayrline(unsigned short x, unsigned short y0, unsigned short y1) 	//һ�������꣬���������꣬��ɫ
{  
	unsigned char  bak;
	if(y0>y1)        // ��y0��y1��С�������У��Ա㻭ͼ
	{  
		bak = y1;
		y1 = y0;
		y0 = bak;
	}
	do
	{  
		LCD_DrawPoint(x,y0,POINT_COLOR);   // ���ϵ��������ʾ�������ֱ��
		y0++;
	}while(y1>=y0);
}

void LCD_DrawLine(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1)

{  
	int dx;       		// ֱ��x���ֵ����
	int dy;       		// ֱ��y���ֵ����
	signed char dx_sym;      	// x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	signed char dy_sym;      	// y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	int dx_x2;       	// dx*2ֵ���������ڼӿ������ٶ�
	int dy_x2;       	// dy*2ֵ���������ڼӿ������ٶ�
	int di;       		// ���߱���
	
	if(x0==x1)         	//�ж��Ƿ�Ϊ��ֱ��
	{
		LCD_Displayrline(x0,y0,y1);    //����ֱ��
		return;
	}
	if(y0==y1)         	//�ж��Ƿ�Ϊˮƽ��
	{
		LCD_Displayhline(x0,x1,y0);    //��ˮƽ��
		return;
	}
	dx = x1-x0;         // ��ȡ����֮��Ĳ�ֵ
	dy = y1-y0;
	//�ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� 
	if(dx>0)         	// �ж�x�᷽��
		dx_sym = 1;        // dx>0������dx_sym=1
	else
		dx_sym = -1;      	// dx<0������dx_sym=-1
	if(dy>0)         			// �ж�y�᷽��
		dy_sym = 1;        		// dy>0������dy_sym=1
	else
		dy_sym = -1;      	// dy<0������dy_sym=-1
							//��dx��dyȡ����ֵ 
	dx = dx_sym * dx;
	dy = dy_sym * dy;
							// ����2����dx��dyֵ 
	dx_x2 = dx<<1;
	dy_x2 = dy<<1;
							// ʹ��Bresenham�����л�ֱ�� 
	if(dx>=dy)         		// ����dx>=dy����ʹ��x��Ϊ��׼	��ֱ�ߵ�б��С��1
	{  
		di = dy_x2 - dx;
		while(x0!=x1)
		{  
			LCD_DrawPoint(x0, y0,POINT_COLOR);
			x0 += dx_sym;
			if(di<0)
			{  
				di += dy_x2;     // �������һ���ľ���ֵ
			}
			else
			{  
				di += dy_x2 - dx_x2;
				y0 += dy_sym;
			}
		}
	  LCD_DrawPoint(x0, y0,POINT_COLOR);    // ��ʾ���һ��
	}
	else          // ����dx<dy����ʹ��y��Ϊ��׼
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
		LCD_DrawPoint(x0, y0,POINT_COLOR);    // ��ʾ���һ��
	} 
}


void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
	LCD_Displayhline(x1,x2,y1);
	LCD_Displayhline(x1,x2,y2);
	LCD_Displayrline(x1,y1,y2); 	//����ֱ�ߺ���
	LCD_Displayrline(x2,y1,y2); 	//����ֱ�ߺ���
}

void LCD_Fill(unsigned short sx,unsigned short sy,unsigned short ex,unsigned short ey,unsigned long color)		   				//��䵥ɫ
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
	if(r==0) return;    //�������ˣ���Һ����ʾ�����Բ�뾶Ϊ31
	a=0;
	b=r;
	di=3-(r<<1);       //�ж��¸���λ�õı�־
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
											//ʹ��Bresenham�㷨��Բ
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
