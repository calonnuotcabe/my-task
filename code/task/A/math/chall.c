#include <stdio.h>
#include <string.h>
int main() {
	char a[101];
	scanf("%100s",a);
	int len = strlen(a);
	int var = 0;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			if (a[i] == '+') {
				i++;
				if (a[j] == '+') {
					j++;
				}
			}
			if (a[i] < a[j]) {
				var = a[i];
				a[i] = a[j];
				a[j] = var;
			}
		}
	}
	printf("%s\n", a);
	return 0;
}
