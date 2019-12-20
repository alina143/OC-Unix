#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>
int main(int argc, char *argv[]) {

	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	char sendBuff[1025];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, '0', sizeof(serv_addr));

	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	serv_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 10);
 
	while(1) {
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		int nbytes=	0;																																						   
		while((nbytes = read(connfd, sendBuff, sizeof(sendBuff)-1)) > 0){
			for (int i = 0; i < nbytes; i++)
        {
            sendBuff[i] = (char) toupper((unsigned char) sendBuff[i]);
        }

        if (write(STDOUT_FILENO, sendBuff, nbytes) == -1)
        {
            perror("client write");
            exit(EXIT_FAILURE);
        }
		}

		close(connfd);
		sleep(1);}
	
}
