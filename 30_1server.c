 #define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy.socket"
#define BUFFER_SIZE 12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.h"

int
main(int argc, char *argv[])
{
   struct sockaddr_un name;
   int down_flag = 0;
   int ret;
   int connection_socket;
   int data_socket;
   int result;
   char buffer[BUFFER_SIZE];

   unlink(SOCKET_NAME);


   connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
   if (connection_socket == -1) {
	   perror("socket");
	   exit(EXIT_FAILURE);
   }


   memset(&name, 0, sizeof(struct sockaddr_un));

   name.sun_family = AF_UNIX;
   strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

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

	   result = 0;
	   for (;;) {


		   ret = read(data_socket, buffer, BUFFER_SIZE);
		   if (ret == -1) {
			   perror("read");
			   exit(EXIT_FAILURE);
		   }


		   buffer[BUFFER_SIZE - 1] = 0;

		   if (!strncmp(buffer, "DOWN", BUFFER_SIZE)) {
			   down_flag = 1;
			   break;
		   }

		   if (!strncmp(buffer, "END", BUFFER_SIZE)) {
			   break;
		   }


		   result += atoi(buffer);
	   }

	   sprintf(buffer, "%d", result);
	   ret = write(data_socket, buffer, BUFFER_SIZE);
	   if (ret == -1) {
		   perror("write");
		   exit(EXIT_FAILURE);
	   }

	   close(data_socket);


	   if (down_flag) {
		   break;
	   }
   }

   close(connection_socket);

   unlink(SOCKET_NAME);

   exit(EXIT_SUCCESS);
}
