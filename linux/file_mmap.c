#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#define LENGTH (1024*1024*1024)

int main()
{
  int fd, i;
  char *text, *ptr;
  fd = open ("linux_large.txt", O_RDWR);
  if(fd == -1)
    {
      perror("open");
      return 1;
    }

  ptr = text = mmap (0, LENGTH, PROT_READ | PROT_WRITE,
	       MAP_SHARED, fd, 0);

  if (text == MAP_FAILED)
  {
    perror("map");
    close(fd);
    return 1;
  }

  printf("%.*s", 1024, ptr);

  for(i = 1024; i < 2048; ++i)
    {
      ptr[i] = '0';
    }

  if (msync(ptr, 2048, MS_SYNC) == -1)
    perror("msync");

  if (munmap (text, LENGTH) == -1)
    perror("unmap");

  if (close(fd) == -1)
    perror("unmap");

  return 1;
} 

