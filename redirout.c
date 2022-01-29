#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  const int BUFFER_SIZE = 2048;

  if(argc < 3){
    printf("Too few arguments. Usage: redirout (destination file) (command)\n");
    exit(0);
  }

  // Open file specified by second argument. Give it only write permissions
  int redirected_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
  ftruncate(redirected_fd, 0); //Deletes any contents in the file

  int pfd[2];
  pipe(pfd);

  int pid = fork();

  if(pid ==0){
    // Child Process
    dup2(pfd[1], 1); // Makes stdout redirect to write end of pipe
    close(pfd[0]); // Close read end of pipe (we dont need it)
    execvp(argv[2], argv + 2); //Run the command with the 3rd and subsequent command line arguments
    exit(0);
  }
  close(pfd[1]); // Close write end of pipe
  char *buffer[BUFFER_SIZE]; // Create a buffer to store bytes between the read end of pipe and the new file
  int n;
  while(read(pfd[0], buffer, BUFFER_SIZE) > 0){ // While reading from the pipe returns more than 1 byte
    write(redirected_fd, buffer, BUFFER_SIZE);  // Add the stored bytes to the new file
  }

  return 0;
}