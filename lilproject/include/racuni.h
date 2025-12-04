#ifndef RACUNI_H
#define RACUNI_H

#include "artikli.h"

struct _Racun;
typedef struct _Racun* PozicijaRacuna;
typedef struct _Racun
{
    char datum[11];
    PozicijaArtikla artikli;
    PozicijaRacuna next;
} Racun;

int ubaciRacunSortirano(PozicijaRacuna head, char* datum, PozicijaArtikla artikli);
int citajListuRacuna(const char* fileName, PozicijaRacuna head);
int citajRacunDatoteku(const char* fileName, char* datum, PozicijaArtikla artikli);
int upitArtikl(PozicijaRacuna head, char* nazivArtikla, char* pocetniDatum, char* krajnjiDatum, int* ukupnaKolicina, double* ukupnoPlaceno);
int izvjestajSveArtikle(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum);
int oslobodiRacune(PozicijaRacuna head);
int usporediDatume(const char* d1, const char* d2);
int validanDatum(const char* datum);

#endif
