#ifndef _LCD_BASE_H_
#define _LCD_BASE_H

extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;
extern char LCD_Type;  // 0: 16bit_565  1: 32bit_888

extern short *fbp565;
extern int *fbp888;

int frame_init(void);
int frame_close(void);

void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);
unsigned int LCD_GetPoint(unsigned int x,unsigned int y);
#endif
