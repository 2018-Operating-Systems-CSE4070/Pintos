#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main(int argc, char **argv)
{
	int i, sum = 0;
	for(i = 1; i < argc; i++)
	{
		int x = atoi(argv[i]);
		if(i == 1) printf("%d ", fibonacci(x));
		sum += x;
	}
	printf("%d\n", sum);
	return EXIT_SUCCESS;
}
