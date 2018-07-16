// name: Chuxuan Zhang
// student ID: 301267261
// data: 2017/12/1

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>

const int test_time = 10000; //times for testing and get the average
const int sec_to_nano = 1000000000; // convert from second to nanoseconds


//initialize

struct timespec func_start, func_end;
struct timespec sys_start, sys_end;
struct timespec rw_sys_start, rw_sys_end;


double convert_time(struct timespec* t)
{
  return t -> tv_sec * sec_to_nano + t -> tv_nsec;
}


void func_call(){};

// calculating the cost of minimal function call
int min_func_cost()
{
  int func_total = 0;
  int func_ave = 0;

  for (int i = 0; i < test_time; i++)
  {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&func_start);
    func_call(); // a function call
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&func_end);

    func_total = func_total+ (convert_time(&func_end) - convert_time(&func_start));
  }
  return func_ave = func_total / test_time;
}


int min_sys_cost()
{
  int sys_total = 0;
  int sys_ave = 0;
  for (int i = 0; i < test_time; i++)
  {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sys_start);
    syscall(SYS_getpid); //
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sys_end);

    sys_total = sys_total + (convert_time(&sys_end) - convert_time(&sys_start));
  }
  return sys_ave = sys_total / test_time;

}

int rw_sys_cost()
{
  pid_t pid;
  char a = 'b'; //transfer one byte
  int rw_sys_total = 0;
  int rw_sys_ave = 0;

  for (int i = 0; i < test_time; i++)
  {
    int pipe_process[2]; // set up a pipe process
    pid = fork(); // fork a child process
    if (pid < 0) // fork error
    {
      perror("fork error");
      exit(1);
    }
    else if (pid > 0) //parent process
    {
      close(pipe_process[0]);
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&rw_sys_start);
      write(pipe_process[1], &a, sizeof(a)); //write parents
      wait(NULL);
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&rw_sys_end);
      close(pipe_process[1]); // c;ose all the processes
    }
    else // child process
    {
      close(pipe_process[1]);
      read(pipe_process[0], &a, sizeof(a)); // read from parents
      exit(0);
    }

    rw_sys_total = rw_sys_total + (convert_time(&rw_sys_end) - convert_time(&rw_sys_start));

  }
  return rw_sys_ave = rw_sys_total / test_time;
}



int main(int argc, char* argv[])
{
  printf("\n");
  printf("Assignment 3 : Calculating the time to perform various function calls");
  printf("\n\n");
  int func_cost = min_func_cost();
  printf("the cost of minimal function call:  ");
  printf("%d", func_cost);
  printf(" nanoseconds");
  printf("\n\n");

  int sys_cost = min_sys_cost();
  printf("the cost of minimal system call:  " );
  printf("%d", sys_cost);
  printf(" nanoseconds");
  printf("\n\n");

  int rw_cost = rw_sys_cost();
  printf("the cost of a read/write system call:  ");
  printf("%d", rw_cost);
  printf(" nanoseconds");
  printf("\n\n");
}
