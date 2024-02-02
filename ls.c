/**
 * ls.c (C) 2024
 *
 * author: Walid Abu Al-Afia
 * description: space and time efficient implementation of good ole' ls.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLUE "\x1b[1m\033[34m"
#define CYAN "\x1b[1m\033[36m"
#define GREEN "\x1b[1m\033[32m"
#define WHITE "\x1b[37m"

/**
 * HELPER FUNCTIONS (Local)
 */
int compare_names(const void *str1, const void *str2) {
  return strcmp(*(const char **)str1, *(const char **)str2);
}

/**
 * Usage - this function prints the proper usage instructions to the user.
 *
 * parameters: None.
 */
void Usage() {
  fprintf(stderr, "\nUsage: ls [OPTION]... [DIR]...\n");
  fprintf(stderr, "List current working directory contents.\n");

  fprintf(stderr, "\nOptions\n");
  fprintf(stderr, "-R\tlist subdirectories recursively\n");
  return;
}

void simple(char *path) {
  // Directory path variable
  DIR *dp;

  // Open the directory
  if (!(dp = opendir(path))) {
    perror(path);
    return;
  }

  // struct dirent *entry;                // entry struct
  // const int max_entries = 512;         // max number of entries allowed
  //
  //
  struct dirent **entries; // array holding pointers to entries
  int entry_count;

  if ((entry_count = scandir(path, &entries, NULL, alphasort)) < 0) {
    perror("scandir");
    closedir(dp);
    return;
  }

  // Sort the entries alpha-numerically
  qsort(entries, entry_count, sizeof(struct dirent *), compare_names);

  for (int i = 0; i < entry_count; i++) {
    // Skip dotfiles
    if ((strncasecmp(entries[i]->d_name, ".", 1)) == 0) {
      free(entries[i]);
      continue;
    }
    // Print GREEN for files
    if (entries[i]->d_type == DT_REG)
      printf(GREEN "%s  " WHITE, entries[i]->d_name);
    // Print BLUE for directories
    if (entries[i]->d_type == DT_DIR)
      printf(BLUE "%s  " WHITE, entries[i]->d_name);
    // Print CYAN for symlinks
    if (entries[i]->d_type == DT_LNK)
      printf(CYAN "%s  " WHITE, entries[i]->d_name);
    free(entries[i]);
  }
  printf("\n");

  free(entries);

  // Close the directory
  closedir(dp);
  return;
}

void RecDir(char *path, int flag) {
  DIR *dp = opendir(path);

  if (!dp) {
    perror(path);
    return;
  }

  struct dirent *ep;
  char newdir[512];
  printf(BLUE "\n%s :\n" WHITE, path);
  while ((ep = readdir(dp)))
    if (strncmp(ep->d_name, ".", 1))
      printf(GREEN "\t%s\n" WHITE, ep->d_name);
  closedir(dp);
  dp = opendir(path);
  while ((ep = readdir(dp)))
    if (strncmp(ep->d_name, ".", 1)) {
      if (flag && ep->d_type == 4) {
        sprintf(newdir, "%s/%s", path, ep->d_name);
        RecDir(newdir, 1);
      }
    }
  closedir(dp);
}

int main(int argc, char **argv) {
  char cwd[512];

  switch (argc) {
  case 1:
    // if there is only one arg,
    //   then print cwd contents
    if (!(getcwd(cwd, sizeof(cwd)))) {
      perror("getcwd()");
      return 1;
    } else {
      // no-flag ls
      simple(cwd);
    }
    break;
    // if there are two command line vars
  case 2:
    if (strcmp(argv[1], "-R") == 0)
      Usage();
    else
      simple(argv[1]);
    break;
  case 3:
    if (strcmp(argv[1], "-R") == 0)
      RecDir(argv[2], 1);
    else
      Usage();
    break;
  default:
    Usage();
  }

  return 0;
}
