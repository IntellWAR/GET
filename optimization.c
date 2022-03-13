#include <stdio.h>
struct ts {
	int a;
	int b;
};

int main (int argc, char** argv) {
	int i=0, j = 3;
	struct ts* a;
	i = 5 + j;
	for (;i>j;i--) {
	j+=i;
	}
	printf ("%d\n", i);
}