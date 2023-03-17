/* FRANT Madalina - 315CB */

#include "headerLDI.h"

TLista AlocCelula(void *x)
{
    TLista aux = (TLista)malloc(sizeof(TCelula)); // incearca alocare
    if (!aux) return NULL; // daca alocarea nu a reusit intoarce NULL

    aux->ant = NULL;
    aux->urm = NULL;
    aux->info = x; // completeaza campul info cu valoarea x primita ca parametru

    return aux; // returneaza adresa celulei create sau NULL
}

void DistrugeL(TLista *L, TFElib elib_elem)
{
    while (*L != NULL) // cat timp exista elemente in lista
    {
        if (*L == ((*L)->urm)) // daca lista contine un singur element
        {
            elib_elem((*L)->info);  // elibereaza spatiul ocupat de element
            free(*L);               // elibereaza spatiul ocupat de celula
            *L = NULL;
        }
        else // lista are cel putin 2 elemente
            ElimInc(L, elib_elem); // se elimina primul element din lista
    }
}

void AfisareL(FILE *f, TLista L, TFAfi afi_elem)
{
    /* se memoreaza in celula1 valoarea primei celule
    pentru a putea implementa o conditie de oprire */
    TLista p = L, celula1 = L;
    if (L == NULL) // lista vida
    {
        fprintf(f, "VIDA");
        return;
    }

    /* in cazul unei liste cu un singur element,
    nu se intra in for, dar elementul este afisat
    la sfarsit */

    // parcurge lista pana la penultima celula
    for (; p->urm != celula1; p = p->urm)
    {
        afi_elem(f, p->info);
    }
    afi_elem(f, p->info); // afiseaza ultima celula
}

void Ins(TLista *L, void *x, TFCmp cmp)
{
    TLista aux = AlocCelula(x); // creeaza celula ce trebuie inserata
    if (!aux) return; // nu a reusit alocarea

    if (*L == NULL) // inserare in lista vida
    {
        aux->ant = aux->urm = aux;
        *L = aux; // actualizare inceput de lista
    }
    else
    {
        /* se memoreaza in celula1 valoarea primei celule
        pentru a putea implementa o conditie de oprire */
        TLista p = *L, celula1 = *L;

        if (cmp((*L)->info, x) == 2) // inserare inainte de primul element
        {
            TLista last = (*L)->ant; // se memoreaza ultima celula din lista
            
            // refacere legaturi
            last->urm = aux;
            aux->ant = last;
            aux->urm = *L;
            (*L)->ant = aux;

            *L = aux; // actualizare inceput de lista
        }
        // parcurge lista pana la penultimul element
        for (; p->urm != celula1; p = p->urm)
        {
            // inserare dupa elementul p (intre p si p->urm)
            if ((cmp(p->info, x) == 1) && (cmp(p->urm->info, x) == 2))
            {
                // refacere legaturi
                aux->ant = p;
                aux->urm = p->urm;
                p->urm->ant = aux;
                p->urm = aux;
            }
        }
        if (cmp(p->info, x) == 1) // inserare dupa ultimul element
        {
            // refacere legaturi
            p->urm = aux;
            aux->ant = p;
            (*L)->ant = aux;
            aux->urm = *L;
        }
    }
}

void ElimInc(TLista *L, TFElib elib_elem)
{
    TLista first, last;     // se memoreaza prima si ultima celula din lista
    first = *L;
    last = (*L)->ant;
    *L = (*L)->urm;         // actualizare inceput de lista

    // refacere legaturi
    (*L)->ant = last;
    last->urm = *L;

    elib_elem(first->info); // elibereaza spatiul ocupat de element
    free(first);            // elibereaza spatiul ocupat de celula
}

void Elim(TLista p, TFElib elib_elem)
{
    // refacere legaturi
    p->ant->urm = p->urm;
    p->urm->ant = p->ant;

    elib_elem(p->info);     // elibereaza spatiul ocupat de element
    free(p);                // elibereaza spatiul ocupat de celula
}