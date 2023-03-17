/* FRANT Madalina - 315CB */

#include <string.h>
#include "headerTH.h"

typedef struct 
{
    char *Name; // Key
    char *IP;   // Value
} DNS;

/* functia calculeaza restul impartirii sumei caracterelor
ce fac parte din Key la numarul maxim de bucket-uri ce
pot fi stocate in tabela hash */
int codHash(void *elem, int M)
{
    int suma = 0;
    DNS* dns = (DNS *)elem;
    char *x = dns->Name;
    for (; *x != '\0'; x++)
        suma += *x;
    return (suma % M);
}

/* afiseaza valoarea Value a unui element */
void afi_IP(FILE *f, void *elem)
{
	DNS *dns = (DNS *)elem;
	fprintf(f, "%s ", dns->IP);
}

/* functie de eliberare specifica structurii de tip DNS */
void elib_DNS(void *x)
{
    DNS *p = (DNS *)x;
    free(p->Name);
    free(p->IP);
    free(p);
}

/* returneaza 1 daca elementele sunt egale, 0 in caz contrar */
int cmp(void *e1, void *e2)
{
	DNS *d1 = (DNS *)e1;
	DNS *d2 = (DNS *)e2;

	if (strcmp(d1->Name, d2->Name) != 0) // valoarea Key difera
		return 0;

	if (strcmp(d1->IP, d2->IP) != 0) // valoarea Value difera
		return 0;

	return 1;
}

/* compara cheia unui element de tip DNS cu cheia primita drept al
doilea parametru; returneaza 1 daca sunt egale, 0 in caz contrar */
int cmp_key(void *x, void *key)
{
    char *e1 = ((DNS*)x)->Name;
    char *e2 = (char *)key;

    if (strcmp(e1, e2) == 0)
        return 1;
    else
        return 0;
}

/* compara 2 elemente pentru a afla ce element trebuie inserat 
primul: returneaza 1 daca trebuie inserat e1, respectiv 2 - e2;
functie necesara implementarii inserarii ordonate (dupa Key) */
int cmp_ord(void *e1, void *e2)
{
    DNS * d1 = (DNS *) e1;
	DNS * d2 = (DNS *) e2;

	if (strcmp(d1->Name, d2->Name) < 0)
		return 1; // trebuie inserat mai intai e1
    else
        return 2; // trebuie inserat mai intai e2
}

int main(int argc, char *argv[])
{
    FILE *input, *output;
    size_t len = 0;
    char *line = NULL;

    // numarul maxim de bucket-uri ce pot fi stocate in tabela hash
    int M = atoi(argv[1]);

    TH *h = (TH *) IniTH(M, codHash);
    if (h == NULL)
        return 0;

    input = fopen(argv[2], "r");
    if (input == NULL)
        return 0;
    output = fopen(argv[3], "w");
    if (output == NULL)
        return 0;

    // citeste fiecare linie din fisier
    while (getline(&line, &len, input) != -1)
    {
        if (line[0] == '\n') // daca linia contine doar '\n'
            continue;

        char *operatie = strtok(line, " ");
        if (operatie[strlen(operatie) - 1] == '\n')
            operatie[strlen(operatie) - 1] = '\0';

        /* print */
        if (strcmp(operatie, "print") == 0)
        {
            AfiTH(output, h, afi_IP); // afiseaza tabela hash
            continue;
        }

        char *arg2 = strtok(NULL, " ");
        if (arg2[strlen(arg2) - 1] == '\n')
            arg2[strlen(arg2) - 1] = '\0';

        /* print_bucket <index_bucket> */
        if (strcmp(operatie, "print_bucket") == 0)
        {
            int index = atoi(arg2);
            // daca indexul nu depaseste numarul maxim de bucket-uri
            if (index < M)
            {
                // afisare bucket corespunzator
                TLista L = h->v[index];
                AfisareL(output, L, afi_IP);
                fprintf(output, "\n");
            }
        }

        /* get <Key> */
        if (strcmp(operatie, "get") == 0)
        {
            char *key = arg2; 
            void *value = Get(h, key, cmp_key);
            if (value == NULL)  // daca Key nu exista
                fprintf(output, "NULL\n");
            else                // daca exista
                fprintf(output, "%s\n", ((DNS *)value)->IP);
        }

        /* find <Key> */
        if (strcmp(operatie, "find") == 0)
        {
            char *key = arg2;
            void *value = Get(h, key, cmp_key);
            if (value == NULL)  // daca Key nu exista
                fprintf(output, "False\n");
            else                // daca exista
                fprintf(output, "True\n");
        }

        /* remove <Key> */
        if (strcmp(operatie, "remove") == 0)
        {
            char *key = arg2;
            void *value = Get(h, key, cmp_key);
            if (value != NULL) // daca Key exista
                ExtrTH(h, key, cmp_key, elib_DNS); // elimina elementul
        }

        /* put <Key Value> */
        if (strcmp(operatie, "put") == 0)
        {
            char *key = arg2; 
            char *value = strtok(NULL, " ");
            if (value[strlen(value) - 1] == '\n')
                value[strlen(value) - 1] = '\0';
            
            DNS *elem = (DNS *)malloc(sizeof(DNS));
            elem->Name = malloc(strlen(key) * sizeof(char) + 1);
            elem->IP = malloc(strlen(value) * sizeof(char) + 1);
            strcpy(elem->Name, key);
            strcpy(elem->IP, value);

            InsTH(h, elem, cmp, cmp_ord, elib_DNS); // insereaza elementul
        }
    }
    free(line);

    DistrTH(&h, elib_DNS); // eliberare tabela hash

    fclose(input);
    fclose(output);

    return 0;
}