#include <stdio.h>

int plus(int, int);
int minus(int, int);

int main() {
	int a, b, r, s;
	scanf("%d %d", &a, &b);
	r = plus(a, b);
	s = minus(a, b);
	printf("%d\n%d\n", r, s);
	return 0;
}
