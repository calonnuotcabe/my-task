#include <stdio.h>

int main() {
	int w;
	int n;
	n = scanf("%d", &w);
	getchar();
	if (n != 1) {
		return 0;
	}
	int check = w%2;
	if (w > 2 && check==0) {
		printf("Yes\n");
		return 0;
	}
	else {
		printf("No\n");
	}
	return 0;
}
