#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <math.h>

#define MAX_SOCKS 30
#define MAX(X, Y) ((X) < (Y) ? (Y) : (X))

int main()
{
  int sockets[MAX_SOCKS];
  int no_sockets = 0;
  int msock, fd_max, i;
  size_t size;
  struct sockaddr_in master_addr, clientname;
  fd_set active_fd_set, read_fd_set;
  msock = socket (AF_INET, SOCK_STREAM, 0);
  memset(sockets, 0, sizeof(int)*30);
  if(msock == -1)
    {
      perror("socket");
      return 1;
    }

  memset(&master_addr, 0, sizeof(struct sockaddr_in));
  master_addr.sin_port = htons( 8888);
  master_addr.sin_family = AF_INET;
  master_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(msock, (struct sockaddr *)&master_addr, sizeof(master_addr)) < 0)
    {
      perror("bind");
      return 1;
    }

  FD_ZERO (&active_fd_set);
  FD_SET (msock, &active_fd_set);
  fd_max = msock + 1;

  if (listen (msock, 5) < 0)
    {
      perror("listen");
      return 1;
    }

  while(1)
    {
      read_fd_set = active_fd_set;
      if (select(fd_max, &read_fd_set, NULL, NULL, NULL) < 0)
	{
	  perror("select");
	  return 1;
	}

      for(i = 0; i < fd_max; ++i)
	{
	  if (FD_ISSET (i, &read_fd_set))
	    {
	      if(i == msock)
		{
		  int new;
		  size = sizeof(clientname);
		  new = accept (msock, (struct sockaddr *) &clientname,(socklent *) &size);
		  if (no_sockets >= MAX_SOCKS)
		    {
		      close(new);
		    }
		  else if(new < 0)
		    {
		      perror ("accept");
		      return 1;
		    }
		  else
		    {
		      int j;
		      for(j = 0; j < MAX_SOCKS; ++j)
			{
			  if(sockets[j] == 0)
			    {
			      sockets[j] = new;
			      break;
			    }
			}
		      no_sockets++;
		      fd_max = MAX(fd_max, new + 1);
		      FD_SET (new, &active_fd_set);
		    }
		}
	      else
		{
		  
		}
	    }
	}
    }

  close(msock);
  
  return 0;
}
