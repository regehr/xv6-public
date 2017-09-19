#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define KIDS 4

int dummy;

void work(int m) {
  for (int n = 0; n < 10000; ++n) {
    int res;
    res = mutex_acquire(m);
    //printf(1, "%d : mutex_acquire(%d) = %d ", getpid(), m, res);
    res = mutex_release(m);
    //printf(1, "%d : mutex_release(%d) = %d\n", getpid(), m, res);
    dummy = res;
  }
}

int
main(void)
{
  int m = mutex_create();
  if (m < 0) {
    printf(1, "oops, couldn't create a mutex\n");
    exit();
  }
  for (int i = 0; i < KIDS; ++i) {
    int pid = fork();
    if (pid < 0) {
      printf(1, "oops, fork failed\n");
      exit();
    }
    if (pid == 0) {
      work(m);
      exit();
    }
  }
  while (1) {
    int res = wait();
    if (res < 0) {
      printf(1, "all kids exited, main is done\n");
      int res = mutex_destroy(m);
      printf(1, "mutex_destroy(%d) = %d\n", m, res);
      exit();
    }
  }
}
