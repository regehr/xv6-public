#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int
main(void)
{
  int i, res;
  i = mutex_create();
  if (i < 0) {
    printf(1, "oops, couldn't create a mutex\n");
    exit();
  }
  res = mutex_acquire(i);
  printf(1, "mutex_acquire(%d) = %d\n", i, res);
  res = mutex_release(i);
  printf(1, "mutex_release(%d) = %d\n", i, res);
  res = mutex_destroy(i);
  printf(1, "mutex_destroy(%d) = %d\n", i, res);
  exit();
}
