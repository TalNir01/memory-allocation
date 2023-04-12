#include "MyMalloc.h"


int main(int argc, char const *argv[])
{
	char *pointer1 = NULL;
	char *pointer2 = NULL;
	char *strPointer = NULL;
	char *pointer3 = NULL;


	strPointer = (char *)talloc(sizeof(char) * 4);
	pointer1 = (char *)talloc(sizeof(char) * 4);



	strcpy(strPointer, "AAA");	
	strcpy(pointer1, "BBB");

	printf("addr = (%p) MemAlloc: (%s) strlen(%ld)\n", strPointer, strPointer, strlen(strPointer));
	printf("addr = (%p) MemAlloc: (%s) strlen(%ld)\n", pointer1, pointer1, strlen(pointer1));

	tFree(strPointer);
	tFree(pointer1);

	pointer2 = (char *)talloc(sizeof(char) * 6);
	strcpy(pointer2, "CCCCCC");
	printf("addr = (%p) MemAlloc: (%s) strlen(%ld)\n", pointer2, pointer2, strlen(pointer2));

	pointer3 = (char *)talloc(sizeof(char) * 2);
	strcpy(pointer3, "$$");
	printf("addr = (%p) MemAlloc: (%s) strlen(%ld)\n", pointer3, pointer3, strlen(pointer3));



	tFree(pointer2);
	tFree(pointer3);

	return 0;
}
