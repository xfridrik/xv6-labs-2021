// program print path to files with declared name from declared directory*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *directory, char *file)
{
  int fd;
  struct dirent de;
  struct stat st;
  char buf[512], *p;

  if((fd=open(directory,0)) < 0){
    fprintf(2, "find: cant open directory\n");
    exit(-1);
  }

  if(strlen(directory) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("find: path too long\n");
    exit(-1);
  }

  strcpy(buf,directory);
  p=buf+strlen(buf);
  *p++='/';

  // algorithm inspired by program ls.c

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    // ensure right file name lenght
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(stat(buf, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", buf);
      exit(-1);
    }

    // file found
    if(st.type == T_FILE){
      if(strcmp(de.name,file) == 0){
        printf("%s\n",buf);
      }
    }
    // folder - recursion
    if(st.type == T_DIR){
      if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0){
        continue;
      }
      // repeat with current buffer
      find(buf,file);
    }
  }
}

int
main(int argc, char *argv[])
{
  if(argc!=3){
    fprintf(2, "Usage: find <directory> <file>\n");
    exit(-1);
  }
  find(argv[1],argv[2]);
  exit(0);
}