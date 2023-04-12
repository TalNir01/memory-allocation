#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char * test = (char *)malloc(sizeof(char) * 6);
    strcpy(test, "AAAAAA");
    printf("addr = (%p) MemAlloc: (%s) strlen(%ld)\n", test, test, strlen(test));

    free(test);
    return 0;
}
