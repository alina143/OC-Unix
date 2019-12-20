#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

char *path = "./socket";


int main(int argc, char** argv)
{    

	struct sockaddr_un addr;
	char buf[100];
	int fd, count;
	
	if (argc > 1) path=argv[1];

     fd = socket(AF_UNIX, SOCK_STREAM, 0);
     if (fd == -1)
     {
		 perror ("socket error");
		 exit(-1);
	 }
	memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
   
	 strncpy(addr.sun_path, path, sizeof(addr.sun_path)-1);
    
     if (connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un))<0) {
         perror ("connect error");
		 exit(-1);
     }
     
    
			 while ((count=read(STDIN_FILENO, buf, sizeof(buf)))>0)
			 {
				 if (write(fd, buf, count) != count){
					 if (count > 0 ) fprintf (stderr, "partial write");
					 else {
						 perror ("write error");
						 exit (-1);
						 }
				}
			 }
		
      
    
 }
