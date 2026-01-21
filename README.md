# LSH (Linux Shell) Documentation

LSH is a simple implementation of a shell in C, demonstrating the basics of how a shell works through the fundamental operations: read, parse, fork, exec, and wait. This Windows-compatible implementation provides a command-line interface similar to Unix/Linux shells while using Windows API functions for process management.

## Core Shell Functions

The shell operates in a continuous cycle:
1. **Read**: Capture user input from the command line
2. **Parse**: Break the input into commands and arguments
3. **Execute**: Run the command (either built-in or external)
4. **Wait**: Wait for completion before accepting new input

## Implementation Details

### Command Processing
- **Reading Input**: Uses custom `lsh_read_line()` function to allocate and read user input
- **Parsing**: `lsh_split_line()` tokenizes the input using space delimiters
- **Execution**: `lsh_execute()` checks for built-in commands or launches external processes

### Process Management
- **Windows Implementation**: Uses `CreateProcess()` instead of Unix `fork()`/`exec()`
- **Custom Fork**: `custom_fork()` function simulates fork-like behavior for Windows
- **Process Waiting**: Uses `WaitForSingleObject()` to wait for child processes to complete

### Built-In Commands
- **cd**: Change directory using `chdir()`
- **help**: Display help information about the shell
- **exit**: Terminate the shell

## Limitations
As a simple implementation, LSH has several limitations:
- No support for input/output redirection
- No implementation of pipes
- Limited error handling
- No command history or command editing
- Limited signal handling
- No job control
- No environment variable expansion

