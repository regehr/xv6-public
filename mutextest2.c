#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int
main(void)
{
  for (int x = 0; x < 20;  ++x) {
    int i, res;
    i = mutex_create();
    if (i < 0) {
      printf(1, "oops, couldn't create a mutex\n");
      exit();
    }
    for (int n = 0; n < 10; ++n) {
      res = mutex_acquire(i);
      printf(1, "mutex_acquire(%d) = %d ", i, res);
      res = mutex_release(i);
      printf(1, "mutex_release(%d) = %d\n", i, res);
    }
    if ((x & 1) == 0) {
      res = mutex_destroy(i);
      printf(1, "mutex_destroy(%d) = %d\n", i, res);
    }
  }
  exit();
}
