#ifdef _WIN32
// Windows code
#include <conio.h>
#include <windows.h>
int mx_kbhit(void) { return _kbhit(); }
void mx_sleep(int milliseconds) { Sleep(milliseconds); }
// Windows code
#else
// Other code
#include <unistd.h> 
#include <sys/time.h> 
#include "port.h" 
int mx_kbhit(void) 
{ 
    struct timeval tv = { 0, 0 }; 
    fd_set readfds; 
  
    FD_ZERO(&readfds); 
    FD_SET(STDIN_FILENO, &readfds); 
  
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1; 
}

void mx_sleep(int milliseconds) { usleep(milliseconds * 1000); }
// Other code
#endif