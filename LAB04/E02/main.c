#include <stdio.h>

int majority(int*, int);

int main(int argc, const char * argv[]) {
	
	int N, major;
	
	printf("Inserire lunghezza vettore: ");
	scanf("%d", &N);
	
	int a[N];
	printf("Inserire adesso gli elementi del vettore separati da uno spazio: ");
	for (int i = 0; i < N; i++) {
		scanf("%d", &a[i]);
	}
	
	major = majority(a, N);
	
	if (major != -1)
		printf("Elemento maggioritario: %d\n", major);
	else
		printf("Non è presente un elemento maggioritario\n");
	
	return 0;
	
}

/**
 Funzione ricorsiva di ricerca elemento maggioritario, ossia quel'elemento
 che compare nel vettore più di N/2 volte, può anche non esistere.
 
 Il ragionamento che ho fatto è collegato alla ricorsione fatta durante le lezioni
 di teoria sulla ricerca dell'elemeto maggiore di un vettore: era necessario tenere
 conto, ad ogni ricorsione, di un sottovettore di sinistra ed un sottovettore di destra
 
 Ho quindi previsto di cercare l'elemento maggioritario (se esistente) nei due
 sottovettori di sinstra e di destra che si vanno a generare "giocando" con l'aritmetica
 dei puntatori (nelle lezioni di teoria si usava un funzione con i parametri left e right, che indicavano
 inizio e fine del vettore, in questo esercizio si bypassa facilemente appunto spostando il
 puntatore avanti di N/2 posizioni per il caso di destra).
 I due sottovettori vengono poi passati ricorsivamente alla funzione stessa, che ripete l'operazione
 sino al caso banale, ossia se ho un vettore lungo 2 oppure singolo, nel caso:
 - vettore di lunghezza 1: elemento maggioritario è l'elemento stessso, return elemento
 - vettore di lunghezza 2: elemento maggioritario presente solo se entrambe le celle contengono lo stesso
  elemento, restitusico l'elemento in caso positivo, -1 in caso negativo
 
 In questo modo, una volta arrivati alle soluzioni banali di destra e di sinistra, ricompongo la soluzione
 del problema "tornando indietro" e controllando:
 - se entrambi i sottovettori sono == -1, allora anche il mio vettore (che potrebbe essere a sua volta un
  un sottovettore) non contiene l'elemento maggioritario.
 - se sottovettore di sinistra contiene elemento maggiritario (controllo se != -1): controllo se l'elemento
  maggioritario del sottovettore è presente nel vettore più di N/2 volte, in caso positivo restituisco il valore,
  in caso negativo non faccio niente, resta ancora da controllare il sottovettore di destra
 - se sottovettre di destra contiene elemento maggioritario: faccio gli stessi controlli del punto precedente,
  in questo modo se dovessi trovare l'elemento maggioritario lo resituirei, in caso contrario mi limito
  ad uscire dal ciclo, il caso negativo (ossia se non ho trovato ricorrenze di elemento maggioritario ne
  da destra ne da sinistra) viene gestito alla fine della funzione, che se non ha ritornato un elemento maggioritario
  a questo punto da per scontato non esistere e ritorna -1
 
 Ripetaendo questi cicli ricorsivamente si trova infine l'elemento maggioriario
 
 */
int majority(int* a, int N){
	
	/* Caso banale */
	if (N == 2){
		if (*a == *(a + 1))
			return *a;
		else
			return -1;
	}
	
	if (N == 1)
		return *a;
	
	/* Devo registrarmi l'elemento maggiore dei sottovettori di destra e di sinistra */
	int el_m_sx = majority(a, N/2);
	int el_m_dx = majority((a + N/2), N / 2);
	
	if (el_m_sx == -1 && el_m_dx == -1){
		return -1;
	}
	
	int temp = 0;
	for (int i = 0; i < N && el_m_sx != -1; i++) {
		if(a[i] == el_m_sx)
			temp ++;
	}
	if (temp > N/2)
		return el_m_sx;
	
	temp = 0;
	for (int i = 0; i < N && el_m_dx != -1; i++) {
		if(a[i] == el_m_dx)
			temp ++;
	}
	if (temp > N/2)
		return el_m_dx;
	
	
	return -1;
	
}
