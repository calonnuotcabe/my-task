#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
	char a[101];
	char b[101];
	scanf("%100s", a);
	scanf("%100s", b);
        int len = strlen(a);	
	for (int i = 0; i < len; i++) {
		a[i] = tolower(a[i]);
		b[i] = tolower(b[i]);
	}
	int cmp = strcmp(a, b);
	if (cmp == 0) {
		printf("0\n");
	}
	if (cmp < 0) {
		printf("-1\n");
	}
	if (cmp > 0) {
                printf("1\n");
        }
	return 0;

	
}
