#include <assert.h>
#include <string.h>

int main()
{
	setenv("MallocScribble", "1", 1);

	char *a = (char*)malloc(64);
	char *b = (char*)malloc(64);
	char *c = (char*)malloc(64);

	free(b);

	for (int i = 0; i < 64; ++i)
	{
		printf("%hhx ", (char)b[i]);
	}

	return (0);
}
