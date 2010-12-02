#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "treap.h"

int main (int argc, char **argv)
{
	struct treap T={0};
	int i=0;
	int r=0;
	srand(time(0));
	for(i=0; i< 10; i++) {
		r=rand()%30+1;
		insert(&T, r, i);
	}
	show(T.root);
	delete(&T,r);
	show(T.root);
	return 0;
}

