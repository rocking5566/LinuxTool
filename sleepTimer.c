#include <stdio.h>
#include <time.h>
#include <unistd.h>



static void my_sleep(struct timespec *exp, unsigned int *timeAdjust)
{
	struct timespec     delay;
	struct timespec     now;

	clock_gettime(CLOCK_REALTIME, &now);

	*timeAdjust = 0;

	if ((exp->tv_sec > now.tv_sec) || ((exp->tv_sec == now.tv_sec) && (exp->tv_nsec > now.tv_nsec)))
	{
		delay.tv_sec = 0;

		if (exp->tv_nsec < now.tv_nsec)
			delay.tv_nsec = exp->tv_nsec + 1000000000 - now.tv_nsec;
		else
			delay.tv_nsec = exp->tv_nsec - now.tv_nsec;

		if ((exp->tv_sec - now.tv_sec) <= 1)
		{
			if (delay.tv_nsec >= 10000000)
				nanosleep(&delay, 0);
			else
			{
				exp->tv_sec = now.tv_sec;
				exp->tv_nsec = now.tv_nsec;
				*timeAdjust = delay.tv_nsec;
			}
		}
		else
		{
			delay.tv_nsec = 10000000;
			nanosleep(&delay, 0);
			clock_gettime(CLOCK_REALTIME, exp);
			printf("[P]Time changed - Last: %ld.%09ld ,Now:%ld.%09ld\n", now.tv_sec, now.tv_nsec, exp->tv_sec, exp->tv_nsec);
		}
	}
	else
	{
		unsigned int diff = now.tv_sec - exp->tv_sec;

		if (diff > 1)
		{
			delay.tv_sec = 0;
			delay.tv_nsec = 10000000;
			nanosleep(&delay, 0);
			clock_gettime(CLOCK_REALTIME, exp);
			printf("[P]Time changed - Last: %ld.%09ld ,Now:%ld.%09ld\n", now.tv_sec, now.tv_nsec, exp->tv_sec, exp->tv_nsec);
		}
		else
		{
			if (((1000000000*diff) + now.tv_nsec - exp->tv_nsec)/1000000 >= 500)
			{
				delay.tv_sec = 0;
				delay.tv_nsec = 10000000;
				nanosleep(&delay, 0);
				clock_gettime(CLOCK_REALTIME, exp);
			}
		}
	}
}




int main()
{
	int i, ret;
	unsigned int timeAdjust = 0;
	struct timespec     exp, now;

	clock_gettime(CLOCK_REALTIME, &exp);

   while(1)
   {
   		exp.tv_nsec = exp.tv_nsec + 1000000000 + timeAdjust;

		if (exp.tv_nsec >= 1000000000)
		{
			exp.tv_nsec -= 1000000000;
			exp.tv_sec++;
		}

		clock_gettime(CLOCK_REALTIME, &now);

		printf("------------ time = %u, sec=%u, nsec=%u\n", time(0), now.tv_sec, now.tv_nsec);

		for(i=0; i<3; i++)
			usleep(i*50);

		my_sleep(&exp, &timeAdjust);

	}

	return 0;
}


