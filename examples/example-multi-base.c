#!cscript -s example-multi-ext.c
#include <stdio.h>

extern const char *external_string;

int main()  {
	printf("%s\n", external_string);
}
