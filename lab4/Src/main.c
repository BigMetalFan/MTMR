#include <stdint.h>
#include "main.h"
#include "stdio.h"

uint32_t clockPeriod = 0;

int main(void)
{
	
    clockPeriod = initRcc(USER_SOURCE);

	for(;;){

	}
}
