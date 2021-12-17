#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 4096
#define MAX_WORD 1024

enum {OUTSIDE, INSIDE};
int main()
{
	char buf[MAX_LINE], *temp;
	int  i, in_word = OUTSIDE;
	char str[MAX_WORD];

	/* read a line from stdin, break it into words,
	   and print out each word at one line */
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		for (temp = buf; *temp; temp++) {
			if (isalpha(*temp)) {
				if (in_word == OUTSIDE) {
					in_word = INSIDE;
					i = 0;
				}
				str[i++] = *temp;
			} else {
				if (in_word == INSIDE) {
					str[i] = 0;
					printf("%s\n", str);
				}
				in_word = OUTSIDE;
			}
		}
	}
	if (in_word == INSIDE) {
		str[i] = 0;
		printf("%s\n", str);
	}

	return 0;
}
