#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50
#define SUCCESS 0
#define ERROR_MEMORY_ALLOCATION -1
#define ERROR_NOT_FOUND -2

struct _Person;
typedef struct _Person* Position;

typedef struct _Person {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    int birthYear;
    Position next;
} Person;

// Deklaracije funkcija
int addToStart(Position* head, const char* firstName, const char* lastName, int birthYear);
int addToEnd(Position* head, const char* firstName, const char* lastName, int birthYear);
int printList(Position head);
Position findByLastName(Position head, const char* lastName);
int deletePerson(Position* head, const char* lastName);
int freeList(Position* head);
Position createPerson(const char* firstName, const char* lastName, int birthYear);

// Funkcija za kreiranje nove osobe
Position createPerson(const char* firstName, const char* lastName, int birthYear) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) return NULL;

    strncpy(newPerson->firstName, firstName, MAX_LENGTH - 1);
    newPerson->firstName[MAX_LENGTH - 1] = '\0';

    strncpy(newPerson->lastName, lastName, MAX_LENGTH - 1);
    newPerson->lastName[MAX_LENGTH - 1] = '\0';

    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

// Dodavanje osobe na početak liste
int addToStart(Position* head, const char* firstName, const char* lastName, int birthYear) {
    Position newPerson = createPerson(firstName, lastName, birthYear);
    if (!newPerson) return ERROR_MEMORY_ALLOCATION;

    newPerson->next = *head;
    *head = newPerson;
    return SUCCESS;
}

// Dodavanje osobe na kraj liste
int addToEnd(Position* head, const char* firstName, const char* lastName, int birthYear) {
    Position newPerson = createPerson(firstName, lastName, birthYear);
    if (!newPerson) return ERROR_MEMORY_ALLOCATION;

    if (!*head) {
        *head = newPerson;
        return SUCCESS;
    }

    Position temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = newPerson;
    return SUCCESS;
}

// Ispis liste osoba
int printList(Position head) {
    if (!head) {
        printf("\nLista je prazna.\n");
        return 0;
    }

    printf("\n--- LISTA OSOBA ---\n");
    Position temp = head;
    while (temp) {
        printf("%s %s, %d\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
    printf("-------------------\n");
    return 0;
}

// Pronalazak osobe po prezimenu
Position findByLastName(Position head, const char* lastName) {
    Position temp = head;
    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Brisanje osobe iz liste po prezimenu
int deletePerson(Position* head, const char* lastName) {
    if (!head || !*head) return ERROR_NOT_FOUND;

    Position temp = *head;
    Position previous = NULL;

    while (temp && strcmp(temp->lastName, lastName) != 0) {
        previous = temp;
        temp = temp->next;
    }

    if (!temp) return ERROR_NOT_FOUND;

    if (!previous)
        *head = temp->next;
    else
        previous->next = temp->next;

    free(temp);
    return SUCCESS;
}

// Oslobađanje memorije liste
int freeList(Position* head) {
    if (!head) return ERROR_NOT_FOUND;

    Position temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    return SUCCESS;
}

int main() {
    Position head = NULL;
    int choice = -1;
    char firstName[MAX_LENGTH] = {0}, lastName[MAX_LENGTH] = {0}, buffer[256];
    int birthYear = 0;

    do {
        printf("\n--- MENI ---\n");
        printf("1. Dodaj na pocetak\n");
        printf("2. Dodaj na kraj\n");
        printf("3. Ispisi listu\n");
        printf("4. Pronadji po prezimenu\n");
        printf("5. Obrisi po prezimenu\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");

        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        if (sscanf(buffer, "%d", &choice) != 1) continue;

        switch (choice) {
        case 1:
            printf("Unesite ime, prezime i godinu rodjenja:\n> ");
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            if (sscanf(buffer, "%49s %49s %d", firstName, lastName, &birthYear) != 3) break;
            if (addToStart(&head, firstName, lastName, birthYear) != SUCCESS)
                printf("Nekaj ti nece.\n");
            break;

        case 2:
            printf("Unesite ime, prezime i godinu rodjenja:\n> ");
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            if (sscanf(buffer, "%49s %49s %d", firstName, lastName, &birthYear) != 3) break;
            if (addToEnd(&head, firstName, lastName, birthYear) != SUCCESS)
                printf("Nekaj ti nece.\n");
            break;

        case 3:
            printList(head);
            break;

        case 4:
            printf("Unesite prezime osobe za pretragu:\n> ");
            if (!fgets(lastName, sizeof(lastName), stdin)) break;
            lastName[strcspn(lastName, "\n")] = 0;
            {
                Position found = findByLastName(head, lastName);
                if (found)
                    printf("Pronadjena osoba: %s %s, %d\n", found->firstName, found->lastName, found->birthYear);
                else
                    printf("Osoba s prezimenom '%s' nije pronadena.\n", lastName);
            }
            break;

        case 5:
            printf("Unesite prezime osobe za brisanje:\n> ");
            if (!fgets(lastName, sizeof(lastName), stdin)) break;
            lastName[strcspn(lastName, "\n")] = 0;
            if (deletePerson(&head, lastName) == SUCCESS)
                printf("Osoba s prezimenom '%s' je izbrisana.\n", lastName);
            else
                printf("Osoba s prezimenom '%s' nije pronadena.\n", lastName);
            break;

        case 0:
            printf("Adieu.\n");
            break;

        default:
            printf("Nekaj ti nece.\n");
        }

    } while (choice != 0);

    freeList(&head);
    return 0;
}
