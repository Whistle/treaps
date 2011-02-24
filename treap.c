#include "treap.h"
#include <stdlib.h>
#include <stdio.h>

int null_count=0;

/* Hilffunktionen */
int is_leaf(struct treap_node *n) {
	if(n==0)
		return -1;
	if(n->left || n->right)
		return 0;
	return 1;
}

int is_left_child(struct treap_node *n) {
	if(n==0 || n->parent==0)
		return -1;
	if(n == n->parent->left)
		return 1;
	else
		return 0;
}

void print_smallest(struct treap *t) {
	struct treap_node *n=0;
	if(t==0)
		return;
	n=t->root;
	while(n->left) {
		n=n->left;
	}
	printf("Smallest number:\nValue: %d Priority: %d\n", n->value, n->priority);
}

void print_biggest(struct treap *t) {
	struct treap_node *n=0;
	if(t==0)
		return;
	n=t->root;
	while(n->right) {
		n=n->right;
	}
	printf("Biggest number:\nValue: %d Priority: %d\n", n->value, n->priority);
}

int lrotate(struct treap *t, struct treap_node *n) {
	struct treap_node *parent=n->parent;
	struct treap_node *child=n->right;
	struct treap_node *two=child->left;

	printf("Rotiere nach links\n");

	if(parent) {
		if(n==parent->left)
			parent->left=child;
		else 
			parent->right=child;
	} else {
		t->root=child;
	}   
	child->left=n;
	child->parent=parent;

	n->parent=child;
	n->right=two;
	if(two) 
		two->parent=n;
	return 0;
}

int rrotate(struct treap *t, struct treap_node *n) {
	struct treap_node *parent=n->parent;
	struct treap_node *child=n->left;
	struct treap_node *two=child->right;

	printf("Rotiere nach rechts\n");

	if(parent) {
		if(n==parent->left)
			parent->left=child;
		else
			parent->right=child;
	} else {
		t->root=child;
	}
	child->right=n;
	child->parent=parent;

	n->parent=child;
	n->left=two;
	if(two)
		two->parent=n;
	return 0;
}
/* Einfache Ausgabe des Baumes */

int show(struct treap_node *n) {
	if(n==0)
		return 0;
	show(n->left);
	printf("Adress: 0x%x Value: %d priorityity: %2d ParentAdr: 0x%x\n", (int)n, n->value, n->priority, (int)n->parent);
	show(n->right);
	return 0;
}

struct treap_node *find(struct treap *t, int value) {
	struct treap_node *n=0;
	if(t==0)
		return 0;
	printf("Suche die %d\n", value);
	n=t->root;
	while(n) {
		if(n->value==value) {
			printf("Gefunden\n");
			return n;
		} else if(n->value>value) {
			n=n->left;
		} else {
			n=n->right;
		}
	}
	return 0;
}

int delete(struct treap *t, int value) {
	struct treap_node *n=find(t,value);
	if(n==0)
		return 0;
	printf("Loesche die %d\n", value);

	/* Blatt werden */
	while(is_leaf(n)==0) {
		if(n->left && n->right) {
			if(n->left->priority > n->right->priority) {
				rrotate(t, n);
			} else {
				lrotate(t, n);
			}
		} else if(n->left && !n->right) {
			rrotate(t, n);
		} else {
			lrotate(t, n);
		}
	}

	/* Pointer des Vaterknotens nullen und den Knoten loeschen */
	if(is_left_child(n)) {
		n->parent->left=0;
		free(n);
	} else {
		n->parent->right=0;
		free(n);
	}
	return 0;
}

int adjust_position(struct treap *t, struct treap_node *n) {
	while(n->priority >= n->parent->priority) {
		if(n->priority == n->parent->priority) {
			return 0;
		}

		if(is_left_child(n))
			rrotate(t, n->parent);
		else
			lrotate(t, n->parent);

		/* Sollte nach der Rotation der neue Knoten der Root-Knoten sein ist Ende */
		if(n->parent==0)
			return 0;

	}
	return 0;
}

int insert(struct treap *t, int value, int priority) {
	struct treap_node *n=t->root;
	struct treap_node *new_node=(struct treap_node *)malloc(sizeof(struct treap_node));
	if(new_node==0)
		return -1;
	new_node->value=value;
	new_node->priority=priority;
	new_node->parent=new_node->left=new_node->right=0;
	if(t->root==0) {
		t->root=new_node;
		return 0;
	}

	/* Einfaches Einhaengen in den Baum (aufsteigend sortiert) */
	while(1) {
		if(n->value==value) {
			free(new_node);
			return 1;
		}
		if(n->value<value) {
			if(n->right==0) {
				n->right=new_node;
				new_node->parent=n;
				break;
			} else {
				n=n->right;
			}
		}
		if(n->value>value) {
			if(n->left==0) {
				n->left=new_node;
				new_node->parent=n;
				break;
			} else {
				n=n->left;
			}
		}
	}
	printf("Fuege eine %d ein\n", value);
	return adjust_position(t,new_node);
}

int random_insert(struct treap *t, int value) {
	return insert(t, value, rand()%10000);
}

int build_dottree(FILE *fd, struct treap_node *n) {
	if(fd==0)
		return -1;
	if(n==0) {
		return 0;
	}
	if(n->left) {
		fprintf(fd, "%d [label=\"%d\\n%d\"]",n->value, n->priority, n->value);
		fprintf(fd, "%d -> %d;\n", n->value, n->left->value);
		build_dottree(fd, n->left);
	} else {
		null_count++;
		fprintf(fd, "null%d [shape=none, style=invisible];\n", null_count);
		fprintf(fd, "%d [label=\"%d\\n%d\"]",n->value, n->priority, n->value);
		fprintf(fd, "%d -> null%d\n", n->value, null_count);
	}
	if(n->right) {
		fprintf(fd, "%d [label=\"%d\\n%d\"]",n->value, n->priority, n->value);
		fprintf(fd, "%d -> %d;\n", n->value,  n->right->value);
		build_dottree(fd, n->right);
	} else {
		null_count++;
		fprintf(fd, "null%d [shape=none, style=invisible];\n", null_count);
		fprintf(fd, "%d [label=\"%d\\n%d\"]",n->value, n->priority, n->value);
		fprintf(fd, "%d -> null%d;\n", n->value, null_count);
	}
	return 0;
}

int create_dotfile(struct treap *t)  {
	/* Write new Header*/
	FILE *fd=fopen("tree.dot", "w");
	printf("No File yet. Writing Header to File!\n");
	fprintf(fd, "digraph treap {\n");
	fprintf(fd, "node [shape=circle, fontname=\"Arial\"];\n");
	build_dottree(fd, t->root);
	fprintf(fd, "}");
	return 0;
}
