#include <stdint.h>
#include "hardware_init.h"
#include "software_init.h"

static inline void System_Init()
{
	System_Hardware_Init();
	System_Software_Init();
}
