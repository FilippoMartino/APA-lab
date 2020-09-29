#include <stdio.h>

/* Massima grandezza vettore */
#define MAX_N 30

/*  
    V[maxN]: vettore circolare
    N: lunghezza vettore
    P: numero di posizioni 
    dir: -1 per destra, 1 per sinistra
*/

void ruota(int v[maxN], int N, int P, int dir);

int main(int argc, char const *argv[]) {
    
    int maxN = MAX_N;
    int N, P, dir;

    printf("Inserire lunghezza vettore (massimo 30): ")
    scanf("%d", &N);
    
    /* Controllo su dimensione vettore inserita */
    if(N > 30){
        printf("dimensione inserita non compatibile\n");
        return -1;
    }

    int v[N];

    printf("inserire vettore, posizioni:");
    for (int i = 0; i < N; i++){
        printf("[%d]", i);
        scanf("%d", &v[i]);
    }
    

    
    return 0;
}

void ruota(int v[maxN], int N, int P, int dir){

}