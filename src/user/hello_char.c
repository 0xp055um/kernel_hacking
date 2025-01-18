#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LEN 32
#define DEV "/dev/hello_char"

int main() {
  char buf[LEN] = {0};
  int fd = open(DEV, O_RDWR);
  if (fd < 0) {
    perror("Failed to open file");
    return -1;
  }

  // Lets Read from the Kernel
  if (read(fd, buf, LEN) < 0) {
    perror("Failed to read file");
    close(fd);
    return -1;
  }

  puts("Succesfully red file");
  printf("Message: %s\n", buf);

  puts("Now Lets try writing");
  strcpy(buf, "Hello from Userspace!\n");

  // Lets Write to the Kernel
  if (write(fd, buf, LEN) < 0) {
    perror("Failed to write to file");
    close(fd);
    return -1;
  }

  close(fd);
  return 0;
}
