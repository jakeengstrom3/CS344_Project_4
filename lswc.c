#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Wrong number of arguments\n",
                "Call with lswc directory");
    }

    int pfd[2];   // Pipe file descriptors, write and read
    pipe(pfd);    // Create a pipe
    int pid = fork();

    if(pid == 0){
        //Child process
        dup2(pfd[0], 0); //Duplicates pfd[0] (Read/input end of pipe) into file descriptor 0
        close(pfd[1]); //Close write end of pipe (we dont need it)
        execlp("wc", "wc", "-l", (char *) 0); //Execute command 
        exit(0);
    }
    dup2(pfd[1], 1); //Duplicate write end of pipe to stdout
    close(pfd[0]); //Close read end of pipe
    execlp("ls", "ls", "-al", argv[1], (char *) 0);
}