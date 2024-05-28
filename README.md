# Simple-Shell

This project is a simple shell implementation written in C. The shell accepts user commands, executes them, and logs the executed commands in a file named log.txt.

## Features

Accepts user commands and arguments
Handles built-in commands like **__exit__**
Searches for executable files in the current directory and directories specified in the **__PATH__** environment variable
Logs executed commands with timestamps in the **__log.txt__** file

## Prerequisites

To run this project, you need to have a C compiler installed on your system. This project was developed and tested using the GNU Compiler Collection (GCC) on a Linux environment.

## Building and Running
To build and run the project, follow these steps:

1. Open a terminal and navigate to the project directory.
2. Compile the source code using the following command:

       gcc -o shell main.c
This command will create an executable file named shell.

3. Run the executable with the following command:

       ./shell

This will start the shell and display the prompt $.

## Usage
Once the shell is running, you can enter commands and arguments separated by spaces. 
For example:

    $ ls -l

To exit the sh Details
The shell implementation consists of the following functions:

**__log_command(const char *command):__** This function logs the executed command with a timestamp in the log.txt file.

**__execute_command(char *path, char **args):__** This function executes the command specified by the path and args arguments. It forks a child process and uses the execv system call to execute the command.

**__find_command_path(char *command):__** This function searches for the executable file associated with the given command. It checks if the command is a built-in command, has a path specified, is a shell variable, or is located in the current directory or directories specified in the PATH environment variable.

The main function is responsible for the main loop of the shell. It reads user input, splits the input into command and arguments, finds the command path, and executes the command if it is valid.
Notes

- The maximum length of a command is limited to **__MAX_COMMAND_LENGTH__** (512 characters).
- The shell assumes that the maximum number of arguments for a command is **__MAX_COMMAND_LENGTH / 2 + 1__**.
- The **__log.txt__** file will be created in the current working directory if it doesn't exist.
- The shell does not support advanced features like input/output redirection, piping, or background processes.

Feel free to explore and modify the code to add additional features or enhance the functionality as needed.
