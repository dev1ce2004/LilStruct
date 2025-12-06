#include "racuni.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int usporediDatume(const char* d1, const char* d2)
{
    return strcmp(d1, d2);
}

int validanDatum(const char* datum)
{
    if (strlen(datum) != 10) return 0;
    if (datum[4] != '-' || datum[7] != '-') return 0;
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7) continue;
        if (datum[i] < '0' || datum[i] > '9') return 0;
    }
    return 1;
}

int ubaciRacunSortirano(PozicijaRacuna head, char* datum, PozicijaArtikla artikli)
{
    PozicijaRacuna prev = head;
    PozicijaRacuna trenutni = head->next;

    while (trenutni != NULL && usporediDatume(trenutni->datum, datum) < 0)
    {
        prev = trenutni;
        trenutni = trenutni->next;
    }

    PozicijaRacuna noviRacun = (PozicijaRacuna)malloc(sizeof(Racun));
    if (!noviRacun) return MALLOC_ERROR;
    strcpy(noviRacun->datum, datum);
    noviRacun->artikli = artikli;
    noviRacun->next = trenutni;
    prev->next = noviRacun;

    return SUCCESS;
}

int citajListuRacuna(const char* fileName, PozicijaRacuna head)
{
    FILE* file = fopen(fileName, "r");
    if (!file) return FILE_NOT_OPENED;

    char nazivDatoteke[MAX_LINE];
    while (fscanf(file, "%s", nazivDatoteke) != EOF)
    {
        char datum[11];
        PozicijaArtikla headArtikala = (PozicijaArtikla)malloc(sizeof(Artikl));
        if (!headArtikala) return MALLOC_ERROR;
        headArtikala->next = NULL;

        if (citajRacunDatoteku(nazivDatoteke, datum, headArtikala) != SUCCESS)
        {
            fclose(file);
            return -3;
        }

        ubaciRacunSortirano(head, datum, headArtikala);
    }

    fclose(file);
    return SUCCESS;
}

int citajRacunDatoteku(const char* fileName, char* datum, PozicijaArtikla artikli)
{
    FILE* file = fopen(fileName, "r");
    if (!file) return FILE_NOT_OPENED;

    if (fgets(datum, 11, file) == NULL) { fclose(file); return -2; }
    datum[10] = '\0';

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file))
    {
        char naziv[MAX_NAZIV];
        int kolicina;
        double cijena;
        if (sscanf(line, "%[^,],%d,%lf", naziv, &kolicina, &cijena) == 3)
        {
            ubaciArtiklSortirano(artikli, naziv, kolicina, cijena);
        }
    }

    fclose(file);
    return SUCCESS;
}

int upitArtikl(PozicijaRacuna head, char* nazivArtikla, char* pocetniDatum, char* krajnjiDatum, int* ukupnaKolicina, double* ukupnoPlaceno)
{
    *ukupnaKolicina = 0;
    *ukupnoPlaceno = 0;

    PozicijaRacuna trenutniRacun = head->next;
    while (trenutniRacun)
    {
        if (usporediDatume(trenutniRacun->datum, pocetniDatum) >= 0 &&
            usporediDatume(trenutniRacun->datum, krajnjiDatum) <= 0)
        {
            PozicijaArtikla trenutniArtikl = trenutniRacun->artikli->next;
            while (trenutniArtikl)
            {
                if (strcmp(trenutniArtikl->naziv, nazivArtikla) == 0)
                {
                    *ukupnaKolicina += trenutniArtikl->kolicina;
                    *ukupnoPlaceno += trenutniArtikl->kolicina * trenutniArtikl->cijena;
                }
                trenutniArtikl = trenutniArtikl->next;
            }
        }
        trenutniRacun = trenutniRacun->next;
    }
    return SUCCESS;
}

int izvjestajSveArtikle(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum)
{
    typedef struct _UkupniArtikl {
        char naziv[MAX_NAZIV];
        int kolicina;
        double ukupno;
        struct _UkupniArtikl* next;
    } UkupniArtikl;

    UkupniArtikl* headArtikala = (UkupniArtikl*)malloc(sizeof(UkupniArtikl));
    headArtikala->next = NULL;

    PozicijaRacuna trenutniRacun = head->next;
    while (trenutniRacun) {
        if (usporediDatume(trenutniRacun->datum, pocetniDatum) >= 0 &&
            usporediDatume(trenutniRacun->datum, krajnjiDatum) <= 0) {

            PozicijaArtikla trenutniArtikl = trenutniRacun->artikli->next;
            while (trenutniArtikl) {
                UkupniArtikl* prev = headArtikala;
                UkupniArtikl* curr = headArtikala->next;
                while (curr && strcmp(curr->naziv, trenutniArtikl->naziv) != 0) {
                    prev = curr;
                    curr = curr->next;
                }
                if (curr) {
                    curr->kolicina += trenutniArtikl->kolicina;
                    curr->ukupno += trenutniArtikl->kolicina * trenutniArtikl->cijena;
                } else {
                    UkupniArtikl* novi = (UkupniArtikl*)malloc(sizeof(UkupniArtikl));
                    strcpy(novi->naziv, trenutniArtikl->naziv);
                    novi->kolicina = trenutniArtikl->kolicina;
                    novi->ukupno = trenutniArtikl->kolicina * trenutniArtikl->cijena;
                    novi->next = headArtikala->next;
                    headArtikala->next = novi;
                }
                trenutniArtikl = trenutniArtikl->next;
            }
        }
        trenutniRacun = trenutniRacun->next;
    }

    printf("\n=== Izvjestaj svih artikala ===\n");
    UkupniArtikl* curr = headArtikala->next;
    UkupniArtikl* maxArtikl = curr;

    while (curr) {
        printf("%-20s | ", curr->naziv);
        int bar = (int)(curr->ukupno / 10);
        for (int i = 0; i < bar; i++) printf("#");
        printf(" %.2lf\n", curr->ukupno);

        if (curr->ukupno > maxArtikl->ukupno)
            maxArtikl = curr;

        curr = curr->next;
    }

    printf("\nNajprodavaniji artikl: %s, ukupno %.2lf\n", maxArtikl->naziv, maxArtikl->ukupno);

    // oslobađanje memorije
    curr = headArtikala;
    while (curr) {
        UkupniArtikl* tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    return SUCCESS;
}

int oslobodiRacune(PozicijaRacuna head)
{
    PozicijaRacuna trenutni = head->next;
    while (trenutni)
    {
        PozicijaRacuna toDelete = trenutni;
        trenutni = trenutni->next;
        oslobodiArtikle(toDelete->artikli);
        free(toDelete->artikli);
        free(toDelete);
    }
    head->next = NULL;
    return SUCCESS;
}
