#include <stdio.h>

#define TRUE 1
#define FALSE 0

int is_big_endian(void);
void stampaCodifica (void *, int, int);

int main(int argc, const char * argv[]) {
	
	float af;
	double ad;
	long double ald;
	
	int big_endian = is_big_endian();
	
	printf("Dimensioni variabili: \n");
	printf("- float: %d byte, %d bit\n", (int) sizeof(float), (int) sizeof(float) * 8);
	printf("- double: %d byte, %d bit\n", (int) sizeof(double), (int) sizeof(double) * 8);
	printf("- long double: %d byte, %d bit\n\n", (int) sizeof(long double), (int) sizeof(long double) * 8);
	
	printf("Inserisci un numero (con virgola ed evenutale esponente: ");
	scanf("%f", &af);
	printf("\n");
	ad = (double) af;
	ald = (long double) af;
	
	printf("Float:\n");
	stampaCodifica((void*) &af, sizeof(float), big_endian);
	printf("Double:\n");
	stampaCodifica((void*) &ad, sizeof(double), big_endian);
	printf("Long Double:\n");
	stampaCodifica((void*) &ald, sizeof(long double), big_endian);
	
	//	printf("numero float %f\n", af);
	//	printf("numero double %f\n", ad);
	//	printf("numero double %Lf\n", ald);
	
	return 0;
}

/* Determina il tipo di codifica */
int is_big_endian(void){
	
	/**
	 Per determinare il tipo di codifica che utlizza il compilatore (big endian o little endian), vado anzitutto a
	 dichiarare un intero (n) con il valore 1 (2 byte -> 16 bit) , che verrà codificato in c come
	 - caso little endian: 1000000000000000 (il Least significant byte occupa indirizzo di memoria più basso)
	 - caso big endian: 0000000000000001 (il Least significant byte occupa indirizzo di memoria più alto)
	 
	 Dichiaro poi un puntatore ad un char (1 byte -> 8 bit), a cui casto il puntatore all'intero.
	 Nella memoria ram mi trovo ad avere 16 celle di memoria contigue codificate come specificato prima (nell'elenco
	 puntato), con un incremento del puntatore char *p mi sposto avanti ed indetro di 8 bit alla volta:
	 se facessi p* + 1 finirei sulla prima cella del secondo blocco, ossia sul primo bit del secondo byte.
	 Per capire il tipo di codifica mi è sufficente controllare se il primo bit del primo byte puntato da *p è uguale
	 ad 1, in tal caso ho una codifica little endian, in caso contrario big endian
	 
	 */
	
	int n = 1;
	char *p = (char*) &n;
	
	if (*p == 1)
		return FALSE;
	else
		return TRUE;
}

/* Stampa la codifica binaria */
void stampaCodifica (void *p, int size, int bigEndian){
	
	/*
	 Dichiarazione puntatore unsigned char da usare
	 come supporto per conteggio singoli bit
	 */
	unsigned char *b = (unsigned char *) p;
	/* Questo mi serve per inserirci dentro un byte */
	unsigned char byte;
	
	/* Queste variabili mi servono per formattare la stampa */
	int sign_bit = 1;
	int exp_number;
	int exp_printed = 0;
	int first_mantissa = 1;
	
	/* Il numero dell'esponente varia in base al tipo, faccio dei controlli */
	if (size == 4)
		exp_number = 8;
	else if (size == 8)
		exp_number = 11;
	else
		exp_number = 15;
	
	/* Faccio due cicli di lettura, se fosse big endian il codice è uguale ma i cicli sono invertiti */
	if(!bigEndian){
		
		/**
			Per leggere un singolo bit opero come segue:
			- Ciclo for esterno che scorre il "vettore" avanti o indietro, in base a se ho una codifica big o little endian.
			- Ciclo for interno che scorre i singoli gruppi da 1 byte avanti o indietro sempre in base alla codifica.
			
			Ad ogni ciclo, utilizzando gli operatori di bitwise, sposto il bit puntato dall'unsigned char alla prima posizione di un nuovo spazio
			in memoria riservato ad un char, ossia all'area di memoria minore del byte successivo:
			- Viene prima fatto lo spostamento di j posizioni con >> (l'operatore di >> non va a modificare la memoria originale)
			- Successivamente viene confrontato con un valore 1 mediante l'operatore & che restituisce 1 se entrambe le ricorrenze sono
			 1, 0 se una delle due è 0
			In questo modo riesco ad inserire in byte, un valore 0 o 1 che potrò poi andare a stampare singolarmente: per inserire un 1/0 come
			caratteri effettuo una conversione mediante + '0' che converte un intero in carattere.
		 
			La formattazione viene gestita con dei flag
			
			
		 */
		
		for (int i = size - 1; i >= 0; i--) {
			for (int j = 7; j >= 0; j--) {
				byte = ((b[i] >> j) & 1) + '0';
				
				if(sign_bit){
					printf("Bit di segno: %c\n", byte);
					sign_bit = 0;
					printf("Bit di esponente: ");
				}
				if(exp_printed < exp_number){
					printf("%c", byte);
					exp_printed ++;
				}
				if(exp_printed == exp_number){
					if (first_mantissa){
						printf("\nBit di mantissa: ");
						printf("%c", byte);
						first_mantissa = 0;
					}
					printf("%c", byte);
				}

			}
		}
	}else{
		for (int i = 0; i < size; i++) {
			
			for (int j = 0; j < 7; j++) {
				byte = ((b[i] >> j) & 1) + '0';
				
				if(sign_bit){
					printf("Bit di segno: %c\n", byte);
					sign_bit = 0;
					printf("Bit di esponente: ");
				}
				if(exp_printed < exp_number){
					printf("%c", byte);
					exp_printed ++;
				}
				if(exp_printed == exp_number){
					if (first_mantissa){
						printf("\nBit di mantissa: ");
						printf("%c", byte);
						first_mantissa = 0;
					}
					printf("%c", byte);
				}
			}
			
		}
	}
	
	printf("\n");
	puts("");
}
	
	
