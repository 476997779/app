#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
int main()
{
struct timeval starttime,endtime;
gettimeofday(&starttime,0);
sleep(5);
gettimeofday(&endtime,0);

double timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
timeuse /=1000000;//除以1000则进行毫秒计时，如果除以1000000则进行秒级别计时，如果除以1则进行微妙级别计时
printf("%f\n",timeuse);
}
