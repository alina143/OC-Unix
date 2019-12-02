#include <stdlib.h>
#include <libgen.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
void on_error(const char* message)
{
	printf("%s with code %d (%s), aborted.\n", message, errno, strerror(errno));
	exit(1);
}
typedef struct 
{
    int fd;
    size_t lines_count;
    size_t offset[100];
    size_t size[100];
} FileTable;

const int BUFFER_SIZE = 6;
FileTable* read_file(const char* name)
{
    FileTable* file = (FileTable*) malloc(sizeof(FileTable));

    if (!file)
    {
        return NULL;
    }
    
    file->fd = open(name, O_RDONLY);

    if (file->fd == -1)
    {
        free(file);
        return NULL;
    }

    file->lines_count = 1;
    file->offset[0] = 0;
    char buf[BUFFER_SIZE];
    size_t i = 0, line_offset = 0;
    int count_bytes = read(file->fd, buf, BUFFER_SIZE);

    while (count_bytes> 0)
    {  i=0;
        for (const char* ch = buf; ch < buf + BUFFER_SIZE; ch++)
        {
            i++;

            if (*ch == '\n')
            {
                file->size[file->lines_count-1] = i;
                line_offset += i;
                file->offset[file->lines_count] = line_offset;
                file->lines_count++;
   
            }
        }

        count_bytes = read(file->fd, buf, BUFFER_SIZE);
    }   

    if (count_bytes == -1)
    {
        close(file->fd);
        free(file);
        return NULL;
    }

    printf("File size: %zd lines\n", file->lines_count-1);
    return file;
}

int get_request()
{
    puts("Enter line number (0 for exit): ");
	struct pollfd fd;//member of struct
	fd.fd = 0; //input data
	fd.events=POLLIN;
    
    int code = poll(&fd,1,5000);
    int res;
    if (code==1)
    {
		if (scanf("%d", &res) == EOF)
		{
			return -1;
		}
		else
		{
			return res;
		}
    }
    else
    {
		return (code == 0)? -2 : -1;
	}
    
}
void print_all_file(FileTable* file){
	if (lseek(file->fd, 0, SEEK_SET) == -1)
    {
       if(close(file->fd)==-1){
				on_error("can't close file");
		}
			free(file); 
    }

    char buf[BUFFER_SIZE];
    int nbytes = read(file->fd, buf, BUFFER_SIZE);

    while (nbytes > 0)
    {
        write(STDIN_FILENO, buf, nbytes);  
        nbytes = read(file->fd, buf, BUFFER_SIZE);
    }

   
}
void write_lines(FileTable* file){
	  int string_number;

    do 
    {  
        string_number = get_request();

        if (string_number ==-1)
        {
            puts("Bad line number, aborted.\nERROR");
        }else if(string_number ==-2){
			puts("TIMEOUT");
			print_all_file(file);
			return;
		}
        else if (file->lines_count < string_number) 
        {
            puts("Line not found\nERROR");
            break;
        }
        else if (string_number > 0)
        {
            size_t len = file->size[string_number-1];

            for (size_t i = 0; i < len; i += BUFFER_SIZE)
            {
                char buf[BUFFER_SIZE];
                size_t chunk = (len - i > BUFFER_SIZE)? BUFFER_SIZE : len - i;  

                if (lseek(file->fd, file->offset[string_number-1] + i, SEEK_SET) == -1 || read(file->fd, buf, chunk) != chunk)
                {
                    if(close(file->fd)==-1){
						 on_error("can't close file");
					 }
                    free(file);
                    on_error("Reading error");
                }
                
                write(STDIN_FILENO, buf, chunk);
            }
            
            //puts("\n OK");
        }
    } 
    while (string_number>0);

}
int main()
{
	char* file_name ="test.txt";
    FileTable* file = read_file(file_name);

    if (!file)
    {
		on_error("Can't read the input file\n");
    }

	write_lines(file);
	if(close(file->fd)==-1){
		on_error("can't close file");
	}
	free(file); 
    return 0;
}
