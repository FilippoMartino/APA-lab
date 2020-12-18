#include "pg.h"

/* quasi ADT personaggio; si noti che si tratta di un composto con riferimento
a un equipArray di proprieta' */

/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pgp){
	
	if (fp == NULL) {
		printf("Errore durante la lettura da file\n");
		return -1;
	}
	
	fscanf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe);
	stat_read(fp, &pgp->b_stat);
	pgp->equip = equipArray_init();
	
	return 0;
	
}
/* non essendo struct dinamica, pulisce chiamando il distruttire di equipArray */
void pg_clean(pg_t *pgp){
	equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray){
	
	
	
	fprintf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
	stat_print(fp, &pgp->b_stat, 1);
	fprintf(fp, "\nEquipaggiamenti: ");
	equipArray_print(fp, pgp->equip, invArray);
	
}

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray){
	equipArray_update(pgp->equip, invArray);
}
