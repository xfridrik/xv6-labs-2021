//sending "p" from parent to child
//and "c" from child to parent via pipe and
//print ping/pong when received

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
int pipetoparent[2];
int pipetochild[2];

if(pipe(pipetoparent)!=0 || pipe(pipetochild)!=0){
  fprintf(2,"pipe unssucessful\n");
  exit(-1);
}

int pid=fork();
if(pid<0){
  fprintf(2,"fork unssucessful\n");
  exit(-1);
}

if(pid==0){
  //child process
  char received;
  read(pipetochild[0], &received, 1);
  printf("%d: received ping\n", getpid());
  write(pipetoparent[1], "c", 1);
}else{
  //parent process
  char received;
  write(pipetochild[1], "p", 1);
  read(pipetoparent[0], &received, 1);
  printf("%d: received pong\n", getpid());
}
exit(0);
}