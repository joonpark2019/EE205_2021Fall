#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>

#define MAXLENGTH 1024
#define TRUE    1
#define FALSE   0

//you need to pass a num

int main(const int argc, const char *argv[])
{
    printf("%c", (('9' - '0') / 3) + '0');
    return 0;
}