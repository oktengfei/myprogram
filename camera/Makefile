CC = arm-S3C2440-linux-gnueabi-gcc



object = framebuffer.o TFTlcd.o main.o
program: $(object)
	$(CC) -o program $^ ${LIBPATH}

%.o : %.S
	$(CC) -o $@ $< -c

%.o : %.c
	$(CC) -o $@ $< -c

clean:
	rm *.o *.elf *.bin *.dis -rf
