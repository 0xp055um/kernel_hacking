#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define LEN 32

#define IOC_MAGIC 'j'
#define IOC_GET _IO(IOC_MAGIC, 1)
#define IOC_SET _IO(IOC_MAGIC, 2)


int main(int argc, char **argv) {
  char buf[LEN] = {0};

  if (argc != 2) {
    printf("Use: %s \"/dev/hello_char\"\n", argv[0]);
    return -1;
  }

  int fd = open(argv[1], O_RDWR);
  if (fd < 0) {
    perror("Failed to open file");
    return -1;
  }

  printf("Lets write to the driver the word PASS\n");
  ioctl(fd, IOC_SET, "PASSWORD");
  printf("Lets see what we get\n");
  ioctl(fd, IOC_GET, buf);
  printf("We got: %s\n",buf);

  close(fd);
  return 0;
}
