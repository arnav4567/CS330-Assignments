// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int x = fork();
  int i;
  if(x==0)
  {
	printf("Entered child with pid : %d\n", getpid());
	int p = waitpid(-1, &x);
	sleep(3);
        printf("From child, return value of waitpid is : %d\n", p);
  }

  else
  {
	printf("Entered parent with pid : %d\n", getpid());
	int p = waitpid(x, &i);
        printf("From parent, return value of waitpid is : %d\n", p);
  }
  if(x==0)
	printf("Child is exiting\n");
  else
	printf("Parent is exiting\n");
  exit(0);
}
