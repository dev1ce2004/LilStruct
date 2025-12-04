#include "racuni.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
PS C:\Users\User\Desktop\lilproject> gcc -Iinclude src/main.c src/racuni.c src/artikli.c -o lilproject.exe 
PS C:\Users\User\Desktop\lilproject> .\lilproject.exe
*/
int main()
{
    Racun headRacuna = { "", NULL, NULL };
    headRacuna.next = NULL;

    if (citajListuRacuna("data/racuni.txt", &headRacuna) != SUCCESS)
    {
        printf("Greska pri citanju liste racuna.\n");
        return FILE_NOT_OPENED;
    }

    int choice;
    do {
        printf("\n=== MENI ===\n");
        printf("1. Upit po artiklu\n");
        printf("2. Izvjestaj svih artikala\n");
        printf("0. Izlaz\n");
        printf("Izaberite opciju: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char nazivArtikla[MAX_NAZIV];
            char pocetniDatum[11], krajnjiDatum[11];
            int ukupnaKolicina = 0;
            double ukupnoPlaceno = 0;

            printf("Unesi naziv artikla: ");
            scanf("%s", nazivArtikla);
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            if (!validanDatum(pocetniDatum)) { printf("Neispravan datum!\n"); continue; }
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            if (!validanDatum(krajnjiDatum)) { printf("Neispravan datum!\n"); continue; }

            upitArtikl(&headRacuna, nazivArtikla, pocetniDatum, krajnjiDatum, &ukupnaKolicina, &ukupnoPlaceno);
            printf("Artikl: %s\nUkupna kolicina: %d\nUkupno placeno: %.2lf\n", nazivArtikla, ukupnaKolicina, ukupnoPlaceno);
        }
        else if (choice == 2) {
            char pocetniDatum[11], krajnjiDatum[11];
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            if (!validanDatum(pocetniDatum)) { printf("Neispravan datum!\n"); continue; }
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            if (!validanDatum(krajnjiDatum)) { printf("Neispravan datum!\n"); continue; }

            izvjestajSveArtikle(&headRacuna, pocetniDatum, krajnjiDatum);
        }

    } while (choice != 0);

    oslobodiRacune(&headRacuna);
    return 0;
}
