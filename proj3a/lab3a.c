#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>

#include "ext2_fs.h"

int fd_image, fd_csv;
struct ext2_super_block super;

void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, "%s", loc);
  fprintf(stderr, ": ");
  fprintf(stderr, "%s\n", strerror(err));

  exit(1);
}

void processArguments(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Requires an image file.\n");
    exit(1);
  }

  fd_image = open(argv[1], O_RDONLY);
  if (fd_image < 0) {
    handleError("processArguments (open)", errno);
  }
}

void analyzeSuper() {
  pread(fd_image, &super, sizeof(struct ext2_super_block), 1024);

  fprintf(stdout, "SUPERBLOCK,");
  fprintf(stdout, "%d,", super.s_blocks_count);
  fprintf(stdout, "%d,", super.s_inodes_count);
  fprintf(stdout, "%d,", 1024 << super.s_log_block_size);  // SPEC
  fprintf(stdout, "%d,", super.s_inode_size);
  fprintf(stdout, "%d,", super.s_blocks_per_group);
  fprintf(stdout, "%d,", super.s_inodes_per_group);
  fprintf(stdout, "%d\n", super.s_first_ino);



}

int main(int argc, char* argv[]) {
  processArguments(argc, argv);
  analyzeSuper();
}
