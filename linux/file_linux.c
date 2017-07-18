#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
  char *text = "Mr. and Mrs. Dursley, of number four, Privet Drive, were proud to say that they were perfectly normal, thank you very much. They were the last people you'd expect to be involved in anything strange or mysterious, because they just didn't hold with such nonsense.\n";
  char *start = text;
  const unsigned int BUF_SIZE = 20;
  char buf[BUF_SIZE];
  int fd, fd2, len;
  ssize_t ret;
  fd = open("linux_text.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  if(fd == -1)
    {
      perror("open");
      return 1;
    }

  // Writing
  len = strlen(text);
  // A loop is not necessary, but use it anyway
  while(len != 0 && (ret = write(fd, text, len)) != 0)
    {
      if(ret == -1)
	{
	  if (errno == EINTR)
	    {
	      continue;
	    }
	  perror("write");
	  break;
	}
      len -= ret;
      text += ret;
    }

  if (close (fd) == -1)
    {
      perror("close");
      return 1;
    }

  fd2 = open("linux_text.txt", O_RDONLY);
  if(fd2 == -1)
    {
      perror("open");
      return 1;
    }

  len = strlen(start);
  while(len != 0 && (ret = read (fd2, buf, BUF_SIZE)) != 0)
    {
      if(ret == -1)
	{
	  if(errno == EINTR)
	    continue;
	  perror("read");
	  break;
	}
      len -= ret;
      printf("%.*s#", (int)ret, buf);
      lseek(fd2, 2, SEEK_CUR);
    }
  printf("\n");
  if (close(fd2) == -1)
    {
      perror("close");
      return 1;
    }
  unlink("linux_text.txt");
  return 0;
}


