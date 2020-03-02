#include <stdio.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <sys/mman.h>  
#include <linux/fb.h>  
#include <string.h>
#define COLOR_WRITE 0xFFFF  
#define COLOR_BLACK 0x0000  
#define COLOR_BLUE 0xFF00  

struct fb_fix_screeninfo fbfix = { 0 };  
struct fb_var_screeninfo fbvar = { 0 };  
long screensize = 0;  
int fdfb = -1;  
short *fb32 = NULL;  

const int stride;
static int location = 0;
int i = 0;
int x = 0;  
int y = 0;  
int j = 0;



int main(int argc, const char *argv[])
{
	fdfb = open("/dev/fb0", O_RDWR);  
	if( 0 > fdfb) {  
		printf("Failure to open framebuffer device: /dev/fb0 !\n");  
		exit( -1 );  
	}  
#if 1
	if( ioctl( fdfb, FBIOGET_FSCREENINFO, &fbfix ) ) {  
		printf("Failure to get fixed screen information !\n");  
		exit( -2 );  
	}  


	if( ioctl( fdfb, FBIOGET_VSCREENINFO, &fbvar ) ) {  
		printf("Failure to get varible screen information !\n");  
		exit( -3 );  
	}  

	printf("bits_per_pixel 0x%x\n",fbvar.bits_per_pixel);
	//screensize = fbvar.xres * fbvar.yres * ( fbvar.bits_per_pixel / 8 );  
	screensize = fbfix.smem_len;
	printf("screensize is %d\n",screensize);


	fb32 = (short *)mmap( 0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fdfb, 0 );  if( NULL == fb32 ) {  printf("Failure to map framebuffer device memory to process's memory!\n");  
		exit( -4 );  }
#endif memset(fb32,0xFF,screensize);
#if 0	foyCC
SS
			}  } #if	1	
	

				*( fb32 + y * fbvar.xres + x ) = COLOR_WRITE;  } 
		} #endif 
#endif
			
//ioctl(fdfb,FBIOPUT_VSCREENINFO,&fb32);
 //  ioctl(fdfb,FBIOPAN_DISPLAY,&fb32); munmap( fb32, screensize );  

printf("Finished to demo to operate framebuffer !\n");  
close(fdfb);  return 0;
}
