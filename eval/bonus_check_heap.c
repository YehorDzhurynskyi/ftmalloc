#include <assert.h>
#include <string.h>

int main()
{
	{
		//setenv("MallocCheckHeapRelaxed", "1", 1);
		char *a = (char*)malloc(64);
		char *b = (char*)malloc(64);
		char *c = (char*)malloc(64);

		free(b);

		*((size_t*)(b)) = -1;
		*((size_t*)(b + sizeof(size_t))) = -1;

		free(a);
	}

	{
		//setenv("MallocCheckHeapFully", "1", 1);
		char *a = (char*)malloc(64);
		*((size_t*)(a - sizeof(size_t))) = -1;

		free(a);
	}

	return (0);
}
