#include "racuni.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


/*
ovo je pakal - 3:35 12/6/2025
*/

int min3(int a, int b, int c) {
    int m = a < b ? a : b;
    return m < c ? m : c;
}

int levenshtein(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    int **d = malloc((len1+1) * sizeof(int*));
    for(int i=0; i<=len1; i++)
        d[i] = malloc((len2+1) * sizeof(int));

    for(int i=0;i<=len1;i++) d[i][0] = i;
    for(int j=0;j<=len2;j++) d[0][j] = j;

    for(int i=1;i<=len1;i++){
        for(int j=1;j<=len2;j++){
            int cost = (tolower(s1[i-1]) == tolower(s2[j-1])) ? 0 : 1;
            d[i][j] = min3(d[i-1][j]+1, d[i][j-1]+1, d[i-1][j-1]+cost);
        }
    }

    int result = d[len1][len2];

    for(int i=0;i<=len1;i++)
        free(d[i]);
    free(d);

    return result;
}

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
                if (levenshtein(trenutniArtikl->naziv, nazivArtikla) <= 2)
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

int izvjestajSviArtikli(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum)
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
    if (!curr) {
        printf("Nema artikala u zadanom periodu.\n");
    } else {
        UkupniArtikl* maxArtikl = curr;
        while (curr) {
            printf("Artikl: %-20s | Kolicina: %-5d | Ukupno: %.2lf\n",
                   curr->naziv, curr->kolicina, curr->ukupno);

            if (curr->ukupno > maxArtikl->ukupno)
                maxArtikl = curr;

            curr = curr->next;
        }
        printf("\nNajprodavaniji artikl: %s, ukupno: %.2lf\n",
               maxArtikl->naziv, maxArtikl->ukupno);
    }

    curr = headArtikala;
    while (curr) {
        UkupniArtikl* tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    return SUCCESS;
}

int ispisiRacunePoDatumu(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum)
{
    PozicijaRacuna trenutniRacun = head->next;
    int found = 0;

    while (trenutniRacun)
    {
        if (usporediDatume(trenutniRacun->datum, pocetniDatum) >= 0 &&
            usporediDatume(trenutniRacun->datum, krajnjiDatum) <= 0)
        {
            found = 1;
            printf("\nRacun: %s\n", trenutniRacun->datum);
            PozicijaArtikla trenutniArtikl = trenutniRacun->artikli->next;
            while (trenutniArtikl)
            {
                printf("  Artikl: %-20s | Kolicina: %-5d | Cijena: %.2lf\n",
                       trenutniArtikl->naziv, trenutniArtikl->kolicina, trenutniArtikl->cijena);
                trenutniArtikl = trenutniArtikl->next;
            }
        }
        trenutniRacun = trenutniRacun->next;
    }

    if (!found)
        printf("Nema racuna u zadanom periodu.\n");

    return SUCCESS;
}

int ispisCSV(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum, const char* fileName)
{
    FILE* file = fopen(fileName, "w");
    if(!file) { printf("error u otvaranju CSV filea.\n"); return FILE_NOT_OPENED; }

    fprintf(file, "Naziv,Kolicina,Ukupno\n");

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
                while(curr && strcmp(curr->naziv, trenutniArtikl->naziv) != 0) { prev = curr; curr = curr->next; }

                if(curr) {
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

    UkupniArtikl* curr = headArtikala->next;
    while(curr) {
        fprintf(file, "%s,%d,%.2lf\n", curr->naziv, curr->kolicina, curr->ukupno);
        curr = curr->next;
    }

    fclose(file);

    curr = headArtikala;
    while(curr) {
        UkupniArtikl* tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    printf("Izvjestaj je sacuvan u %s\n", fileName);
    return SUCCESS;
}

int upitArtiklNapredno(PozicijaRacuna head, char* nazivArtikla, char* pocetniDatum, char* krajnjiDatum, int minKolicina, int maxKolicina, double minCijena, double maxCijena, int* ukupnaKolicina, double* ukupnoPlaceno)
{
    *ukupnaKolicina = 0;
    *ukupnoPlaceno = 0;

    PozicijaRacuna trenutniRacun = head->next;
    while(trenutniRacun)
    {
        if(usporediDatume(trenutniRacun->datum, pocetniDatum) >= 0 &&
           usporediDatume(trenutniRacun->datum, krajnjiDatum) <= 0)
        {
            PozicijaArtikla trenutniArtikl = trenutniRacun->artikli->next;
            while(trenutniArtikl)
            {
                int fuzzy = levenshtein(trenutniArtikl->naziv, nazivArtikla) <= 2;
                int kolicinaOK = trenutniArtikl->kolicina >= minKolicina && trenutniArtikl->kolicina <= maxKolicina;
                int cijenaOK = trenutniArtikl->cijena >= minCijena && trenutniArtikl->cijena <= maxCijena;

                if(fuzzy && kolicinaOK && cijenaOK)
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

int naprednaStatistikaPD(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum)
{
    Stat* listaDani = (Stat*)malloc(sizeof(Stat));
    listaDani->next = NULL;
    ArtStat* listaArtikala = (ArtStat*)malloc(sizeof(ArtStat));
    listaArtikala->next = NULL;

    PozicijaRacuna racun = head->next;

    while(racun)
    {
        if(usporediDatume(racun->datum, pocetniDatum) >= 0 &&
           usporediDatume(racun->datum, krajnjiDatum) <= 0)
        {
            int kolicinaD = 0;
            double ukupnoD = 0;

            PozicijaArtikla art = racun->artikli->next;
            while(art)
            {
                kolicinaD += art->kolicina;
                ukupnoD += art->kolicina * art->cijena;

                ArtStat* prev = listaArtikala;
                ArtStat* curr = listaArtikala->next;
                while(curr && strcmp(curr->naziv, art->naziv) != 0)
                {
                    prev = curr;
                    curr = curr->next;
                }
                if(curr)
                {
                    curr->kolicina += art->kolicina;
                    curr->ukupno += art->kolicina * art->cijena;
                }
                else
                {
                    ArtStat* novi = (ArtStat*)malloc(sizeof(ArtStat));
                    strcpy(novi->naziv, art->naziv);
                    novi->kolicina = art->kolicina;
                    novi->ukupno = art->kolicina * art->cijena;
                    novi->next = listaArtikala->next;
                    listaArtikala->next = novi;
                }

                art = art->next;
            }

            Stat* prevD = listaDani;
            Stat* currD = listaDani->next;
            while(currD && strcmp(currD->datum, racun->datum) != 0)
            {
                prevD = currD;
                currD = currD->next;
            }

            if(currD)
            {
                currD->kolicina += kolicinaD;
                currD->ukupno += ukupnoD;
            }
            else
            {
                Stat* noviD = (Stat*)malloc(sizeof(Stat));
                strcpy(noviD->datum, racun->datum);
                noviD->kolicina = kolicinaD;
                noviD->ukupno = ukupnoD;
                noviD->next = listaDani->next;
                listaDani->next = noviD;
            }
        }

        racun = racun->next;
    }

    Stat* curr = listaDani->next;
    printf("\n=== stat po danu ===\n");
    while(curr)
    {
        printf("%s | Kolicina: %d | Zarada: %.2lf\n", curr->datum, curr->kolicina, curr->ukupno);
        curr = curr->next;
    }

    while(listaDani) { Stat* tmp = listaDani; listaDani = listaDani->next; free(tmp); }
    while(listaArtikala) { ArtStat* tmp = listaArtikala; listaArtikala = listaArtikala->next; free(tmp); }

    return SUCCESS;
}

int naprednaStatistikaPM(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum)
{
    Stat* listaMj = (Stat*)malloc(sizeof(Stat));
    listaMj->next = NULL;
    ArtStat* listaArtikala = (ArtStat*)malloc(sizeof(ArtStat));
    listaArtikala->next = NULL;

    PozicijaRacuna racun = head->next;

    while(racun)
    {
        if(usporediDatume(racun->datum, pocetniDatum) >= 0 &&
           usporediDatume(racun->datum, krajnjiDatum) <= 0)
        {
            int kolicinaM = 0;
            double ukupnoM = 0;

            PozicijaArtikla art = racun->artikli->next;
            while(art)
            {
                kolicinaM += art->kolicina;
                ukupnoM += art->kolicina * art->cijena;

                ArtStat* prev = listaArtikala;
                ArtStat* curr = listaArtikala->next;
                while(curr && strcmp(curr->naziv, art->naziv) != 0)
                {
                    prev = curr;
                    curr = curr->next;
                }
                if(curr)
                {
                    curr->kolicina += art->kolicina;
                    curr->ukupno += art->kolicina * art->cijena;
                }
                else
                {
                    ArtStat* novi = (ArtStat*)malloc(sizeof(ArtStat));
                    strcpy(novi->naziv, art->naziv);
                    novi->kolicina = art->kolicina;
                    novi->ukupno = art->kolicina * art->cijena;
                    novi->next = listaArtikala->next;
                    listaArtikala->next = novi;
                }

                art = art->next;
            }

            char mjesec[8];
            strncpy(mjesec, racun->datum, 7);
            mjesec[7] = '\0';

            Stat* prevM = listaMj;
            Stat* currM = listaMj->next;
            while(currM && strcmp(currM->datum, mjesec) != 0)
            {
                prevM = currM;
                currM = currM->next;
            }

            if(currM)
            {
                currM->kolicina += kolicinaM;
                currM->ukupno += ukupnoM;
            }
            else
            {
                Stat* noviM = (Stat*)malloc(sizeof(Stat));
                strcpy(noviM->datum, mjesec);
                noviM->kolicina = kolicinaM;
                noviM->ukupno = ukupnoM;
                noviM->next = listaMj->next;
                listaMj->next = noviM;
            }
        }

        racun = racun->next;
    }

    printf("\n=== stat po mjesecu ===\n");
    Stat* curr = listaMj->next;
    while(curr)
    {
        printf("%s | Kolicina: %d | Zarada: %.2lf\n", curr->datum, curr->kolicina, curr->ukupno);
        curr = curr->next;
    }

    while(listaMj) { Stat* tmp = listaMj; listaMj = listaMj->next; free(tmp); }
    while(listaArtikala) { ArtStat* tmp = listaArtikala; listaArtikala = listaArtikala->next; free(tmp); }

    return SUCCESS;
}

int naprednaStatistika(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum)
{
    ArtStat* listaArtikala = (ArtStat*)malloc(sizeof(ArtStat));
    listaArtikala->next = NULL;

    Stat* listaGod = (Stat*)malloc(sizeof(Stat));
    listaGod->next = NULL;

    int ukupnaKolicina = 0;
    double ukupnoPlaceno = 0;

    PozicijaRacuna racun = head->next;
    while (racun)
    {
        if (usporediDatume(racun->datum, pocetniDatum) >= 0 &&
            usporediDatume(racun->datum, krajnjiDatum) <= 0)
        {
            PozicijaArtikla art = racun->artikli->next;
            while (art)
            {
                ukupnaKolicina += art->kolicina;
                ukupnoPlaceno += art->kolicina * art->cijena;

                ArtStat* prev = listaArtikala;
                ArtStat* curr = listaArtikala->next;
                while (curr && strcmp(curr->naziv, art->naziv) != 0)
                {
                    prev = curr;
                    curr = curr->next;
                }
                if (curr)
                {
                    curr->kolicina += art->kolicina;
                    curr->ukupno += art->kolicina * art->cijena;
                }
                else
                {
                    ArtStat* novi = (ArtStat*)malloc(sizeof(ArtStat));
                    strcpy(novi->naziv, art->naziv);
                    novi->kolicina = art->kolicina;
                    novi->ukupno = art->kolicina * art->cijena;
                    novi->next = listaArtikala->next;
                    listaArtikala->next = novi;
                }

                char godina[5];
                strncpy(godina, racun->datum, 4);
                godina[4] = '\0';

                Stat* prevG = listaGod;
                Stat* currG = listaGod->next;
                while (currG)
                {
                    char currGodina[5];
                    strncpy(currGodina, currG->datum, 4);
                    currGodina[4] = '\0';

                    if (strcmp(currGodina, godina) == 0)
                        break;

                    prevG = currG;
                    currG = currG->next;
                }

                if (currG)
                {
                    currG->kolicina += art->kolicina;
                    currG->ukupno += art->kolicina * art->cijena;
                }
                else
                {
                    Stat* noviG = (Stat*)malloc(sizeof(Stat));
                    strcpy(noviG->datum, racun->datum);
                    noviG->kolicina = art->kolicina;
                    noviG->ukupno = art->kolicina * art->cijena;
                    noviG->next = listaGod->next;
                    listaGod->next = noviG;
                }

                art = art->next;
            }
        }
        racun = racun->next;
    }

    printf("\n=== ukupna stat ===\n");
    printf("Ukupna kolicina: %d\n", ukupnaKolicina);
    printf("Ukupno placeno: %.2lf\n", ukupnoPlaceno);

    printf("\n=== Top 5 po zaradi ===\n");
    for (int i = 0; i < 5; i++)
    {
        ArtStat* curr = listaArtikala->next;
        ArtStat* max = NULL;
        while (curr)
        {
            if (!max || curr->ukupno > max->ukupno)
                max = curr;
            curr = curr->next;
        }
        if (max)
        {
            printf("%d. %s | Kolicina: %d | Ukupno: %.2lf\n", i + 1, max->naziv, max->kolicina, max->ukupno);
            ArtStat* prev = listaArtikala;
            curr = listaArtikala->next;
            while (curr && curr != max)
            {
                prev = curr;
                curr = curr->next;
            }
            if (curr)
                prev->next = curr->next;
            free(curr);
        }
        else
            break;
    }

    printf("\n=== Zarada po godini ===\n");
    Stat* currG = listaGod->next;
    while (currG)
    {
        char godina[5];
        strncpy(godina, currG->datum, 4);
        godina[4] = '\0';
        printf("Godina: %s | Ukupna kolicina: %d | Ukupno: %.2lf\n",
               godina, currG->kolicina, currG->ukupno);
        currG = currG->next;
    }

    while (listaGod)
    {
        Stat* tmp = listaGod;
        listaGod = listaGod->next;
        free(tmp);
    }
    while (listaArtikala)
    {
        ArtStat* tmp = listaArtikala;
        listaArtikala = listaArtikala->next;
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
