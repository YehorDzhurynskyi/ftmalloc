#include <assert.h>
#include <string.h>

void show_alloc_mem_ex();

int main()
{
	char *a = (char*)malloc(64);
	strcpy(a, "HELLO WORLD");
	char *b = (char*)malloc(64);

	show_alloc_mem_ex();

	free(a);
	free(b);

	return (0);
}
