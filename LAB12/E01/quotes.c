#include "quotes.h"

#define MAX_TR 10

typedef struct _q_bst_node* link;

struct _q_bst_node {
	QUOTE quote;
	link parent; // puntatore al padre
	link left; // puntatore a figlio sinistro
	link right; // puntatore a figlio destro
	int N; // dimensione sottoalbero
};

/* La struttura propria del bst contiene solo il puntatore alla testa, e il valore del nodo sentinella */
struct _qbst {
	link root;
	link z;
};

/* Alloca un nuovo nodo, inizializzandolo con i parametri forniti */
static link new_node(QUOTE quote, link parent, link left, link right, int N){
	
	link node = (link) malloc(sizeof(link));
	node->quote = quote;
	node->parent = parent;
	node->left = left;
	node->right = right;
	
	node->N = N;
	
	return node;
	
}

/* Funzione ricorsiva per inserimento IN FOGLIA */
static link insert_leaf(link h, QUOTE quote, link z){
	
	/* Nel caso il BST sia vuoto o fossimo arrivati alla fine */
	if (h == z)
		return new_node(quote, z, z, z, 1);
	
	/* se la foglia da inserire è minore del nodo attuale ricorriamo a sinistra, altrimenti a destra */
	if (date_cmp(h->quote.date, quote.date) == -1){
		h->left = insert_leaf(h->left, quote, z);
		h->left->parent = h;
	} else {
		h->right = insert_leaf(h->right, quote, z);
		h->right->parent = h;
	}
	
	(h->N) ++;
	
	/* Ritorniamo h (puntatore a foglia) */
	return h;
	
}

/* Funzione banale per la copia di una transizione */
static Transaction tr_dup(Transaction tr){
	Transaction t;
	t.date = tr.date;
	t.quantity = tr.quantity;
	t.value = tr.value;
	
	return t;
}

/* Funzione ricorsiva per la stampa di un BST secondo strategie diverse */
static void tree_print(link h, link z, Strategy strategy){
	
	if (h == z)
		return;
	
	if (strategy == PREORDER)
		QUOTE_show(h->quote);
	
	tree_print(h->left, z, strategy);
	
	if (strategy == INORDER)
		QUOTE_show(h->quote);
	
	tree_print(h->right, z, strategy);
	
	if (strategy == POSTORDER)
		QUOTE_show(h->quote);;
	
}

/* Funzione ricorsiva di conteggio elementi BST */
static int count(link head, link z){
	
	if (head == z)
		return 0;
	
	return count(head->left, z) + count(head->right, z) + 1;
	
}

/* Dealloca ricorsivamente il BST */
static void tree_free(link head, link z){
	
	if (head == z)
		return;
	
	/* Ricorro fino alla fine dell'albero, dealloco il nodo (non avrò figli destri) e risalgo */
	
	tree_free(head->left, z);
	tree_free(head->right, z);
	
	free(head);
	
}

/* Funzione di ricerca ricorsiva */
static QUOTE search(link h, Date key, link z){
	
	/* Caso in cui abbiamo trovato l'item corrispondente alla chiave cercata */
	
	if (!date_cmp(h->quote.date, key))
		return h->quote;
	
	/* Caso in cui si sia arrivati alla fine dell'albero e la chiave non sia stata trovata */
	if (h == z)
		return QUOTE_set_default();
	
	/* Se la chiave è minore del nodo corrente ricorro sul ramo di sinistra */
	if (date_cmp(h->quote.date, key) == -1)
		return search(h->left, key, z);
	
	/* In caso contrario ricorro sul ramo di destra */
	return search(h->right, key, z);
	
}

void search_max_between(link h, Date date_1, Date date_2, link z, QUOTE* max){

	if (h == z)
		return;
	
	if (date_is_between(date_1, date_2, h->quote.date)){
		if (max->value < h->quote.value){
			max->date = h->quote.date;
			max->value = h->quote.value;
		}
	}
	
	search_max_between(h->left, date_1, date_2, z, max);
	search_max_between(h->right, date_1, date_2, z, max);
	
}

void search_min_between(link h, Date date_1, Date date_2, link z, QUOTE* max){

	if (h == z)
		return;
	
	if (date_is_between(date_1, date_2, h->quote.date)){
		if (max->value > h->quote.value){
			max->date = h->quote.date;
			max->value = h->quote.value;
		}
	}
	
	search_min_between(h->left, date_1, date_2, z, max);
	search_min_between(h->right, date_1, date_2, z, max);
	
}

void get_max_path_lenght(link h, link z, int* max){

	if (h == z)
		return;
	
	*max = *max + 1;
	
	get_max_path_lenght(h->left, z, max);
	get_max_path_lenght(h->right, z, max);
	
}

void get_min_path_lenght(link h, link z, int* min, int* stop){

	if (h == z){
		*stop = 1;
		return;
	}
	
	if (!*stop)
		*min = *min + 1;
	
	get_min_path_lenght(h->left, z, min, stop);
	get_min_path_lenght(h->right, z, min, stop);
	
}

/* Rotazione a destra */
static link rot_right(link h){
	
	link x = h->left;
	h->left = x->right;
	x->right->parent = h;
	x->right = h;
	x->parent = h->parent;
	h->parent = x;
	x->N = h->N;
	h->N = 1;
	h->N += (h->left) ? h->left->N : 0;
	h->N += (h->right) ? h->right->N : 0;
	
	return x;
}

/* Rotazione a sinistra */
static link rot_left(link h){
	
	link x = h->right;
	h->right = x->left;
	x->left->parent = h;
	x->left = h;
	x->parent = h->parent;
	h->parent = x;
	x->N = h->N;
	h->N = 1;
	h->N += (h->left) ? h->left->N : 0;
	h->N += (h->right) ? h->right->N : 0;
	
	return x;
	
}

/* Funzione di partizionamento: riorganizza il BST avendo l'item con la k-esima chiave più piccola in radice */
static link partition(link h, int r){
	
	int t = h->left->N;
	
	if (t > r){
		h->left = partition(h->left, r);
		h = rot_right(h);
	}
	
	if (t < r){
		h->right = partition(h->right, r - t - 1);
		h = rot_left(h);
	}
	
	return h;
	
}

/* Bilancia un bst */
static link balance(link h, link z){
	
	if (h == z)
		return z;
	
	int r = (h->N + 1) / 2 - 1;
	h = partition(h, r);
	h->parent = balance(h->left, z);
	h->right = balance(h->right, z);
	
	return h;
}

/*_________________________QUOTE_________________________*/

/* Restituisce una quotazione di default */
QUOTE QUOTE_set_default(void){
	
	QUOTE quote;
	quote.date = date_get_default();
	quote.value = -1;
	
	return quote;
}

/* Dato array di transizioni con una stessa data, restituisce la quotazione corrispondente */
QUOTE QUOTE_load(Transaction* tr_array, int tr_number){
	
	float num = 0;
	float den = 0;
	
	for (int i = 0; i < tr_number; i++) {
		num += tr_array[i].value * tr_array[i].quantity;
		den += tr_array[i].quantity;
	}
	
	QUOTE quote;
	
	quote.date = tr_array[0].date;
	quote.value = num/den;
	
	return quote;
	
}

/*__________________________BST__________________________*/

/* Inizializza un bst adattato al relativo problema */
Q_BST Q_BST_init(void){
	
	Q_BST bst = (Q_BST) malloc(sizeof(Q_BST));
	bst->z = new_node(QUOTE_set_default(), NULL, NULL, NULL, 0);
	bst->root = bst->z;
	
	return bst;
}

/* Wrapper, utilizza funzione insert_leaf() */
void Q_BST_insert_leaf(Q_BST bst, QUOTE quote){
	bst->root = insert_leaf(bst->root, quote, bst->z);
}

Q_BST Q_BST_load(Transaction* tr_array, int tr_number){
	
	Q_BST bst = Q_BST_init();
	
	int start = 0;
	int counter = 0;
	
	Transaction temp[MAX_TR];
	
	for (int i = 0; i < tr_number; i++) {
		
		if (i < (tr_number - 1) && date_same_day(tr_array[i].date, tr_array[i + 1].date)){
			counter ++;
		} else {
			
			counter ++;
			
			int x = 0;
			for (int j = start; x < counter; j ++, x++)
				temp[x] = tr_dup(tr_array[j]);
			
			Q_BST_insert_leaf(bst, QUOTE_load(temp, counter));
			
			counter = 0;
			start = i + 1;
			
		}
	}
	
	return bst;
	
}

/* Sfrutta la Q_BST_COUNT per verificare su il bst non ha elementi */
int Q_BST_empty(Q_BST bst){
	
	if (!Q_BST_count(bst))
		return 1;
	else
		return 0;
}

/* Wrapper, utilizza funzione count() */
int Q_BST_count(Q_BST bst){
	return count(bst->root, bst->z);
}

/* Wrapper, utilizza funzione tree_print() */
void Q_BST_visit(Q_BST bst, Strategy strategy){
	
	if (Q_BST_empty(bst)) {
		printf("BST vuoto\n");
		return;
	}
	
	tree_print(bst->root, bst->z, strategy);
	printf("\n");
}

/* Stampa formattata una quote */
void QUOTE_show(QUOTE quote){
	
	printf("Data: ");
	day_show(quote.date);
	printf(" - valore: %.2f\n", quote.value);
	
}

/* Wrapper, utilizza tree_free */
void Q_BST_free(Q_BST bst){
	
	if (bst == NULL)
		return;
	
	/* Deallochiamo tutto l'albero */
	tree_free(bst->root, bst->z);
	free(bst->z);
	free(bst);
	
}

QUOTE Q_BST_get_quote_in_date(Q_BST bst, Date key){
	return search(bst->root, key, bst->z);
}

QUOTE Q_BST_get_max_quote_between(Q_BST bst, Date date_1, Date date_2){
	
	QUOTE max = QUOTE_set_default();
	
	search_max_between(bst->root, date_1, date_2, bst->z, &max);
	
	return max;
}

QUOTE Q_BST_get_min_quote_between(Q_BST bst, Date date_1, Date date_2){
	
	QUOTE min = QUOTE_set_default();
	min.value = 1000000;
	
	search_min_between(bst->root, date_1, date_2, bst->z, &min);
	
	return min;
}

int Q_BST_get_max_path_len(Q_BST bst){
	
	int max = 0;
	get_max_path_lenght(bst->root, bst->z, &max);
	return max;
	
}

int Q_BST_get_min_path_len(Q_BST bst){
	
	int min = 0;
	int stop = 0;
	
	get_min_path_lenght(bst->root, bst->z, &min, &stop);
	
	return min;
	
}

void Q_BST_balance(Q_BST bst){
	bst->root = balance(bst->root, bst->z);
}
