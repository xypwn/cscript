#!cscript -s
#include <stdio.h>

int main(int argc, char *argv[])  {
	printf("Program name: %s\n", argv[0]);

	if (argc <= 1) {
		printf("No arguments given\n");
	} else {
		for (size_t i = 1; i < argc; i++) {
			printf("Argument %zu: %s\n", i, argv[i]);
		}
	}
}
