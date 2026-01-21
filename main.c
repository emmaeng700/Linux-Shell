
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