#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/************************
 * Print error and exit *
 ************************/

void errQuit(const char *errText)
{
	fprintf(stderr, "%s", errText);
	
	exit(EXIT_FAILURE);
}
