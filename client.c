/* Filename: fifoclient_twoway.c */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "/tmp/fifo_twoway"
int main() {
   int fd;
   
   fd = open(FIFO_FILE, O_CREAT|O_RDWR);
   
    if(fd == -1){
    return 1;
    }

    
    int x = 1;

    
    write(fd, &x, sizeof(int));
    printf("Sent command: %d\n", x);

    int userID = 0;

    read(fd, &userID, sizeof(int));
    printf("Received UserID: %d\n", userID);


   return 0;
}
