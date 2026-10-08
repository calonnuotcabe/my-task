#include <stdio.h> 

int main() {
	int n;
	char buf[4];
	scanf("%d", &n);
	getchar();
	int c;
	int result = 0;
	for (int i = 0; i < n; i++) {
		scanf("%4s", buf);
	        getchar();
		c = buf[1];
		if (c == '+') {
			result++;
		}
		if (c == '-') {
			result--;
		}
	}
	printf("%d\n", result);
	return 0;
}
