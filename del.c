#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s [module name]\n", argv[0]);
		exit(0);
	}

	long ret = delete_module(argv[1]);
	printf("Delete module [%s] %s\n", argv[1], (!ret ? "success" : "fail"));
	return 0;
}
