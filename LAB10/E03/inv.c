#include "inv.h"



void stat_read(FILE *fp, stat_t *statp){
	
	fscanf(fp, "%d %d %d %d %d %d",
		   &statp->hp,
		   &statp->mp,
		   &statp->atk,
		   &statp->def,
		   &statp->mag,
		   &statp->spr );
	
}


/* Ho interpretato soglia come il numero di elementi di statistiche da stampare */
void stat_print(FILE *fp, stat_t *statp, int soglia){

	stat_t my_stat = gestisci_soglia(statp, soglia);
	
	fprintf(fp, "%d %d %d %d %d %d",
			my_stat.hp,
			my_stat.mp,
			my_stat.atk,
			my_stat.def,
			my_stat.mag,
			my_stat.spr
			);
	
}

/* funzioni di input/output di un oggetto dell'inventario */
void inv_read(FILE *fp, inv_t *invp){
	
	fscanf(fp, "%s %s",
		   invp->nome,
		   invp->tipo);
	
	stat_read(fp, &invp->stat);
	
}

void inv_print(FILE *fp, inv_t *invp){
	
	fprintf(fp, "%s %s ",
			invp->nome,
			invp->tipo);
	
	stat_print(fp, &invp->stat, 1);
	fprintf(fp, "\n");
	
}

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp){
	
	return invp->stat;
	
}

stat_t gestisci_soglia(stat_t* stat, int soglia){
	
	stat_t current = *stat;
	
	if (current.hp < soglia)
		current.hp = 1;
	
	if (current.mp < soglia)
		current.mp = 1;
	
	if (current.atk < soglia)
		current.atk = 1;
	
	if (current.def < soglia)
		current.def = 1;
	
	if (current.mag < soglia)
		current.mag = 1;
	
	if (current.spr < soglia)
		current.spr = 1;
	
	return current;
	
}

stat_t init_stat(){
	stat_t current;
	current.atk = 0;
	current.def = 0;
	current.hp = 0;
	current.mag = 0;
	current.mp = 0;
	current.spr = 0;
	
	return current;
}

stat_t sum_stat(stat_t stat_1, stat_t stat_2){
	
	stat_t current;
	current.atk = stat_1.atk + stat_2.atk;
	current.def = stat_1.def + stat_2.def;
	current.hp = stat_1.hp + stat_2.hp;
	current.mag = stat_1.mag + stat_2.mag;
	current.mp = stat_1.mp + stat_2.mp;
	current.spr = stat_1.spr + stat_2.spr;
	
	return current;
}

