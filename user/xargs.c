// read arguments from input and execute
// provided program for every line of input

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc<2){
    fprintf(2, "xargs: no args\n");
    exit (-1);
  }

  char buf[256]={'\n'};
  char c;
  int j=0;

  // read line into buf
  while(read(0,&c,1)){
    if(j>255){
      fprintf(2,"xargs: too long input arg\n");
      exit(-1);
    }
    if(c=='\n'){
      buf[j]='\n';

      int pid=fork();
        if(pid<0){
          fprintf(2,"fork unssucessful\n");
          exit(-1);
        }

      if(pid==0){
        //child process
        char *arg2[MAXARG];
        // index to input arg array
        int arg_i = 0;
        // index to one arg word
        int i = 0;

        // get args from buffer
        while(buf[i]!='\n' && buf[i]!='\0'){
          if(arg_i-1>MAXARG){
            fprintf(2, "xargs: too many args\n");
            exit (-1);
          }
          arg2[arg_i]=&buf[i];
          arg_i++;
          // iterate to end of arg
          while(buf[i]!='\0' && buf[i]!=' ' && buf[i]!='\n'){ 
            i++;
          }
          buf[i]='\0';
          i++;
        } 

        if(argc-1+arg_i>=MAXARG){
          fprintf(2, "xargs: too many args\n");
          exit (-1);
        }

        // insert argv and input arguments to array
        char *arguments[MAXARG];

        for (int i = 0; i < argc-1; i++){
          arguments[i]=argv[i+1];
        }

        for(int i=argc-1; i<argc-1+arg_i;i++){
          arguments[i]=arg2[i-(argc-1)];
        }

        arguments[argc-1+arg_i]='\0';
        exec(arguments[0],arguments);

      }else{
        // parent process - wait and reset buffer
        wait(0);
        memset(buf,'\n',sizeof(buf));
        j=0;
      }

    }else{
      // add char to buf
      buf[j]=c;
      j++;
    }
  }
exit(0);
}