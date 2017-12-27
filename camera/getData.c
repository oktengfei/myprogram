#include "stdio.h"		// printf
#include "fcntl.h"		// 打开设备 
#include "unistd.h"		// 
#include "stdlib.h"		// calloc()
#include "string.h"		// memset()
#include "sys/mman.h"	// mmap()
#include "sys/ioctl.h"
#include "linux/videodev2.h" // Video for Linux 2

#define REQBUF_COUNT 4
typedef struct {
	void *start;
	unsigned int length;
}BUFFER;

int main(int argv, char *argc[])
{	
	int fd;
	fd = open("/dev/video0",O_RDWR);
	if(fd < 0)
		printf("Error: Open device failed !\n");
	else
	{	
		printf("1. Open device success !\n");

		// 获取设备信息
		struct v4l2_capability cap;
		ioctl(fd,VIDIOC_QUERYCAP,&cap);
		printf("\tDriverName:\t%s\n\tCardName:\t%s\n\t"
			   "BusInfo:\t%s\n\tDriverVersion:\t%u.%u.%u\n",
				cap.driver,cap.card,cap.bus_info,(cap.version>>16)&0xff,
				(cap.version>>8)&0xff,cap.version&0xff);	
		
		// 获取格式
		struct v4l2_fmtdesc fmtdesc;
		fmtdesc.index = 0;
		fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		printf("\n\tSupport Format:\n");
		while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) != -1)
		{
			printf("\t\t\t%d.%s\n",1+fmtdesc.index++,fmtdesc.description);
		}

		struct v4l2_format fmt; 
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		ioctl(fd,VIDIOC_G_FMT,&fmt);
		printf("\n\tCurrentFormat:\n");
		printf("\t\t\twidth:  %d\n\t\t\theight: %d\n",
				fmt.fmt.pix.width,fmt.fmt.pix.height);
		
		fmtdesc.index = 0;
		fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) != -1)
		{
			if(fmtdesc.pixelformat & fmt.fmt.pix.pixelformat)
			{
				printf("\t\t\tformat: %s\n",fmtdesc.description);
				break;
			}
			fmtdesc.index++;
		}

		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
		if(ioctl(fd,VIDIOC_TRY_FMT,&fmt) == -1)
		{
			printf("ERROR: Not support format RGB32\n");
		}
		else 
			printf("\tRGB32 is supported ! \n");

		// 查询BUF
		struct v4l2_requestbuffers reqbuf;
		reqbuf.count = REQBUF_COUNT;
		reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		reqbuf.memory = V4L2_MEMORY_MMAP;
		ioctl(fd,VIDIOC_REQBUFS,&reqbuf);

		BUFFER *buffers;
		buffers = (BUFFER*)calloc(reqbuf.count,sizeof(*buffers));
		if(!buffers)
		{
			printf("ERROR: calloc error !\n");
		}	
		printf("\n\treqbuf.count:\t%d\n\n",reqbuf.count);
		
		unsigned int n_buf;
		for(n_buf=0;n_buf<reqbuf.count;n_buf++)
		{
			struct v4l2_buffer buf;
			memset(&buf,0,sizeof(buf));
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.index = n_buf;

			if(-1 == ioctl(fd,VIDIOC_QUERYBUF,&buf))
			{
				printf("ERROR: querybuf error !\n");
			}
			buffers[n_buf].length = buf.length;
			printf("\tbuf.length: %d, buf.m.offset: %d\n",buf.length,buf.m.offset);
		
			buffers[n_buf].start = mmap(NULL,
										buf.length,
										PROT_READ|PROT_WRITE,
										MAP_SHARED,fd,buf.m.offset);
			if(MAP_FAILED == buffers[n_buf].start)
			{
				printf("ERROR: mmap failed !\n");
			}
		}

		// 将四个缓冲帧放入队列
		unsigned int i;
		enum v4l2_buf_type type;
		for(i=0;i<reqbuf.count;i++)
		{
			struct v4l2_buffer buf;
			memset(&buf,0,sizeof(buf));
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.index = i;
			ioctl(fd,VIDIOC_QBUF,&buf);
		}
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		ioctl(fd,VIDIOC_STREAMON,&type);


		// 处理数据
		unsigned char table[]={"/var/www/html/ramDisk/00.jpg"};
		unsigned char num = 0;
		FILE *fp;
		while(1)
		for(i=0;i<reqbuf.count;i++)
		{
			// 从缓冲区中取出一个缓冲帧
			struct v4l2_buffer buf;
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			ioctl(fd,VIDIOC_DQBUF,&buf);	
			// 图像处理
			// memcpy(ram,buffer[i].start,width*height*2);
			// printf("\tbuffers.length: %d\n",buffers[i].length);
			
			table[34] = num%100/10+0x30;
			table[35] = num%10+0x30;
			fp = fopen(table,"wb");
			if(fp < 0)
				printf("file open failed!");
			else
			{
				fwrite((unsigned char*)buffers[i].start,1,buffers[i].length,fp);
			}
			fclose(fp);

			// 将取出的缓冲帧放回缓冲区
			ioctl(fd,VIDIOC_QBUF,&buf);
			usleep(200000);
		}

		close(fd);
	}

	return 0;
}

