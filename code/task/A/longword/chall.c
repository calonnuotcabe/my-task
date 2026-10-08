#include <stdio.h>
#include <string.h>

int main() {
	char buf[101];
	int n;
	scanf("%d", &n);
	if (n < 0 || n > 100) {
		return 1;
	}
	for (int i = 0; i < n; i++) {
		scanf("%100s", buf);
		int len = strlen(buf);
		if (len > 10) {
			printf("%c%d%c\n",buf[0], len-2,buf[len-1]);
		}
		else {
			printf("%s\n", buf);
	        }
        }
	return 0;
}
