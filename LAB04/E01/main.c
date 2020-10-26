#include <stdio.h>

int gcd(int, int);
void swap(int*, int*);

int main(int argc, const char * argv[]) {
	
	int a, b;
	printf("Inserire due numeri: ");
	scanf("%d %d", &a, &b);
	
	/* Se il primo numero è minore del secondo li scambio */
	if (a < b)
		swap(&a, &b);
	
	printf("Il massimo comun divisore è %d\n", gcd(a, b));
	
	return 0;
}

/* Semplice funzione che scambia i due numeri */
void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

/* Funzione ricorsiva per il calcolo del massimo comun divisore */
int gcd(int a, int b){
	
	/**
	 si fanno delle chiamate ricorsive assegnando di volta in volta al primo numero il secondo (minore),
	 al secondo il resto della divisione tra il numero maggiore ed il numero minore
	 continuando così ricorsivamente arriveremmo ad avere il prima o poi un divisore (sempre il numero più piccolo
	 che divide il più grande) che dividerà senza lasciare resto: questo sarà il più grande dei divisori comuni.
	 Avendo quindi resto 0, ed essendo inserito nella seconda variabile (b) la condizione di fine ricorsione sarà
	 proprio il controllo del resto 0
	 */
	
	
	if (b == 0)
		return a;
	
	return gcd(b, a % b);
	
}
