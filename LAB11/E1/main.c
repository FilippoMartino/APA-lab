#include "diagonali.h"


int main(int argc, const char * argv[]) {
	
	if (argc < 2) {
		printf("ERROR! PARAM %s ELEMENTI", argv[0]);
		return -1;
	}
	
	int DD, DP;
	
	printf("Inserire, nell'ordine, DD e DP: ");
	scanf("%d %d", &DD, &DP);
	
	stampa_soluzione_ottima((char*) argv[1], DD, DP);
	
	
	return 0;
}
