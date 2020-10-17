//
//  main.c
//  E04
//
//  Created by Filippo Martino on 17/10/2020.
//

#include <stdio.h>
#include <string.h>

#define MAX_SEQUENCE_ELEMENTS 50
#define MAX_SEQUENCE_NUMBER 10
#define SHELL_SORT_GAP 2

void read_sequences(int, int[MAX_SEQUENCE_NUMBER][MAX_SEQUENCE_ELEMENTS], int[MAX_SEQUENCE_NUMBER], FILE*);
void selection_sort(int, int[MAX_SEQUENCE_ELEMENTS]);
void insertion_sort(int, int[MAX_SEQUENCE_ELEMENTS]);
void shell_sort(int, int[MAX_SEQUENCE_ELEMENTS]);
void print_vector(int, int[MAX_SEQUENCE_ELEMENTS]);
void copy(int, int[MAX_SEQUENCE_ELEMENTS], int[MAX_SEQUENCE_ELEMENTS]);

int main(int argc, const char * argv[]) {
	
	int sequence_number;
	
	if (argc < 2){
		printf("Errore: PARAM %s FILENAME\n", argv[0]);
		return -1;
	}
	
	FILE* fp = fopen(strdup(argv[1]), "r");
	if (fp == NULL){
		printf("Errore all'apertura del file %s\n", argv[1]);
		return -1;
	}
	
	fscanf(fp, "%d", &sequence_number);
	
	int sequences[sequence_number][MAX_SEQUENCE_ELEMENTS];
	int sequences_elements[sequence_number];
	read_sequences(sequence_number, sequences, sequences_elements, fp);
	
	for (int i = 0; i < sequence_number; i++) {
		
//		printf("Sequenza %d: ", i + 1);
//		print_vector(sequences_elements[i], sequences[i]);
		
		int temp[sequences_elements[i]];
		
		copy(sequences_elements[i], temp, sequences[i]);
		printf("\n");
		
		printf("Selection sort [sequenza %d]: ", i + 1);
		selection_sort(sequences_elements[i], temp);
//		print_vector(sequences_elements[i], temp);
		printf("\n");
		
		printf("Insertion sort [sequenza %d]: ", i + 1);
		copy(sequences_elements[i], temp, sequences[i]);
		insertion_sort(sequences_elements[i], temp);
//		print_vector(sequences_elements[i], temp);
		printf("\n");
		
		printf("Shell sort [sequenza %d]: ", i + 1);
		copy(sequences_elements[i], temp, sequences[i]);
		shell_sort(sequences_elements[i], temp);
//		print_vector(sequences_elements[i], temp);
		printf("\n");
		
		
		
	}
	
	fclose(fp);
	
	return 0;
}

void read_sequences(int sequence_number, int sequences[sequence_number][MAX_SEQUENCE_ELEMENTS], int sequences_elements[sequence_number], FILE*fp){
	
	int sequence_elements = 0;
	
	for (int i = 0; i < sequence_number; i++) {
		fscanf(fp, "%d", &sequence_elements);
		sequences_elements[i] = sequence_elements;
		for (int j = 0; j < sequence_elements; j ++) {
			fscanf(fp, "%d", &sequences[i][j]);
		}
	}
}

void selection_sort(int n, int sequence[n]){
	
	int min, t;
	int scambi = 0;
	int cicli_esterni = 0;
	int cicli = 0;
	
	for(int i = 0; i < n - 1; i++){
		cicli_esterni ++;
		min = i;
		for (int j = i + 1; j < n; j++){
			cicli ++;
			if (sequence[j] < sequence[min]){
				min = j;
			}
		}
		t = sequence[min];
		// si trova l'elemento più piccolo dell'array e si scambia con l'elemento alla posizione i
		sequence[min] = sequence[i];
		sequence[i] = t;
		scambi ++;
	}
	
	printf("numero scambi: %d, numero cicli esterni: %d, per ognuno di cui interni: %d, per un totale di %d cicli\n",
		   scambi,
		   cicli_esterni,
		   n,
		   cicli);
	
}

void insertion_sort(int n, int sequence[n]){
	
	int temp, j;
	
	int scambi = 0;
	int cicli_esterni = 0;
	int cicli = 0;
	
	for(int i = 1; i < n; i++){
		cicli_esterni ++;
		temp = sequence[i];
		j = i - 1;
		while(j >= 0 && sequence[j] > temp){
			cicli ++;
			sequence[j + 1] = sequence[j];
			scambi ++;
			j--;
		}
		sequence[j + 1] = temp;
	}
	
	printf("numero scambi: %d, numero cicli esterni: %d, per ognuno di cui interni: da 1 a %d (potenziale), per un totale di %d cicli\n",
		   scambi,
		   cicli_esterni,
		   n,
		   cicli);
}

void shell_sort(int n, int sequence[n]){
	
	int scambi = 0;
	int cicli_esterni_1 = 0;
	int cicli_esterni_2 = 0;
	int cicli = 0;
	
	for (int gap = n/SHELL_SORT_GAP; gap > 0; gap /= 2)	{
		cicli_esterni_1 ++;
		for (int i = gap; i < n; i += 1) {
			cicli_esterni_2 ++;
			int temp = sequence[i];
			
			int j;
			for (j = i; j >= gap && sequence[j - gap] > temp; j -= gap)
				sequence[j] = sequence[j - gap];
			scambi ++;
			cicli ++;
			
			sequence[j] = temp;
		}
	}
	
	printf("numero scambi: %d, numero cicli esterni (1 livello): %d, (2 livello): %d, per ognuno di cui interni: da 1 a %d (potenziale), per un totale di %d cicli\n",
		   scambi,
		   cicli_esterni_1,
		   cicli_esterni_2,
		   n,
		   cicli);
	
}

void print_vector(int n, int v[n]){
	for (int i = 0; i < n; i++){
		printf("[%d] ", v[i]);
	}
	printf("\n");
}

void copy(int n, int a[n], int b[n]){
	for (int i = 0; i < n; i ++) {
		a[i] = b[i];
	}
}

