#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int wy_unzip_file(const char *zip,const char *dir)
{
	int ret = 50;
	char buff[64] = {0};
	
	sprintf(buff,"unzip %s -d %s",zip,dir);
	
	printf("buff = %s \n",buff);
	
	ret = system(buff);
	
	printf("ret = %d\n",ret);
	
	if(0 == WEXITSTATUS(ret)){
		printf("unzip success\n");
		return 0;
	}else{
		printf("unzip failed\n");
		return -1;
	}



}

int main(int argc, const char *argv[])
{
	wy_unzip_file("~/app/test/update.zip","~/app/test/");

	return 0;
}
