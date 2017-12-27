#include "framebuffer.h"
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"      // for open();
#include <linux/fb.h>
#include <stdlib.h>     // for exit();
#include <sys/ioctl.h>  // for ioctl();
#include <sys/mman.h>   // for mmap(); munmap();

char LCD_Type;  // 0: 16bit_565  1: 32bit_888

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
short *fbp565=NULL;
int *fbp888=NULL;

int fd;
long int screen_size=0;
int frame_init(void)
{
    
    fd=open("/dev/fb0",O_RDWR);
    if(fd<0)
    {
        printf("Error: Cannot open framebuffer device.\r\n");
        return 1;
    }

    if(ioctl(fd,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error: Reading fixed information failed.\r\n");
        return 2;
    }

    if(ioctl(fd,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error: Reading variable information failed.\r\n");
        return 3;
    }

    printf("Screen_width=%d,Screen_height=%d\r\n",vinfo.xres,vinfo.yres);
    printf("Screen_color_bit_per_pixel=%d\r\n",vinfo.bits_per_pixel);
    screen_size = vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;
    
    if(vinfo.bits_per_pixel==16)
    {
        LCD_Type = 0; // 16bit_565
        fbp565=(short *)mmap(0,screen_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        // map framebuffer to user memory
        if((int)fbp565 == -1)
        {
            printf("Error: Failed to map framebuffer device to memory.\r\n");
            return 4;
        }

    }
    else if(vinfo.bits_per_pixel==32)
    {
        LCD_Type = 1; // 32bit_888
    
        fbp888=(int *)mmap(0,screen_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        // map framebuffer to user memory
        if((int)fbp888 == -1)
        {
            printf("Error: Failed to map framebuffer device to memory.\r\n");
            return 4;
        }
    }
    else
    {
        printf("Error: Unknown Lcd tyle.\r\n");
        return 5;
    }
    

              

    return 0;

}

int frame_close(void)
{
    if(LCD_Type)
        munmap(fbp888,screen_size);
    else
        munmap(fbp565,screen_size);

    close(fd);
    return 0;
}


void LCD_DrawPoint(unsigned int x, unsigned int y,unsigned int color)
{
   if(LCD_Type)
        *(fbp888 + y*(vinfo.xres) +x)=((color&0xf800)<<8)|((color&0x7e0)<<5)|((color&0x1f)<<3);
        //*(fbp888 + y*vinfo.xres +x)=color;
   else
        *(fbp565 + y*vinfo.xres +x)=color;
}

unsigned int LCD_GetPoint(unsigned int x,unsigned int y)
{
    if(LCD_Type)
      return *(fbp888 + y*vinfo.xres +x);
    else
      return *(fbp565 + y*vinfo.xres +x);

}
