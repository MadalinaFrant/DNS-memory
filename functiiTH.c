/* FRANT Madalina - 315CB */

#include "headerTH.h"

TH* IniTH(int M, TFHash fh)
{
    TH* h = (TH *) calloc(sizeof(TH), 1); // incearca alocare tabela hash
    if (!h) // alocarea nu a reusit
    {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TLista *) calloc(M, sizeof(TLista)); // incearca alocare vector
    if (!h->v) // alocarea nu a reusit
    {
        printf("eroare alocare vector de pointeri TLista in hash\n");
        free(h);
        return NULL;
    }

    h->M = M;   // completare camp M (numar maxim de bucket-uri)
    h->fh = fh; // completare camp fh (functie hash)

    return h;
}

void DistrTH(TH **T, TFElib elib_elem)
{
    TLista *p;

    for (p = (*T)->v; p < (*T)->v + (*T)->M; p++) 
        DistrugeL(p, elib_elem); // distruge fiecare lista din tabela hash

    free((*T)->v);  // eliberare vector
    free(*T);       // eliberare tabela hash
    *T = NULL;
}

void AfiTH(FILE *f, TH *T, TFAfi afi_elem)
{
    TLista p;
    int i;
    for (i = 0; i < T->M; i++) 
    {
        p = T->v[i];
        if (p) // daca lista (bucket-ul) exista se afiseaza
        {
            fprintf(f, "%d: ", i);
            AfisareL(f, p, afi_elem);
            fprintf(f, "\n");
        }
    }
}

int ExistaTH(TH *T, void *x, TFCmp f)
{
    int cod = T->fh(x, T->M);

    /* se memoreaza in celula1 valoarea primei celule
    pentru a putea implementa o conditie de oprire */
    TLista p = T->v[cod], celula1 = p;

    if (p == NULL) // daca lista este vida
        return 0;

    /* in cazul unei liste cu un singur element;
    nu se intra in for, dar conditia de existenta
    este verificata la sfarsit */

    // parcurgere pana la penultimul element
    for (; p->urm != celula1; p = p->urm)
    {
        if (f(p->info, x) == 1)
            return 1;
    }
    if (f(p->info, x) == 1) // trebuie verificat si ultimul element
            return 1;

    return 0;
}

void InsTH(TH *T, void *x, TFCmp f, TFCmp f_ord, TFElib elib_elem)
{
    int cod = T->fh(x, T->M);

    /* daca elementul nu exista in tabela hash acesta se insereaza,
    altfel se elibereaza spatiul alocat acestuia */

	if (ExistaTH(T, x, f) == 0)
		Ins(T->v + cod, x, f_ord);
    else
        elib_elem(x);
}

void ExtrTH(TH *T, void *x, TFCmp f, TFElib elib_elem)
{
    TLista *p, celula1, el;

    for (p = T->v; p < T->v + T->M; p++)
    {
        if (*p == NULL) // lista vida
            continue; // se trece la urmatoarea lista

        if ((*p) == (*p)->urm) // lista cu un singur element
        {
            if (f((*p)->info, x) == 1) // daca elementul trebuie eliminat
            {
                elib_elem((*p)->info);  // elibereaza spatiul ocupat de element
                free(*p);               // elibereaza spatiul ocupat de celula
                *p = NULL;
                return;
            }
            else
            {
                continue; // se trece la urmatoarea lista
            }
        }

        /* se memoreaza in celula1 valoarea primei celule
        pentru a putea implementa o conditie de oprire */
        celula1 = *p;

        // parcurgere pana la penultimul element
        for (el = *p; el->urm != celula1; el = el->urm)
        {
            if (f(el->info, x) == 1)
            {
                if (el == *p) // daca trebuie eliminat primul element din lista
                {
                    ElimInc(p, elib_elem);
                    return;
                }
                else // elementul de eliminat nu este primul in lista
                {
                    Elim(el, elib_elem);
                    return;
                }
            }
        }
        if (f(el->info, x) == 1) // trebuie verificat si ultimul element
        {
            Elim(el, elib_elem);
            return;
        }
    }
}

void* Get(TH *T, void *x, TFCmp f)
{
    TLista p, celula1;
    int i;
    for (i = 0; i < T->M; i++) 
    {
        p = T->v[i];
        /* se memoreaza in celula1 valoarea primei celule
        pentru a putea implementa o conditie de oprire */
        celula1 = p;

        if (p == NULL) // lista vida
            continue; // se trece la urmatoarea lista
        
        /* in cazul unei liste cu un singur element;
        nu se intra in for, dar conditia de existenta
        este verificata la sfarsit */

        // parcurgere pana la penultimul element
        for (; p->urm != celula1; p = p->urm)
        {
            if (f(p->info, x) == 1)
                return p->info;
        }
        if (f(p->info, x) == 1) // trebuie verificat si ultimul element
            return p->info;
    }
    
    return NULL;
}