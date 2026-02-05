#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_ARG "Err: Treba arugment za putanju do direktorijuma\n"
#define ERR_STAT "Err: Status unetete putanje ne valja\n"
#define ERR_ISDIR "Err: Unet argument ne predstavlja putanju do dir\n"
#define ERR_DIR "Err: Greska prilikom otvaranja direktorijuma\n"

#define MAX_LEN 1024

static int najveci = 0;

typedef struct stat Stat;
typedef struct dirent Dirent;

void processdir(char *path, int level, char *resenje) {
  DIR *dp;
  Dirent *dir;
  Stat statbuf;

  char path1[MAX_LEN];

  if (level >= 3) {
    return;
  }

  dp = opendir(path);
  if (dp == NULL) {
    printf(ERR_DIR);
    exit(-1);
  }

  while ((dir = readdir(dp)) != NULL) {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
      continue;
    }

    strcpy(path1, path);
    strcat(path1, "/");
    strcat(path1, dir->d_name);

    if (stat(path1, &statbuf)) {
      printf(ERR_STAT);
      return;
    }

    if (S_ISREG(statbuf.st_mode) && statbuf.st_size > najveci) {
      printf("File je\n");
      najveci = statbuf.st_size;
      strcpy(resenje, path1);
    } else if (S_ISDIR(statbuf.st_mode)) {
      processdir(path1, level + 1, resenje);
    }
  }
  closedir(dp);
}

int main(int argc, char *argv[]) {

  printf("path: %s\n", argv[0]);

  if (argc != 2) {
    printf(ERR_ARG);
    exit(-1);
  }

  Stat stat_buf;

  if (stat(argv[1], &stat_buf)) {
    printf(ERR_STAT);
    exit(-1);
  }

  if (!S_ISDIR(stat_buf.st_mode)) {
    printf(ERR_ISDIR);
    exit(-1);
  }

  char resenje[MAX_LEN];

  processdir(argv[1], 0, resenje);

  printf("Resenje je %s\n", resenje);
  return 0;
}
