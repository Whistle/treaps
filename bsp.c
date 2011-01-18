#include "treap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char **argv)
{
	struct treap T={0};
	int i=0;
	srand(time(0));

	for(i=1; i <= 100; i++)
		random_insert(&T,i);

	show(T.root);
	print_smallest(&T);
	print_biggest(&T);
	create_dotfile(&T);
	return 0;
}

