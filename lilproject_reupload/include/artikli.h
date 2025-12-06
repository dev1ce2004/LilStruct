#ifndef ARTIKLI_H
#define ARTIKLI_H

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

int ubaciArtiklSortirano(PozicijaArtikla head, char* naziv, int kolicina, double cijena);
int oslobodiArtikle(PozicijaArtikla head);

#endif
