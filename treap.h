struct treape {
	int value;
	int prior;
	struct treape *parent;
	struct treape *left;
	struct treape *right;
};

struct treap{
	struct treape *root;
};



/* Hilffunktionen */

int is_leaf(struct treape *e) {
	if(e==0)
		return -1;
	if(e->left || e->right)
		return 0;
	return 1;
}

int is_left_child(struct treape *e) {
	if(e==0 || e->parent==0)
		return -1;
	if(e == e->parent->left)
		return 1;
	else
		return 0;
}

/* Einfache Ausgabe des Baumes */

int show(struct treape *e) {
	if(e==0)
		return 0;
	show(e->left);
	printf("Adress: 0x%x Value: %d Priority: %2d ParentAdr: 0x%x\n", (int)e, e->value, e->prior, (int)e->parent);
	show(e->right);
	return 0;
}


int lrotate(struct treap *t, struct treape *e) {
	struct treape *parent=e->parent;
	struct treape *child=e->right;
	struct treape *two=child->left;

	printf("Rotiere nach links\n");
	
	if(parent) {
		if(e==parent->left)
			parent->left=child;
		else 
			parent->right=child;
	} else {
		t->root=child;
	}   
	child->left=e;
	child->parent=parent;

	e->parent=child;
	e->right=two;
	if(two) 
		two->parent=e;
	return 0;
}

int rrotate(struct treap *t, struct treape *e) {
	struct treape *parent=e->parent;
	struct treape *child=e->left;
	struct treape *two=child->right;

	printf("Rotiere nach rechts\n");

	if(parent) {
		if(e==parent->left)
			parent->left=child;
		else
			parent->right=child;
	} else {
		t->root=child;
	}
	child->right=e;
	child->parent=parent;

	e->parent=child;
	e->left=two;
	if(two)
		two->parent=e;
	return 0;
}


int adjust_position(struct treap *t, struct treape *e) {
	while(e->prior > e->parent->prior) {
		if(is_left_child(e))
			rrotate(t, e->parent);
		else
			lrotate(t, e->parent);

		/* Sollte nach der Rotation der neue Knoten der Root-Knoten sein ist Ende */
		if(e->parent==0)
			return 0;

	}
	return 0;
}


int insert(struct treap *t, int value, int prior) {
	struct treape *e=t->root;
	struct treape *newe=(struct treape *)malloc(sizeof(struct treape));
	if(newe==0)
		return -1;
	newe->value=value;
	newe->prior=prior;
	newe->parent=newe->left=newe->right=0;
	if(t->root==0) {
		t->root=newe;
		return 0;
	}

	/* Einfache Einhaengen in den Baum wie im B-Tree */
	while(1) {
		if(e->value==value) {
			free(newe);
			return 1;
		}
		if(e->value<value) {
			if(e->right==0) {
				e->right=newe;
				newe->parent=e;
				break;
			} else {
				e=e->right;
			}
		}
		if(e->value>value) {
			if(e->left==0) {
				e->left=newe;
				newe->parent=e;
				break;
			} else {
				e=e->left;
			}
		}
	}
	printf("Fuege eine %d ein\n", value);
	adjust_position(t,newe);
	return 0;
}
