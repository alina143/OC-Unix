#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *file;
	
	printf ("Real User ID = %d\n",getuid());
	printf ("Effective User ID = %d\n\n",geteuid());
	file = fopen("a.txt", "rw");
		if (file == NULL )
			perror("error");
		else 
			fclose(file);
	setuid(getuid());
	printf ("Real User ID = %d\n",getuid());
	printf ("Effective User ID = %d\n\n",geteuid());
	file = fopen("a.txt", "rw");
		if (file == NULL )
			perror("error");
		else 
			fclose(file);
	return 0;
}

