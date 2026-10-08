#include <stdio.h>

int main() {
	int input;
	int var;
	scanf("%d", &input);
	if (input < 6) {
		printf("1\n");
		return 0;
	}
	int result = 0;
	var = input % 5;
	input = input / 5;
	if (var < 5 && var >= 1) {
		input++;
	}
	printf("%d\n", input);
	return 0;
}
