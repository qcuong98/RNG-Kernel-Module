#include<stdio.h>
#include<errno.h>  
#include<fcntl.h>   // Required for O_RDONLY
#include <unistd.h> // Required for read

int main(){
    int fd;
    printf("Starting device test code example...\n");
    fd = open("/dev/randomness_generator", O_RDONLY);             // Open the device with read only access
    if (fd < 0) {
        perror("Failed to open the device...");
        return errno;
    }

    int n;
    read(fd, &n, sizeof(int));
    printf("Random number generated: %d\n", n);

    printf("End of the program\n");
    return 0;
}
