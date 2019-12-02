#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void question()
{
	puts("how are you?/g-good or b-bad");
	fflush(stdout);
	char ans;
	read(STDIN_FILENO,&ans,1);
	switch(ans){
		case'g':
			puts("\nGOOD\n");
		break;
		case'b':
			puts("\nBAD\n");
		break;
		default:
			puts("\nwrong answer\n");
				}
		}		

int main(){
	question();
	int fd;
	struct termios tty, savetty;
	fd = open("/dev/tty", O_RDWR);
	tcgetattr(fd, &tty);//get terminal options
	savetty = tty;
	tty.c_lflag&=~(ICANON|ISIG);//kananon input and allow signals
	tty.c_cc[VMIN]=1;
	tcsetattr(fd, TCSAFLUSH, &tty);// change terminal options
	question();
	tcsetattr(fd, TCSAFLUSH, &savetty);
	exit(EXIT_SUCCESS);
}
