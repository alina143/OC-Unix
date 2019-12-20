
#define BUFFER_SIZE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
char *path = "./socket";

int
main(int argc, char *argv[])
{
   struct sockaddr_un name;
   int ret;
   int connection_socket;
   int data_socket;
   char buffer[BUFFER_SIZE];



   connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
   if (connection_socket == -1) {
	   perror("socket");
	   exit(EXIT_FAILURE);
   }


   memset(&name, 0, sizeof(struct sockaddr_un));

   name.sun_family = AF_UNIX;
   strncpy(name.sun_path, path, sizeof(name.sun_path) - 1);

   ret = bind(connection_socket, (const struct sockaddr *) &name,
			  sizeof(struct sockaddr_un));
   if (ret == -1) {
	   perror("bind");
	   exit(EXIT_FAILURE);
   }
   
   ret = listen(connection_socket, 20);
   if (ret == -1) {
	   perror("listen");
	   exit(EXIT_FAILURE);
   }


   for (;;) {

	   data_socket = accept(connection_socket, NULL, NULL);
	   if (data_socket == -1) {
		   perror("accept");
		   exit(EXIT_FAILURE);
	   }

	 
	while(1) {
		//connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		int nbytes=	0;																																						   
		while((nbytes = read(data_socket, buffer, sizeof(buffer)-1)) > 0){
			for (int i = 0; i < nbytes; i++)
				{
				buffer[i] = (char) toupper((unsigned char) buffer[i]);
				}

			if (write(STDOUT_FILENO, buffer, nbytes) == -1)
			{
				perror("client write");
				exit(EXIT_FAILURE);
			}
		}
		
	} 
	   close(data_socket);
   }

   close(connection_socket);

   exit(EXIT_SUCCESS);
}
