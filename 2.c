#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>

const char* timezone_name = "America/Los_Angeles";



int main()
{

    setenv("TZ", timezone_name, 1);
    tzset();
    time_t seconds = time(NULL);
    struct tm* time_object = localtime(&seconds);
    char buffer[80];
    strftime (buffer, 80, "%Z %H:%M", time_object);
    printf("%s\n", buffer);
    exit(0);

 
}
