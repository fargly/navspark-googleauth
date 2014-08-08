#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "port.h"
#include "rawmode.h"
#include "calc.h"

/*************
 * Main loop *
 *************/

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage : %s <key>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
#ifndef _WIN32
  mode_raw(1);
#endif

  int epoch = 0;
  int epoch_new = 0;

  while(1)
  {
    if(mx_kbhit() != 0)
      break;
    
	epoch_new = (int)time(NULL) / 30;
	
	if (epoch_new != epoch)
		printf("\rCode  : %06d\r\n", calculate(argv[1], epoch_new));
	
	epoch = epoch_new;
	
	printf("\r%02d", 30 - (int)time(NULL) % 30);
	fflush(stdout);
	mx_sleep(100);
  }
    
  mode_raw(0);

  printf("\n");

  return EXIT_SUCCESS;
}
