#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOG(text) printf("%s\n", text)
#define FATAL(text)                                                            \
  LOG(text);                                                                   \
  exit(-1);
#define LEN 256

int lowest = INT32_MAX;

void process_dir(const char *path, int depth, char *result) {

  if (depth == 0) {
    return;
  }

  DIR *dp;
  dp = opendir(path);
  if (dp == NULL) {
    LOG("Ne moze da otvori direktorijum");
    return;
  }

  struct dirent *dir;

  while ((dir = readdir(dp)) != NULL) {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
      continue;
    }

    char new_path[LEN];
    strcpy(new_path, path);
    strcat(new_path, "/");
    strcat(new_path, dir->d_name);

    struct stat stat_buf;
    if (stat(new_path, &stat_buf)) {
      LOG("Ne radi stat");
      return;
    }

    if (S_ISREG(stat_buf.st_mode) && stat_buf.st_size < lowest &&
        stat_buf.st_size > 1024) {
      lowest = stat_buf.st_size;
      strcpy(result, new_path);
    } else if (S_ISDIR(stat_buf.st_mode)) {
      process_dir(new_path, depth - 1, result);
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    FATAL("2 dodatna parametra [PATH] [BROJ PODIREKTORIJUMA]");
  }
  if (atoi(argv[2]) == 0) {
    FATAL("2. parametar nije broj");
  }

  struct stat stat_buf;
  if (stat(argv[1], &stat_buf)) {
    FATAL("prvi argument je los");
  }

  if (!S_ISDIR(stat_buf.st_mode)) {
    FATAL("Ovo nije putanja do direktorijuma");
  }

  char result[LEN];
  process_dir(argv[1], atoi(argv[2]), result);

  printf("resenje je %s\n", result);
  LOG("Zavrsio se program");
}
