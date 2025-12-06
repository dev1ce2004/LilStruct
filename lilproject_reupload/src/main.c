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
        printf("1. upit po artiklu\n");
        printf("2. izvjestaj svih artikala\n");
        printf("3. ispis racuna po datumu\n");
        printf("4. ispis izvj u CSV fileu\n");
        printf("5. napredni upit po artiklu\n");
        printf("6. stat y\n");
        printf("7. stat dan\n");
        printf("8. stat mnth\n");
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

            izvjestajSviArtikli(&headRacuna, pocetniDatum, krajnjiDatum);
        }
        else if (choice == 3) {
            char pocetniDatum[11], krajnjiDatum[11];
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            if (!validanDatum(pocetniDatum)) { printf("Neispravan datum!\n"); continue; }

            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            if (!validanDatum(krajnjiDatum)) { printf("Neispravan datum!\n"); continue; }

            ispisiRacunePoDatumu(&headRacuna, pocetniDatum, krajnjiDatum);
        }
        else if(choice == 4) {
            char pocetniDatum[11], krajnjiDatum[11], fileName[128];
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            if(!validanDatum(pocetniDatum)){ printf("Neispravan datum!\n"); continue; }
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            if(!validanDatum(krajnjiDatum)){ printf("Neispravan datum!\n"); continue; }

            printf("Unesi ime CSV filea: ");
            scanf("%s", fileName);

            ispisCSV(&headRacuna, pocetniDatum, krajnjiDatum, fileName);
        }
        else if(choice == 5) {
            char nazivArtikla[MAX_NAZIV], pocetniDatum[11], krajnjiDatum[11];
            int ukupnaKolicina;
            double ukupnoPlaceno;
            int minKolicina, maxKolicina;
            double minCijena, maxCijena;

            printf("Unesi naziv artikla: ");
            scanf("%s", nazivArtikla);
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            printf("Minimalna kolicina: ");
            scanf("%d", &minKolicina);
            printf("Maksimalna kolicina: ");
            scanf("%d", &maxKolicina);
            printf("Minimalna cijena: ");
            scanf("%lf", &minCijena);
            printf("Maksimalna cijena: ");
            scanf("%lf", &maxCijena);

            upitArtiklNapredno(&headRacuna, nazivArtikla, pocetniDatum, krajnjiDatum, minKolicina, maxKolicina, minCijena, maxCijena, &ukupnaKolicina, &ukupnoPlaceno);

            printf("Artikl: %s\nUkupna kolicina: %d\nUkupno placeno: %.2lf\n", nazivArtikla, ukupnaKolicina, ukupnoPlaceno);
        }     
        else if(choice == 6) {
            char pocetniDatum[11], krajnjiDatum[11];
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            if(!validanDatum(pocetniDatum)) { printf("Neispravan datum!\n"); continue; }
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            if(!validanDatum(krajnjiDatum)) { printf("Neispravan datum!\n"); continue; }

            naprednaStatistika(&headRacuna, pocetniDatum, krajnjiDatum);
        }
    else if(choice == 7) {
            char pocetniDatum[11], krajnjiDatum[11];
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            if(!validanDatum(pocetniDatum)) { printf("Neispravan datum!\n"); continue; }
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            if(!validanDatum(krajnjiDatum)) { printf("Neispravan datum!\n"); continue; }

            naprednaStatistikaPD(&headRacuna, pocetniDatum, krajnjiDatum);
        }
    else if(choice == 8) {
            char pocetniDatum[11], krajnjiDatum[11];
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf("%s", pocetniDatum);
            if(!validanDatum(pocetniDatum)) { printf("Neispravan datum!\n"); continue; }
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf("%s", krajnjiDatum);
            if(!validanDatum(krajnjiDatum)) { printf("Neispravan datum!\n"); continue; }

            naprednaStatistikaPM(&headRacuna, pocetniDatum, krajnjiDatum);
        }

    } while (choice != 0);

    oslobodiRacune(&headRacuna);
    return 0;
}
