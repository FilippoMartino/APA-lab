#include "titles.h"

int main(int argc, char* argv[]){
	
	if (argc < 2) {
		printf("Error! PARAM FILENAME%s\n", argv[0]);
		return -1;
	}
	
	FILE* fin = fopen(argv[1], "r");
	
	TL tl = TL_init();
	
	TL_store(&tl, fin);
	TL_show(tl);
	
	fclose(fin);
	
	printf("_______________________________\n\n");
	
	fin = fopen("F2.txt", "r");
	TL_store(&tl, fin);
	TL_show(tl);

	fclose(fin);
	
	printf("_______________________________\n\n");
	
	fin = fopen("F3.txt", "r");
	TL_store(&tl, fin);
	TL_show(tl);

	
	TL_free(tl);
	
	
	
	return 0;
}
