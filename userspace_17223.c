#include <linux/kernel.h>
#include <sys/syscall.h>
#include  <sys/types.h>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <sys/wait.h>


inline
uint64_t readTSC() {
	uint64_t tsc = __rdtsc();
	return tsc;
}

int main()
{
	pid_t  pid, wpid;
	unsigned long long a,b,a2;
	int status;

	//Measuring custom sys call
	a = readTSC();
	long int ret_val = syscall(548);
	b = readTSC();
	printf("System call my_precious returned %ld in %llu\n", ret_val, b-a);

	// Measuring fork() sys call
	a2 = readTSC();
        pid = fork();

	if(pid < 0){
		printf("Failed to fork process.\n");
		exit(1);
	}
	else if(pid == 0){
		//child process
		b = readTSC();
		printf("Fork created child pid %ld in %llu\n",(long) pid, b-a2);
		exit(0);
	}
	else{
		//parent process
		b = readTSC();
        	wpid = wait(&status);
		
		if(WIFEXITED(status)){
			printf("Fork returned parent pid %ld in %llu\n",(long) pid, b-a2);
		}
	}
	return 0;
}
