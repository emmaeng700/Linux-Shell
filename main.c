
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <direct.h>

/*
Basic liftime of a Shell
    1. Initialize: reads and executes shell configuratiom files.
    2. Interpret: shell reads commands from stdin and executes them.
    3. Terminate: shell executes any shutdown commands, frees up any memory, and terminates. 
*/

// Custom fork implementation to simulate UNIX fork, helping me to launch new processes
HANDLE custom_fork(const char* cmd) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Build command line
    char cmdLine[1024];
    sprintf(cmdLine, "%s", cmd);

    if (!CreateProcess(
        NULL,        // Application name
        cmdLine,     // Command line
        NULL,        // Process security
        NULL,        // Thread security
        FALSE,       // Inherit handles
        0,           // Creation flags
        NULL,        // Environment
        NULL,        // Current directory
        &si,         // STARTUPINFO
        &pi          // PROCESS_INFORMATION
    )) {
        fprintf(stderr, "CreateProcess failed. Error: %lu\n", GetLastError());
        return NULL;
    }

    // Optional: WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread); 
    return pi.hProcess;
}

/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int lsh_help(char **args)
{
  int i;
  printf("Felix Nuworsu's LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int lsh_exit(char **args)
{
  return 0;
}

// How shell starts processes
int lsh_launch(char **args) {
    HANDLE process_handle;
    int status;
    
    // Build complete command line with arguments
    char cmdLine[1024] = "";
    int i = 0;
    while (args[i] != NULL) {
        strcat(cmdLine, args[i]);
        strcat(cmdLine, " ");
        i++;
    }
    
    process_handle = custom_fork(cmdLine);
    
    if (process_handle == NULL) {
        // Error launching process
        fprintf(stderr, "lsh: failed to launch process\n");
        return 1;
    } else {
        // Parent process - wait for child to complete
        status = WaitForSingleObject(process_handle, INFINITE);
        CloseHandle(process_handle);
    }
    
    return 1;
}

int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

// Parsing a line
char **lsh_split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    
    while (token != NULL) {
        tokens[position] = token;
        position ++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);

    }
    tokens[position] = NULL;
    return tokens;
} 

