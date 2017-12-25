#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <err.h>
#include <sysexits.h>
#include "safe-fork.h"

int main(void) {
  pid_t pid;
  int pipefd[2];

  pipe(pipefd);
  pid = safe_fork();

  if (pid == 0) { /* child process */
    dup2(pipefd[1], STDOUT_FILENO);
    execl("/usr/bin/wc","wc", "-w", NULL);
    close(pipefd[0]);
    close(pipefd[1]);
  }
  
  else if (pid > 0) { /* parent process */
      int reading_int = 0;
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[1]);
      scanf("%d", &reading_int);
      close(pipefd[0]);
      
      if (reading_int < 200) {
	printf("Too short!\n");
	exit(1);
      }

      else {
	printf("Long enough!\n");
	exit(0);
      }
  }
  
  else {
    err(EX_OSERR, "fork error");
  }
  return 0;
}
