#include <stdio.h>

int main(){
	int n;
	int a;
	int b;
	int c;
	scanf("%d", &n);
	int result = n;
	for(int i = 0; i < n; i++) {
		scanf("%d %d %d",&a, &b, &c );
		if ( (a == 0 && b == 0) || (b == 0 && c == 0) || (a == 0 && c == 0)) {
			result--;
		}
	}
	printf("%d\n", result);
	return 0;
}
