#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void ask_question(){
puts("Are you ready?/n y or n");
fflush(stdout);
char ans;
read(STDIN_FILENO,&ans,1);
switch(ans){
case'y':
puts("\nGOOD\n");
break;
case'n':
puts("\nHmmm\n");
break;
default:
puts("\nwrong answer\n");
}
}

int main(){
ask_question();
int fd;
struct termios tty, savetty;
fd = open("/dev/tty", O_RDWR);
tcgetattr(fd, &tty);
savetty = tty;
tty.c_lflag&=~(ICANON|ISIG);
tty.c_cc[VMIN]=1;
tcsetattr(fd, TCSAFLUSH, &tty);
ask_question();
tcsetattr(fd, TCSAFLUSH, &savetty);
exit(EXIT_SUCCESS);
}