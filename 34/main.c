#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
struct file {
  char path[PATH_MAX+1];
  long size;
};

struct file largest = {"", 0};
struct file smallest = {"", LONG_MAX};

void recsearch(char *path);

int main(int argc, char **argv){
  if(argc != 2)
    return -1;
  char* argpath = realpath(argv[1], NULL);
  if (argpath==NULL)
    return -1;
  char argpath2[PATH_MAX+1];
  strcpy(argpath2, argpath);
  strcat(argpath2, "/");
  recsearch(argpath2);
  printf("largest:\n%s - %ld bytes\n", largest.path, largest.size);
  printf("smallest:\n%s - %ld bytes\n", smallest.path, smallest.size);
  return 0;
}

void recsearch(char *path){
  DIR *dirptr = opendir(path);
  struct dirent *dirent;
  struct stat statbuf;
  for(dirent = readdir(dirptr); dirent!=NULL; dirent  = readdir(dirptr)){
    if(strcmp(".", dirent->d_name)==0)
      continue;
    if(strcmp("..", dirent->d_name)==0)
      continue;
    char pathbuffer[PATH_MAX+1] = "";
    strcat(pathbuffer, path);
    strcat(pathbuffer, dirent->d_name);
    if(dirent->d_type==DT_REG){
      lstat(pathbuffer, &statbuf);
      if(statbuf.st_size > largest.size){
        largest.size = statbuf.st_size;
        strcpy(largest.path, pathbuffer);
      }
      if(statbuf.st_size < smallest.size){
        smallest.size = statbuf.st_size;
        strcpy(smallest.path, pathbuffer);
      }
    } else if (dirent->d_type==DT_DIR){
      strcat(pathbuffer, "/");
      // printf("%s\n", pathbuffer);
      recsearch(pathbuffer);
    }
  }
}
