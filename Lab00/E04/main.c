//
//  main.c
//  E04
//
//  Created by Filippo Martino on 30/09/2020.
//

#include <stdio.h>
#include <string.h>

#define MAX_ROW_COLUM_SIZE 20
#define MAX_FILENAME_SIZE 20
#define MAX_SUBMATRIX_SIZE 100

int main(int argc, const char * argv[]) {
    
    char filename[MAX_FILENAME_SIZE];
    int col_number, row_number, max_submatrix_size, submatrix_size;
    
    printf("Inserire nome file: ");
    scanf("%s", filename);
    
    if (strlen(filename) > 20){
        printf("Lunghezza file non consentita\n");
        return -1;
    }
    
    FILE* fp = fopen(filename, "r");
    
    if (fp == NULL){
        printf("Errore nell'apertura del file\n");
        return -1;
    }
    
    /* Prendo il numero di righe ed il numero di colonne della matrice*/
    fscanf(fp, "%d %d", &row_number, &col_number);
    
    /* La sottomatrice quadrata più grande possibile è il valore minore tra i due */
    if (row_number > col_number)
        max_submatrix_size = col_number;
    else
        max_submatrix_size = row_number;
    
    /* L'utente deve scegliere la grandezza delle sottomatrici, comprese tra 1 e la grandezza massima */
    printf("Inserire taglia sottomatrici cercate [deve essere compresa tra 1 e %d]: ", max_submatrix_size);
    scanf("%d", &submatrix_size);
    
    /* Controllo sul dato inserito dall'utente */
    if (submatrix_size > max_submatrix_size || submatrix_size < 2){
        printf("La taglia scelta non può essere accettata\n");
        return -1;
    }
    
    /* Adesso devo salvare il contenuto del file dentro una matrice */
    int file_matrix[row_number][col_number];
    
    for (int i = 0; i < row_number; i ++) {
        for (int j = 0; j < col_number; j++) {
            fscanf(fp, "%d", &file_matrix[i][j]);
        }
    }
    
    /*
     variabili per algoritmo di generazione sottomatrici:
     
    -   max_elements_counter: serve per salvarmi globalmente la somma degli elementi
        di una sottomatrice più grande trovata fino a quel punto del ciclo
    
    -   current_elements_counter: somma ad ogni ciclo gli elementi della sottomatrice
        e si confronta con il suo "omonimo globale"
    
    -   current_elements_submatrix: stessa funzione della somma degli elementi locale,
        ma vengono salvati i valori della sottomatrice nel caso sia necessario poi stamparla alla fine
     
    -   max_elements_submatrix: associa al valore massimo trovato fino a
        quel punto del ciclo la matrice corrsispondente
     */
    
    int current_elements_counter = 0;
    int max_elements_counter = 0;
    int current_elements_submatrix[max_submatrix_size][max_submatrix_size];
    int max_elements_submatrix[max_submatrix_size][max_submatrix_size];
    
    /*
     Ho gestito la suddivisione in sottostrighe mediante 4 cicli for annidiati,
     2 servono per avanzare nel modo più classico sulla matrice ricavata dal file,
     i 2 più interni servono invece per ricavare le sottomatrici man a
     mano che si va avanti (a livello superiore)
     */
    
    for (int current_row = 0; current_row < row_number - (submatrix_size - 1); current_row++) {
        for (int current_colum = 0; current_colum < col_number - (submatrix_size - 1); current_colum ++) {
            
            for (int i = current_row; i < submatrix_size + current_row; i++) {
                for (int j = current_colum; j < submatrix_size + current_colum; j++) {
                    current_elements_submatrix[i- current_row][j - current_colum] = file_matrix[i][j];
                    current_elements_counter = file_matrix[i][j] + current_elements_counter;
                }
            }
            
            /*
             Ogni volta che viene individuata una nuova sottomatrice,
             viene salvata in una matrice temporane, appunto "current_elements_submatrix" e
             la somma degli elementi corrispondenti, salavata in "current_elements_counter" viene
             confrontata con la somma degli elementi precendentemente ottenuta, se maggiore viene
             sostituito sia il valore della somma sia la matrice corrispondente al nuovo valore
             */
            
            if (current_elements_counter > max_elements_counter){
                max_elements_counter = current_elements_counter;
                current_elements_counter = 0;
        
                for (int x = 0; x < submatrix_size; x ++){
                    for (int z = 0; z < submatrix_size; z ++) {
                        max_elements_submatrix[x][z] = current_elements_submatrix[x][z];
                        
                    }
                }
            }
            current_elements_counter = 0;
        }
    }

    /* Infine mi limito a stampare la sottomatrice trovata con il corrispettivo valore */
    printf("Matrice con il massimo valore [%d]:\n", max_elements_counter);
    for (int i = 0; i < submatrix_size; i++) {
        for (int j = 0; j < submatrix_size; j++) {
            printf("[%d] ", max_elements_submatrix[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}
