/* Definition der Datenstruktur */
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


/* Basisoperationen */
int delete(struct treap *t, int value);
int insert(struct treap *t, int value, int priority);
int random_insert(struct treap *t, int value);


/* Ausgabe */
int show(struct treap_node *e);
void print_smallest(struct treap *t);
void print_biggest(struct treap *t);

/* Funktion die Aufgeruft weren kann um, ein ein *.dot - File zu erstellen, 
 * welches anschliessend mittels graphviz in ein svg file verwandelt werden kann
 *
 * http://www.graphviz.org/
 */
int create_dotfile(struct treap *t);
