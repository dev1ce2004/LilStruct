#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAZIV 64
#define FILE_NOT_OPENED (-1)
#define SUCCESS 0
#define MALLOC_ERROR (-2)

struct _Artikl;
typedef struct _Artikl* PozicijaArtikla;
typedef struct _Artikl
{
    char naziv[MAX_NAZIV];
    int kolicina;
    double cijena;
    PozicijaArtikla next;
} Artikl;

struct _Racun;
typedef struct _Racun* PozicijaRacuna;
typedef struct _Racun
{
    char datum[11];
    PozicijaArtikla artikli;
    PozicijaRacuna next;
} Racun;

int ubaciArtiklSortirano(PozicijaArtikla head, char* naziv, int kolicina, double cijena);
int ubaciRacunSortirano(PozicijaRacuna head, char* datum, PozicijaArtikla artikli);
int citajListuRacuna(const char* fileName, PozicijaRacuna head);
int citajRacunDatoteku(const char* fileName, char* datum, PozicijaArtikla artikli);
int upitArtikl(PozicijaRacuna head, char* nazivArtikla, char* pocetniDatum, char* krajnjiDatum, int* ukupnaKolicina, double* ukupnoPlaceno);
int oslobodiArtikle(PozicijaArtikla head);
int oslobodiRacune(PozicijaRacuna head);
int usporediDatume(const char* d1, const char* d2);

int main()
{
    Racun headRacuna = { "", NULL, NULL };
    char nazivArtikla[MAX_NAZIV];
    char pocetniDatum[11], krajnjiDatum[11];
    int ukupnaKolicina = 0;
    double ukupnoPlaceno = 0;

    if (citajListuRacuna("racuni.txt", &headRacuna) != SUCCESS)
    {
        printf("Greska pri citanju liste racuna.\n");
        return FILE_NOT_OPENED;
    }

    printf("Unesi naziv artikla: ");
    scanf("%s", nazivArtikla);
    printf("Unesi pocetni datum (YYYY-MM-DD): ");
    scanf("%s", pocetniDatum);
    printf("Unesi krajnji datum (YYYY-MM-DD): ");
    scanf("%s", krajnjiDatum);

    if (upitArtikl(&headRacuna, nazivArtikla, pocetniDatum, krajnjiDatum, &ukupnaKolicina, &ukupnoPlaceno) != SUCCESS)
    {
        printf("Greska pri izvrsavanju upita.\n");
    }
    else
    {
        printf("Artikl: %s\nUkupna kolicina: %d\nUkupno placeno: %.2lf\n", nazivArtikla, ukupnaKolicina, ukupnoPlaceno);
    }

    oslobodiRacune(&headRacuna);
    return 0;
}

int usporediDatume(const char* d1, const char* d2)
{
    return strcmp(d1, d2);
}

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
