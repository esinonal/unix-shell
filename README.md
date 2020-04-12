# unix-shell
This project was to create a shell, similar to the terminal shell. When
the user types in a command (in response to its prompt), the shell creates
a child process that executes the command  entered and then prompts for 
more user input when it has finished.

This is essentially an interactive loop: it repeatedly prints a prompt 
"myshell> ", parses the input, and executes the command specified on that
line of input, and waits for the command to finish.

The shell is able to parse a command, and run the program corresponding 
to the command. There is also inbuilt functionality for multiple commands, 
where they have to be separated by ";" semicolons. The UNIX shell supports 
redirection to a file with the ">" character; for example "ls -la /tmp > output"
would be sent to the file 'output.' The shell also supports advanced 
redirection, with the ">+" symbol, which appends the requested information 
to the end of the file.

The command to compile the myshell into a shell that can be run 
is the following: 
gcc -Wall -Werror -o myshell myshell.c

The coolest part of this project was seeing working with fork directly.
For example, we got to write pid = fork();, then analyze the two cases
of the parent and the child.

