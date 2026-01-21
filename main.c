
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