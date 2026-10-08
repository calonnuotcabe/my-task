#include <stdio.h>

int main() {
	int k, n;
	scanf("%d %d",&n, &k);
	char a[n+1];
	char b[n+1];
	int check = 0;
	int max = 0;
	int result = 0;

	for (int i = 0; i < n; i++) {
		scanf("%hhd", &a[i]);
		getchar();
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++) {
			if (max < a[j]) {
				max = a[j];
			        check = j;	
			}
		}
		b[i] = max;
		a[check] = 0;
		max = 0;
		check = 0;
	}

	for (int i = 0; i < n; i++) {
		if (b[i] >= b[k-1] && b[i] > 0) {
			result++;
		}
	}

	printf("%d\n", result);
	return 0;
}
