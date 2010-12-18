int null_count=0;

struct treap_node {
	int value;
	int priority;
	struct treap_node *parent;
	struct treap_node *left;
	struct treap_node *right;
};

struct treap{
	struct treap_node *root;
};


/* Hilffunktionen */
int is_leaf(struct treap_node *e) {
	if(e==0)
		return -1;
	if(e->left || e->right)
		return 0;
	return 1;
}

int is_left_child(struct treap_node *e) {
	if(e==0 || e->parent==0)
		return -1;
	if(e == e->parent->left)
		return 1;
	else
		return 0;
}

int has_brother(struct treap_node *e) {
	if(e==0)
		return 0;
	if(e->parent==0)
		return 0;
	if(is_left_child(e) && e->parent->right!=0)
		return 1;
	if(!is_left_child(e) && e->parent->left!=0)
		return 1;
	return 0;
}

int lrotate(struct treap *t, struct treap_node *e) {
	struct treap_node *parent=e->parent;
	struct treap_node *child=e->right;
	struct treap_node *two=child->left;

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

int rrotate(struct treap *t, struct treap_node *e) {
	struct treap_node *parent=e->parent;
	struct treap_node *child=e->left;
	struct treap_node *two=child->right;

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
/* Einfache Ausgabe des Baumes */

int show(struct treap_node *e) {
	if(e==0)
		return 0;
	show(e->left);
	printf("Adress: 0x%x Value: %d priorityity: %2d ParentAdr: 0x%x\n", (int)e, e->value, e->priority, (int)e->parent);
	show(e->right);
	return 0;
}

struct treap_node *find(struct treap *t, int value) {
	struct treap_node *e=0;
	if(t ==0)
		return 0;
	e=t->root;
	while(e) {
		if(e->value==value)
			return e;
		else if(e->value>value)
			e=e->left;
		else
			e=e->right;
	}
	return 0;
}

int delete(struct treap *t, int value) {
	struct treap_node *e=find(t,value);
	if(e==0)
		return 0;
	printf("Loesche die %d\n", value);
	while(is_leaf(e)==0) {
		if(e->left && e->right) {
			if(e->left->priority > e->right->priority) {
				rrotate(t, e);
			} else {
				lrotate(t, e);
			}
		} else if(e->left && !e->right) {
			rrotate(t, e);
		} else {
			lrotate(t, e);
		}
	}
	if(is_left_child(e)) {
		e->parent->left=0;
		free(e);
	} else {
		e->parent->right=0;
		free(e);
	}
	return 0;
}

int adjust_position(struct treap *t, struct treap_node *e) {
	while(e->priority > e->parent->priority) {
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

int insert(struct treap *t, int value, int priority) {
	struct treap_node *e=t->root;
	struct treap_node *newe=(struct treap_node *)malloc(sizeof(struct treap_node));
	if(newe==0)
		return -1;
	newe->value=value;
	newe->priority=priority;
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

int build_dottree(FILE *fd, struct treap_node *e) {
	if(fd==0)
		return -1;
	if(e==0) {
		return 0;
	}
	if(e->left) {
		fprintf(fd, "%d -> %d;\n", e->value, e->left->value);
		build_dottree(fd, e->left);
	} else {
		null_count++;
		fprintf(fd, "null%d [shape=none, style=invisible];\n", null_count);
		fprintf(fd, "%d -> null%d\n", e->value, null_count);
	}
	if(e->right) {
		fprintf(fd, "%d -> %d;\n", e->value,  e->right->value);
		build_dottree(fd, e->right);
	} else {
		null_count++;
		fprintf(fd, "null%d [shape=none, style=invisible];\n", null_count);
		fprintf(fd, "%d -> null%d;\n", e->value, null_count);
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
