#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

void assert(int x, int n) {
  if (!x) {
    printf(1, "assertion %d failed, exiting\n", n);
    exit();
  }
}

const int BYTES = 100 * 1000 * 1000;
const int BUFSIZE = 4096;

#define min(x,y) (((x)<(y))?(x):(y))

void *malloc_pagealigned(int s) {
  char *m = malloc(s + 4096);
  assert(m != 0, 5);
  m += 4096;
  m = (char *)((unsigned long)m & ~4095);
  return m;
}

int main(void) {
  int start = uptime();
  int pipefd[2];
  int res = pipe(pipefd);
  assert(res == 0, 6);
  char *buf = malloc_pagealigned(BUFSIZE);
  for (int i=0; i<BUFSIZE; ++i)
    buf[i] = 1;
  int pid = fork();
  assert(pid >= 0, 7);
  if (pid == 0) {
    // child
    close(pipefd[0]);
    int bytes = BYTES;
    int nwrote = 0;
    do {
      int res = write(pipefd[1], buf, min(bytes, BUFSIZE));
      // printf(1, "wrote %d out of %d\n", res, min(bytes, BUFSIZE));
      assert(res >= 0, 8);
      nwrote += res;
      bytes -= res;
    } while (bytes > 0);
    close(pipefd[1]);
    printf(1, "child process wrote %d bytes\n", nwrote);
  } else {
    // parent
    int nread = 0;
    close(pipefd[1]);
    while (1) {
      int res = read(pipefd[0], buf, BUFSIZE);
      // printf(1, "wrote %d out of %d\n", res, BUFSIZE);
      if (res < 1)
        break;
      nread += res;
    }
    printf(1, "parent process read %d bytes\n", nread);
    close(pipefd[0]);
    int stop = uptime();
    printf(1, "ticks elapsed = %d\n", stop - start);
    wait();
  }
  return 0;
}
