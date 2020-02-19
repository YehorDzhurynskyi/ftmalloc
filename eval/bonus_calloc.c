#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *a = (char*)calloc(1, 64);

	for (int i = 0; i < 64; ++i)
	{
		printf("%hhx ", (char)a[i]);
	}

	free(a);

	return (0);
}
