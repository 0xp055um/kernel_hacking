#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define len 13

int main() {
  char buf[len] = {0};
  int fd = open("/dev/hello_char", O_RDONLY);
  if (fd < 0) {
    perror("Failed to open file");
    return -1;
  }

  if (read(fd, buf, len) < 0) {
    perror("Failed to read file");
    close(fd);
    return -1;
  }

  puts("Succesfully red file");
  printf("Message: %s\n", buf);

  close(fd);
  return 0;
}
