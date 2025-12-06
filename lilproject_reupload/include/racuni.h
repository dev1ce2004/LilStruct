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

typedef struct _Stat {
    char datum[11];  
    int kolicina;
    double ukupno;
    struct _Stat* next;
} Stat;

typedef struct _ArtStat {
    char naziv[MAX_NAZIV];
    int kolicina;
    double ukupno;
    struct _ArtStat* next;
} ArtStat;


int ubaciRacunSortirano(PozicijaRacuna head, char* datum, PozicijaArtikla artikli);
int citajListuRacuna(const char* fileName, PozicijaRacuna head);
int citajRacunDatoteku(const char* fileName, char* datum, PozicijaArtikla artikli);
int upitArtikl(PozicijaRacuna head, char* nazivArtikla, char* pocetniDatum, char* krajnjiDatum, int* ukupnaKolicina, double* ukupnoPlaceno);
int izvjestajSviArtikli(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum);
int ispisiRacunePoDatumu(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum);
int ispisCSV(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum, const char* fileName);
int upitArtiklNapredno(PozicijaRacuna head, char* nazivArtikla, char* pocetniDatum, char* krajnjiDatum, int minKolicina, int maxKolicina, double minCijena, double maxCijena, int* ukupnaKolicina, double* ukupnoPlaceno);
int naprednaStatistika(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum);
int naprednaStatistikaPM(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum);
int naprednaStatistikaPD(PozicijaRacuna head, char* pocetniDatum, char* krajnjiDatum);
int oslobodiRacune(PozicijaRacuna head);
int usporediDatume(const char* d1, const char* d2);
int validanDatum(const char* datum);

#endif
