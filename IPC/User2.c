#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct area
{
    int rw; // Read-write flag: 0 - server writes, 1 - client writes
    char msg[100]; // Message buffer
};

struct area *shmptr;

int main()
{
    int shmid;
    
    // Create a shared memory segment
    shmid = shmget(700, sizeof(struct area), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    
    // Attach the shared memory segment to the process
    shmptr = (struct area *)shmat(shmid, NULL, 0);
    if (shmptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    
    // Initialize the shared memory structure
    shmptr->rw = 0; // Server starts first
    
    while (1)
    {
        // Wait until the client writes (rw = 1)
        while (shmptr->rw != 1)
            ;
        
        // Check if the message is "stop"
        if (strcmp(shmptr->msg, "stop") == 0)
        {
            printf("Server stopping...\n");
            break;
        }
        
        // Print the message received from the client
        printf("Received from client: %s\n", shmptr->msg);
        
        // Server writes a response
        printf("Server: ");
        fgets(shmptr->msg, 100, stdin);
        shmptr->msg[strcspn(shmptr->msg, "\n")] = '\0'; 
        
        // Change the flag to indicate the server has written
        shmptr->rw = 0;
    }
    
    // Detach and remove the shared memory segment
    shmdt((void *)shmptr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
