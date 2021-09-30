//print primes using pipes and child processes

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
newprocess(int p[])
{
  close(0);
  dup(p[0]);
  close(p[0]);
  close(p[1]);

  //read first number - prime
  int prime;
  if(!read(0,&prime,sizeof(prime))){
    //no next prime
    exit(0);
  } else{
    printf("prime %d\n",prime);
  }

  //new pipe
  if((pipe(p))!=0){
    fprintf(2,"primes: pipe unssucessful\n");
    exit(-1);
    }

  int pid=fork();

  if(pid<0){
    fprintf(2,"primes: fork unssucessful\n");
    exit(-1);
  }

  if(pid==0){
    //child process - recursion
    newprocess(p);

  } else{
    //parent process
    close(p[0]);
    int receive;
    //read next numbers and send ones
    //which are not divisible with last prime 
    while(read(0,&receive,sizeof(int))){
      if(receive%prime!=0){
        write(p[1],&receive,sizeof(int));
      }
    }
    //close write and wait
    close(p[1]);
    wait(0);
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
int p[2];

if(pipe(p)){
  fprintf(2,"primes: pipe unssucessful\n");
  exit(-1);
}

int pid=fork();
if(pid<0){
  fprintf(2,"primes: fork unssucessful\n");
  exit(-1);
}

if(pid==0){
  //child process
  newprocess(p);

} else{
  //parent process
  for(int i=2; i<36; i++){
    write(p[1],&i,sizeof(int));
  }
  close(p[1]);
  //wait for all processes to complete
  wait(0);

}
exit(0);
}