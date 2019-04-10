#include "sys.h"
#include "tasks.h"

int main(void)
{
	InitTask();

	while (1)
	{
		HandleLowPirorityTask();
	}
}
