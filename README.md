# 377-final-project

## Code Description 
<sub>mainly focusing on exec_command() function</sub>
### Piping commands
The first thing that was implemented was the piping commands (|). I declared a variable to keep track of the index of the pipe operator, if it existed. If it did exist (index != -1), I called pipe and fork and handled each process, duplicating the read and right ends and assigned them to the proper file descriptors. The child process executed the first set of commands and the parent the second. 

### Redirect STDOUT
Next, I handled the redirection of standard output (>). Similar to pipe, I kept track of the index of the redirection operator. If the index was not -1, meaning there was a redirection, I opened the file specified that would take in the output of the first set of commands and then executed the command.

### Builtin commands
If there was no pipe or redirection, I checked if it was a command I needed to handle separately. I created a separate function called cmd_handler(), which exec_command() calls, to handle functions outside of the regular Linux shell. This function handles 4 different commands: "pwd", "cd", "help", and "tim".
* pwd: This bash command prints out the current working directory. I did this by using the function getcwd(), which takes in a buffer and the size of a buffer and prints out the path to the buffer. 
* cd: This bash command changes the current working directory to the specified location. I did this by using the function chdir(), which changes the current working directory to the input. 
* help: This bash command provides information on the shell's builtin commands. I call the function openHelp() which simply puts a string detailing the shell's functionality and what all can be done. 
* tim: This command is a command I personally implemented that prints out "Tim is the best!" in cool ascii art. Just a silly command!
If this function returns 1, then it successfully found the command. If it returns 0, the command was not found. If the command was successfully found, then I exit.

### Linux shell general commands
If there was no pipe, redirection, and it was not a command I needed to handle separately, I used execvp to normally execute the command, using the argument given.

## Design Decisions
I decided to implement piping and redirection similarily  since they both required executing two sets of commands both before and after an operator. I chose to first check for piping and redirection first since they would need to be handled the soonest. When checking for builtin commands, it was simpler if I created a separate function cmd_handler() and passed in the argument (argv). This way I could also proceed depending on the output of that cmd_handler() and whether it was a success or a failure. For cd, instead of returning 1, I return 2. This ensures that the cd does not exit, since that would end the process that changes the directory. Instead it just returns. I had to change the main function code to kill instead of exit when quit is called so that all the process are ended, otherwise it would take multiple quit commands to terminate the shell. 

## Video Presentation
https://youtu.be/awz5JAm1FYo

## Resources Used
* https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
* https://www.educative.io/blog/bash-shell-command-cheat-sheet
* https://www.geeksforgeeks.org/making-linux-shell-c/
* https://stackoverflow.com/questions/26788603/simple-shell-with-pipe-function
* https://patorjk.com/software/taag/#p=display&f=Big&t=Tim%20is%20the%20best!
* https://man7.org/linux/man-pages/man2/getcwd.2.html
* https://stackoverflow.com/questions/11515399/implementing-shell-in-c-and-need-help-handling-input-output-redirection
* https://www.geeksforgeeks.org/pipe-system-call/
