/*Autor: 
Farah Alhasan 
Jelsin Stiben Sanchez Almanza*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>

#define MAX_COMMAND_LENGTH 512

#define LOG_FILE "log.txt"

// Function to log the executed commands into a file
void log_command(const char *command) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    struct timeval time;
    gettimeofday(&time, NULL);
    fprintf(log_file, "%ld\t%s\n", time.tv_sec, command);
    fclose(log_file); // Close the log file
}


void execute_command(char *path, char **args) {
  pid_t pid = fork();
  if (pid == 0) {
    // Child process
    execv(path, args);
    perror(path); // Print error message if execv fails
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Fork failed
    perror("fork");
    exit(EXIT_FAILURE);
  } else {
    // Parent process
    waitpid(pid, NULL, 0);
  }
}


char *find_command_path(char *command) {
    char *path = NULL;
    char cwd[PATH_MAX]; // Buffer to store the current working directory

    // Check if the command is a built-in command like "exit"
    if (strcmp(command, "exit") == 0) {
        return strdup(command); // Return a duplicate of the command string
    }

    // Check if the command has a path specified (e.g., /usr/bin/command)
    if (strchr(command, '/') != NULL) {
        return strdup(command);
    }

    // Check if the command is a shell variable (e.g., $HOME)
    if (command[0] == '$') {
        return strdup(command);
    }

    // Check if the command exists in the current directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char full_path[PATH_MAX + MAX_COMMAND_LENGTH + 1];
        snprintf(full_path, sizeof(full_path), "%s/%s", cwd, command); 
        if (access(full_path, X_OK) == 0) { 
            return strdup(full_path); 
        }
    }


    char *path_env = getenv("PATH"); // Get the value of the PATH environment variable
    if (path_env != NULL) {
        char *path_env_copy = strdup(path_env); 
        char *token = strtok(path_env_copy, ":"); // Tokenize the PATH variable using ':' as delimiter
        while (token != NULL) {
            char full_path[PATH_MAX + MAX_COMMAND_LENGTH + 1];
            snprintf(full_path, sizeof(full_path), "%s/%s", token, command); // Create the full path
            if (access(full_path, X_OK) == 0) {
                free(path_env_copy); 
                return strdup(full_path); 
            }
            token = strtok(NULL, ":"); // Move to the next token
        }
        free(path_env_copy); 
    }


    path = strdup("");
    return path;
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_COMMAND_LENGTH / 2 + 1]; // Array to store command and arguments

    printf("$");
    fflush(stdout);

    while (fgets(command, MAX_COMMAND_LENGTH, stdin)) {
        command[strcspn(command, "\n")] = 0; // Remove newline character

        if (strlen(command) == 0) {
            // Handle empty input
            printf("$");
            fflush(stdout);
            continue;
        }

        log_command(command);

        // Split the command into arguments
        int i = 0;
        char *token = strtok(command, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the array

        // Check if the command is "exit"
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        char *path = find_command_path(args[0]);
        if (*path == '\0') {
            printf("%s: command not found\n", args[0]);
        } else {
            execute_command(path, args);
            free(path);
        }

        printf("$$");
        fflush(stdout);
    }

    return 0;
}
