#ifndef INV_H_DEFINED
#define INV_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LEN 100
#define MIN_STAT 1

/* quasi ADT statistiche */
typedef struct stat_s {
  int hp, mp, atk, def, mag, spr;
} stat_t;

/* quasi ADT oggetto di inventario */
typedef struct inv_s {
  char nome[LEN];
  char tipo[LEN];
  stat_t stat;
} inv_t;

/* funzioni di input/output delle statistiche */
void stat_read(FILE *fp, stat_t *statp);
void stat_print(FILE *fp, stat_t *statp, int soglia);

/* funzioni di input/output di un oggetto dell'inventario */
void inv_read(FILE *fp, inv_t *invp);
void inv_print(FILE *fp, inv_t *invp);

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

/* Serve per gesire strutture con valori stat sotto la soglia*/
static stat_t gestisci_soglia(stat_t* stat, int soglia);

stat_t init_stat(void);

stat_t sum_stat(stat_t stat_1, stat_t stat_2);

#endif

