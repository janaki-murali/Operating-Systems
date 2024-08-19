#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct area
{
    int server_up;
    int client_up;
    int rw; // 0 - server write, 1 - client write
    char msg[100];
};

struct area *shmptr;

int main()
{
    int shmid;
    
    // Create the shared memory segment
    shmid = shmget(700, sizeof(struct area), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    
    // Attach the shared memory segment
    shmptr = (struct area *)shmat(shmid, NULL, 0);
    if (shmptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    
    // Initialize the shared memory structure
    shmptr->server_up = 1;
    shmptr->client_up = 0;
    shmptr->rw = 0;

    while (1)
    {
        // Wait for the client to write (client_up == 1)
        while (shmptr->client_up == 0);

        // Read the message from client
        printf("Received from user1: %s\n", shmptr->msg);

        // Check if the message is "stop"
        if (strcmp(shmptr->msg, "stop") == 0)
        {
            printf("Server stopping...\n");
            shmctl(shmid, IPC_RMID, NULL); // Remove the shared memory segment
            exit(0);
        }
        
        // Signal that server is ready to write
        shmptr->server_up = 1;
        shmptr->client_up = 0;

        // Get input from the server (user2)
        printf("user2: ");
        fgets(shmptr->msg, 100, stdin);
        shmptr->msg[strcspn(shmptr->msg, "\n")] = '\0'; // Remove newline character

        // Check if the message is "stop"
        if (strcmp(shmptr->msg, "stop") == 0)
        {
            shmctl(shmid, IPC_RMID, NULL); // Remove the shared memory segment
            exit(0);
        }

        // Signal that client can now read (client_up == 1)
        shmptr->rw = 1;
        shmptr->server_up = 0;
        shmptr->client_up = 1;

        // Wait for the client to process (rw == 0)
        while (shmptr->rw == 1);
    }

    return 0;
}
