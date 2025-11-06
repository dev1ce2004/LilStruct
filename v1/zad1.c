#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_BODOVA 30

typedef struct {
    char ime[MAX_NAME];
    char prezime[MAX_NAME];
    int bodovi;
} Student;

int brojRedakaUDatoteci(const char* imeDatoteke);
Student* ucitajStudente(const char* imeDatoteke, int brojStudenata);
void ispisiStudente(Student* studenti, int brojStudenata);

int main() {
    const char* imeDatoteke = "students.txt";
    int brojStudenata = brojRedakaUDatoteci(imeDatoteke);
    
    if (brojStudenata == 0) {
        printf("Datoteka je prazna ili ne postoji.\n");
        return 1;
    }

    Student* studenti = ucitajStudente(imeDatoteke, brojStudenata);
    
    if (studenti == NULL) {
        printf("Greska pri alokaciji memorije ili citanju datoteke.\n");
        return 1;
    }

    ispisiStudente(studenti, brojStudenata);
    
    free(studenti);
    return 0;
}

int brojRedakaUDatoteci(const char* imeDatoteke) {
    FILE* datoteka = fopen(imeDatoteke, "r");
    if (!datoteka) {
        printf("Greska pri otvaranju datoteke.\n");
        return 0;
    }
    
    int brojac = 0;
    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), datoteka) != NULL) {
        brojac++;
    }
    
    fclose(datoteka);
    return brojac;
}

Student* ucitajStudente(const char* imeDatoteke, int brojStudenata) {
    FILE* datoteka = fopen(imeDatoteke, "r");
    if (!datoteka) {
        printf("Greska pri otvaranju datoteke.\n");
        return NULL;
    }
    
    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
    if (!studenti) {
        printf("Greska pri alokaciji memorije.\n");
        fclose(datoteka);
        return NULL;
    }
    
    for (int i = 0; i < brojStudenata; i++) {
        fscanf(datoteka, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }
    
    fclose(datoteka);
    return studenti;
}

void ispisiStudente(Student* studenti, int brojStudenata) {
    for (int i = 0; i < brojStudenata; i++) {
        double relativanBodovi = (double)studenti[i].bodovi / MAX_BODOVA * 100;
        printf("%s %s - Apsolutni bodovi: %d, Relativni bodovi: %.2f%%\n",
               studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativanBodovi);
    }
}
