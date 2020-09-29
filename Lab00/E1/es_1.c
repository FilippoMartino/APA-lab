#include <stdio.h>

int main(int argc, char const *argv[]) {
    
    int lenght;

    printf("Lunghezza vettore: ");
    scanf("%d", &lenght);
    int V[lenght];
    
    for (int i = 0; i < lenght; i++) {
        printf("%d - Valore: ", i);
        scanf("%d", &V[i]);
    }

    printf("Vettore: [");
    for (int i = 0; i < lenght; i++) {
        printf(" %d ", V[i]);
    }
    printf("]\n[");
    
    for(int i = 0; i < lenght; i++) {
        if(V[i] == 0)
            printf("] [");
        else
            printf(" %d ", V[i]);
    }

    printf("]\n");

    return 0;
}
