#include "gettime.h"

#include <time.h>

// Works only for hardware with clock
int gettime_30()
{
	return time(NULL) / 30;
}
