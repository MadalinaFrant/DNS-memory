/* FRANT Madalina - 315CB */

#include <stdio.h>
#include <stdlib.h>

/* Lista generica dublu inlantuita circulara */

typedef struct celula
{
    void *info;                 // adresa informatie
    struct celula *ant, *urm;   // legaturi spre celulele vecine
} TCelula, *TLista;

typedef void (*TFAfi)(FILE*, void*);  // functie afisare
typedef void (*TFElib)(void*);         // functie eliberare
typedef int (*TFCmp)(void*, void*);     // functie comparare 2 elemente

/* functii lista */

/* aloca celula si completeaza campul info cu x */
TLista AlocCelula(void *x);

/* distrugere lista - elimina toate celulele din lista */
void DistrugeL(TLista *L, TFElib elib_elem);

/* afiseaza elementele din lista, folosind o functie de tip TFAfi */
void AfisareL(FILE *f, TLista L, TFAfi afi_elem);

/* insereaza ordonat in lista */
void Ins(TLista *L, void *x, TFCmp cmp);

/* elimina prima celula din lista */
void ElimInc(TLista *L, TFElib elib_elem);

/* elimina celula de la adresa p */
void Elim(TLista p, TFElib elib_elem);