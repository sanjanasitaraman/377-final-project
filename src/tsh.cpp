#include <tsh.h>
#include <iostream>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <fstream>

using namespace std;


void simple_shell::parse_command(char* cmd, char** cmdTokens) {
  // TODO: tokenize the command string into arguments
  char *token = strtok(cmd, " \n");
  int i = 0;
  while(token != NULL){
    cmdTokens[i] =  token;
    token = strtok(NULL, " \n");
    i = i + 1;
  }
  cmdTokens[i] = NULL;
}

void simple_shell::exec_command(char** argv) {
  int pipe_idx = -1; //pipe index
  int redirect_idx = -1; //redirect index
  int i = 0;
  int pid = fork();
  if(pid == -1){
    perror("fork failed");
    exit(0);
  }
  else if (pid == 0){
    while(argv[i] != NULL){ //get index of pipe
      if(strcmp(argv[i], "|") == 0){
        pipe_idx = i; 
        break;
      }
      else if(strcmp(argv[i], ">") == 0){ //get index of redirect
        redirect_idx = i;
        break;
      }
      i++;
    }

    if(pipe_idx != -1){ //if pipe exists
      argv[pipe_idx] = NULL; 
      int fd[2]; //declare file descriptor
      if(pipe(fd) == -1){ //if pipe doesnt work
        perror("pipe failed");
        exit(0);
      } 
      int pid2 = fork();
      if(pid2 == -1){
        perror("fork failed");
        exit(0);
      }
      else if(pid2 == 0){// child process
        close(fd[0]); 
        dup2(fd[1], STDOUT_FILENO); 
        close(fd[1]);
        execvp(argv[0], argv);
        perror("execvp for command 1 failed");
        exit(0);
      }
      else{
        close(fd[1]); //parent process
        dup2(fd[0], STDIN_FILENO); 
        close(fd[0]);
        char** argv2 = argv + pipe_idx + 1; //stores commands after pipe
        execvp(argv2[0], argv2);
        perror("execvp for command 2 failed");
        exit(0);
      }
    }

    else if (redirect_idx != -1) { // if redirection exists
      argv[redirect_idx] = NULL;
      char* filename = argv[redirect_idx + 1]; // get the filename after '>'
      int fd = open(filename, O_WRONLY);
      if (fd == -1) {
          perror("open failed");
          exit(0);
      }
      dup2(fd, STDOUT_FILENO); // redirect stdout to file
      close(fd);
      execvp(argv[0], argv);
      perror("execvp failed");
      exit(0);
    }

    else { //not a pipe or redirect
      int output = cmd_handler(argv);
      if (output != 0) { // check for builtin commands
        if(output == 2){
          return;
        }
        exit(0);
      } //handle general commands
      execvp(argv[0], argv);
      perror("execvp failed");
      exit(0);
    }
  }
  else{
    wait(NULL);
  }
}


int simple_shell::cmd_handler(char** argv) {
    int num_cmds = 4; //num of builtin commands 
    int cmd_idx = 0; 
    const char* cmds[num_cmds]; //array to store builtin commands

    cmds[0] = "pwd";
    cmds[1] = "cd";
    cmds[2] = "help";
    cmds[3] = "tim";
    
    for (int i = 0; i < num_cmds; i++) { //finds index of command in array
      if (strcmp(argv[0], cmds[i]) == 0) {
        cmd_idx = i + 1;
        break;
      }
    }
    switch (cmd_idx) { 
      case 1:  //pwd
        char cwd[PATH_MAX];
        getcwd(cwd,sizeof(cwd)); 
        std::cout << cwd << std::endl; //print out buffer                                                                           
        return 1;
      case 2: //cd
        chdir(argv[1]);
        return 2;
      case 3: //help
        openHelp();
        return 1;
      case 4: 
        printf("  _______ _             _       _   _            _               _   _ \n");
        printf(" |__   __(_)           (_)     | | | |          | |             | | | |\n");
        printf("    | |   _ _ __ ___    _ ___  | |_| |__   ___  | |__   ___  ___| |_| |\n");
        printf("    | |  | | '_ ` _ \\  | / __| | __| '_ \\ / _ \\ | '_ \\ / _ \\/ __| __| |\n");
        printf("    | |  | | | | | | | | \\__ \\ | |_| | | |  __/ | |_) |  __/\\__ \\ |_|_|\n");
        printf("    |_|  |_|_| |_| |_| |_|___/  \\__|_| |_|\\___| |_.__/ \\___||___/\\__(_)\n\n");
        return 1;
      default:
        break;
    }
    return 0;
}


void simple_shell::openHelp(){
  puts("\n***WELCOME TO SANJANA'S SHELL HELP***"
    "\nCopyright @ Sanjana Sitaraman"
    "\nList of Commands supported:"
    "\n>>> general commands available in UNIX shell"
    "\n>>> pipe handling"
    "\n>>> redirection handling"
    "\n>>> pwd: prints current working directory"
    "\n>>> cd: changes the current working directory to specified location"
    "\n>>> help: information on shell builtin commands"
    "\n>>> tim: a special command- try it out to find out what it does!");
    return;
}
    

bool simple_shell::isQuit(char* cmd) {
    // TODO: check for the command "quit" that terminates the shell
  if(strcmp(cmd, "quit") == 0){
    return true;
  }
  return false;
}
