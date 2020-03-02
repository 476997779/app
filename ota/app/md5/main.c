#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int wy_ota_check_md5(void)
{
	int ret = 5;
	ret = system("md5sum -c ./md5.txt");
	printf("ret = %d \n",ret);
	
	if(0 == WEXITSTATUS(ret)){
		printf("md5 check success\n");
		return 0;
	}else{
		printf("md5 check failed\n");
		return -1;
	}
}

int wy_ota_update(void)
{
	system("./update/update.sh");
}
int main(int argc, const char *argv[])
{
	wy_ota_check_md5();

//wy_ota_update();

	return 0;
}
