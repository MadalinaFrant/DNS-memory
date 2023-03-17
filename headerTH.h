/* FRANT Madalina - 315CB */

#include "headerLDI.h"

typedef int (*TFHash)(void*, int); // functie de calculare hash

typedef struct
{
    size_t M;
    TFHash fh;
    TLista *v;
} TH;

/* functii tabela hash */

/* initializare tabela hash */
TH* IniTH(int M, TFHash fh);

/* distrugere tabela hash */
void DistrTH(TH **T, TFElib elib_elem);

/* afisare tabela hash */
void AfiTH(FILE *f, TH *T, TFAfi afi_elem);

/* verifica existenta unui element in tabela hash
returneaza 1 daca exista, 0 daca nu */
int ExistaTH(TH *T, void *x, TFCmp f);

/* insereaza un element in tabela hash, daca acesta nu exista deja */
void InsTH(TH *T, void *x, TFCmp f, TFCmp f_ord, TFElib elib_elem);

/* elimina un element din tabela hash */
void ExtrTH(TH *T, void *x, TFCmp f, TFElib elib_elem);

/* returneaza valoarea corespunzatoare cheii Key,
respectiv NULL daca aceasta nu exista */
void* Get(TH *T, void *x, TFCmp f);