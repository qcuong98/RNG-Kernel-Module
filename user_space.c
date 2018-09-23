/**
 * @file   testebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/ebbchar.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main(){
    int ret, fd;
    char stringToSend[BUFFER_LENGTH];
    printf("Starting device test code example...\n");
    fd = open("/dev/randomness_generator", O_RDONLY);             // Open the device with read only access
    if (fd < 0){
        perror("Failed to open the device...");
        return errno;
    }

    int n;
    read(fd, &n, sizeof(int));
    printf("Random number generated: %d\n", n);

    printf("End of the program\n");
    return 0;
}
