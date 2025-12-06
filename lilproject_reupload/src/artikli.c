#include "artikli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ubaciArtiklSortirano(PozicijaArtikla head, char* naziv, int kolicina, double cijena)
{
    PozicijaArtikla prev = head;
    PozicijaArtikla trenutni = head->next;

    while (trenutni != NULL && strcmp(trenutni->naziv, naziv) < 0)
    {
        prev = trenutni;
        trenutni = trenutni->next;
    }

    if (trenutni != NULL && strcmp(trenutni->naziv, naziv) == 0)
    {
        trenutni->kolicina += kolicina;
        trenutni->cijena = cijena;
    }
    else
    {
        PozicijaArtikla noviArtikl = (PozicijaArtikla)malloc(sizeof(Artikl));
        if (!noviArtikl) return MALLOC_ERROR;
        strcpy(noviArtikl->naziv, naziv);
        noviArtikl->kolicina = kolicina;
        noviArtikl->cijena = cijena;
        noviArtikl->next = trenutni;
        prev->next = noviArtikl;
    }
    return SUCCESS;
}

int oslobodiArtikle(PozicijaArtikla head)
{
    PozicijaArtikla tmp = head->next;
    while (tmp)
    {
        PozicijaArtikla toDelete = tmp;
        tmp = tmp->next;
        free(toDelete);
    }
    head->next = NULL;
    return SUCCESS;
}
