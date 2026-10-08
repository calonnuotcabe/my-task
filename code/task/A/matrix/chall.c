#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	int a[30];
	int step = 0;
	for (int i = 0; i < 25; i++){
		scanf("%d", &a[i+1]);
		getchar();
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++){
			if (a[i*5 + (j +1)] == 1 ) {
				if ((i + 1) != 3 || (j+1) != 3) {
					step = abs(2 - i) + abs(2 - j);
					//printf("%d%d\n", i+1, j+1);
				}
			}
		}
	}	
	printf("%d\n", step);
	return 0;
}
