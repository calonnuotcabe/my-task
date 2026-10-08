#include <stdio.h>
#include <string.h>
 
int main() {
        char a[130];
	char b[101];
        scanf("%100s", a);
        int len = strlen(a);
	int encounter = 0;
        int result = 0;
        for (int i = 0; i < len; i++){
		for (int j = 1+i; j < len; j++) {
			if (a[i] == a[j] && (j < (len-1))) {
				a[j] = 0;
			}
			if (j == len-1 && a[i] == a[len-1]) {
				a[j] = 0;
			}
		
		}
        
	}
	for (int i = 0; i < len; i++){
		if (a[i] >= 97 && a[i] <= 122 ) {
			result++;
		}
	}
	//printf("%d\n", result);
        if (result % 2 == 0) {
                printf("CHAT WITH HER!\n");
        }
        else {
                printf("IGNORE HIM!\n");
        }
        return 0;
}
