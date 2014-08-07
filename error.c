#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "rawmode.h"

/************************
 * Print error and exit *
 ************************/

void errQuit(const char *errText)
{
	fprintf(stderr, "%s", errText);
	
	mode_raw(0);
	exit(EXIT_FAILURE);
}
