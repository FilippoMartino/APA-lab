//
//  main.c
//  E03
//
//  Created by Filippo Martino on 30/09/2020.


#define MAX_N 30

void ruota(int v[], int N, int P, int dir);

/*
 Ruota di una singola posizione tutti gli elementi del vettore
 'v[]' di grandezza 'n' nella direzione 'dir'
 */
void ruota_singola_cella(int v[], int n, int dir);

#include <stdio.h>

int main(int argc, const char * argv[]) {
    
    int maxN = 0;
    
    /* P indica il numero di posizioni di cui verranno spostate le celle */
    int P = 0;
    /*
     Dir indica il verso dello spostamento:
     - [-1]: spostamento verso destra
     - [1]: spostamento verso sinistra
     - [0]: termina il programma
     */
    int dir = 0;
    
    printf("inserire n: ");
    scanf("%d", &maxN);
    
    if(maxN > MAX_N){
        printf("La grandezza massima possibile per il vettore è 30\n");
        return -1;
    }
    
    int V[maxN];
    
    printf("Acquisizione vettore: \n");
    for (int i = 0; i < maxN; i++) {
        printf("[%d]: ", i);
        scanf("%d", &V[i]);
    }
    
    printf("Inserire la direzione di rotazione: [1 SX, -1 DX, 0 stampa il vettore e termina il programma]: ");
    scanf("%d", &dir);
    
    /* Ho preferito un for rispetto al while */
    for(; dir != 0;){
        printf("Numero di posizioni: ");
        scanf("%d", &P);
        
        ruota(V, maxN, P, dir);
        
        printf("Vettore ruotato con successo\n");
        printf("\nInserire la direzione di rotazione: [1 SX, -1 DX, 0 stampa il vettore e termina il programma]: ");
        scanf("%d", &dir);
        
    }
    
    /* Stampa del vettore finale, prima di terminare il programma */
    printf("\nVettore ruotato: ");
    for (int i = 0; i < maxN; i++) {
        printf("[%d] ", V[i]);
    }
    
    printf("\n");
    
    return 0;
}

void ruota(int v[], int N, int P, int dir){
    
    /* Richiamo la funzione della singola rotazione n volte quante sono le posizioni richieste */
    for (int i = 0; i < P; i++)
        ruota_singola_cella(v, N, dir);

}

void ruota_singola_cella(int v[], int n, int dir){
    
    int i;
    /* Rotazione verso sinistra */
    if (dir == 1) {
        
        /* Salvo in una variabile l'unico elemento che verrà sovrascritto */
        int temp = v[0];
        
        for (i = 0; i < n - 1; i++)
            v[i] = v[i + 1];
        
        v[i] = temp;
        
    /* Rotazione verso destra*/
    } else {
        
        /*
         Essendo una scrittura "inversa" rispetto quella di sinistra, devo copiare il vettore in un
         altro, in modo da poter avere sempre i dati non sovrascritti
         */
        int v_copy[n];
        for (int i = 0; i < n; i ++)
            v_copy[i] = v[i];
        
        /* Ruoto elemento per elemento, sovrascrivendo di conseguenza l'ultimo */
        for (i = 0; i < (n - 1); i++)
            v[i + 1] = v_copy[i];
        
        /*
         L'ultimo elemento che era stato sovrascitto lo recupero dal
         vettore copia e lo inserisco in prima posizione
         */
        v[0] = v_copy[n - 1];
        
    }
    
}
