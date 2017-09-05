#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

void assert(int x) {
  if (!x) {
    printf(1, "assertion failed, exiting\n");
    exit();
  }
}

unsigned xorshift32(unsigned *state) {
  unsigned x = *state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *state = x;
  return x;
}

const int N = 1 * 1000 * 1000;
#define B 4096

unsigned size_state;

int randlen(void) {
  int i;
  do {
    i = 1 + (int)xorshift32(&size_state) % B;
  } while (i < 1);
  return i;
}

char buf[B];

int main(void) {
  unsigned stream_state = getpid();
  int pipefd[2];
  int res = pipe(pipefd);
  assert(res == 0);
  int pid = fork();
  size_state = (unsigned)getpid() * 7;
  assert(pid >= 0);
  if (pid == 0) {
    close(pipefd[1]);
    int r = 0;
    int z;
    do {
      int l = randlen();
      z = read(pipefd[0], buf, l);
      assert(z >= 0);
      for (int i = 0; i < z; ++i)
        assert(buf[i] == (char)xorshift32(&stream_state));
      r += z;
    } while (z != 0);
    printf(1, "child process read %d bytes\n", r);
  } else {
    close(pipefd[0]);
    int w = 0;
    int z;
    while (w < N) {
      int l = randlen();
      for (int i = 0; i < l; ++i)
        buf[i] = xorshift32(&stream_state);
      z = write(pipefd[1], buf, l);
      if (l != z)
        printf(1, "short write %d\n", z);
      assert(z > 0);
      w += z;
    }
    printf(1, "parent process wrote %d bytes\n", w);
  }
  return 0;
}
