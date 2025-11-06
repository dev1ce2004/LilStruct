#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50
#define SUCCESS 0
#define ERROR_MEMORY_ALLOCATION -1
#define ERROR_NOT_FOUND -2
#define ERROR_FILE -3

typedef struct _Person* Position;
typedef struct _Person {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    int birthYear;
    Position next;
} Person;

// Funkcija za stvaranje nove osobe
Position createPerson(const char* firstName, const char* lastName, int birthYear) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) return NULL;

    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

// Dodavanje na pocetak liste
int addToStart(Position* head, const char* firstName, const char* lastName, int birthYear) {
    Position newPerson = createPerson(firstName, lastName, birthYear);
    if (!newPerson) return ERROR_MEMORY_ALLOCATION;

    newPerson->next = *head;
    *head = newPerson;
    return SUCCESS;
}

// Dodavanje na kraj liste
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

// Pronalazak osobe po prezimenu
Position findByLastName(Position head, const char* lastName) {
    while (head) {
        if (strcmp(head->lastName, lastName) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

// Pronalazak prethodnog elementa
Position findPrevious(Position head, const char* lastName) {
    if (!head) return NULL;
    while (head->next) {
        if (strcmp(head->next->lastName, lastName) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

// Dodavanje iza elementa
int addAfter(Position head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    Position target = findByLastName(head, targetLastName);
    if (!target) return ERROR_NOT_FOUND;

    Position newPerson = createPerson(firstName, lastName, birthYear);
    if (!newPerson) return ERROR_MEMORY_ALLOCATION;

    newPerson->next = target->next;
    target->next = newPerson;
    return SUCCESS;
}

// Dodavanje ispred elementa
int addBefore(Position* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    if (!head || !*head) return ERROR_NOT_FOUND;

    if (strcmp((*head)->lastName, targetLastName) == 0) {
        return addToStart(head, firstName, lastName, birthYear);
    }

    Position previous = findPrevious(*head, targetLastName);
    if (!previous) return ERROR_NOT_FOUND;

    Position newPerson = createPerson(firstName, lastName, birthYear);
    if (!newPerson) return ERROR_MEMORY_ALLOCATION;

    newPerson->next = previous->next;
    previous->next = newPerson;
    return SUCCESS;
}

// Brisanje elementa
int deletePerson(Position* head, const char* lastName) {
    if (!head || !*head) return ERROR_NOT_FOUND;

    Position previous = findPrevious(*head, lastName);
    Position toDelete = NULL;

    if (!previous) {
        if (strcmp((*head)->lastName, lastName) == 0) {
            toDelete = *head;
            *head = (*head)->next;
        } else {
            return ERROR_NOT_FOUND;
        }
    } else {
        toDelete = previous->next;
        previous->next = toDelete->next;
    }

    free(toDelete);
    return SUCCESS;
}

// Ispis liste
int printList(Position head) {
    if (!head) {
        printf("Lista je prazna.\n");
        return 0;
    }
    printf("\n--- LISTA OSOBA ---\n");
    while (head) {
        printf("%s %s, %d\n", head->firstName, head->lastName, head->birthYear);
        head = head->next;
    }
    printf("-------------------\n");
    return 0;
}

// Sortiranje po prezimenu
int sortByLastName(Position head) {
    if (!head) return 0;

    Position i, j;
    char tempFirst[MAX_LENGTH], tempLast[MAX_LENGTH];
    int tempYear;

    for (i = head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->lastName, j->lastName) > 0) {
                strcpy(tempFirst, i->firstName);
                strcpy(tempLast, i->lastName);
                tempYear = i->birthYear;

                strcpy(i->firstName, j->firstName);
                strcpy(i->lastName, j->lastName);
                i->birthYear = j->birthYear;

                strcpy(j->firstName, tempFirst);
                strcpy(j->lastName, tempLast);
                j->birthYear = tempYear;
            }
        }
    }
    return 0;
}

// Upis u datoteku
int writeToFile(Position head, const char* fileName) {
    FILE* file = fopen(fileName, "w");
    if (!file) return ERROR_FILE;

    while (head) {
        fprintf(file, "%s %s %d\n", head->firstName, head->lastName, head->birthYear);
        head = head->next;
    }
    fclose(file);
    return SUCCESS;
}

// Citanje iz datoteke
int readFromFile(Position* head, const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) return ERROR_FILE;

    char first[MAX_LENGTH], last[MAX_LENGTH];
    int year;

    while (fscanf(file, "%s %s %d", first, last, &year) == 3) {
        addToEnd(head, first, last, year);
    }

    fclose(file);
    return SUCCESS;
}

// Oslobadanje memorije
int freeList(Position* head) {
    Position temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    return 0;
}

int main() {
    Position head = NULL;
    int choice = -1;
    char firstName[MAX_LENGTH], lastName[MAX_LENGTH], targetLastName[MAX_LENGTH], fileName[100];
    int birthYear;
    char buffer[256];

    do {
        printf("\n--- MENU ---\n");
        printf("1. Dodaj na pocetak\n");
        printf("2. Dodaj na kraj\n");
        printf("3. Dodaj iza elementa\n");
        printf("4. Dodaj ispred elementa\n");
        printf("5. Ispisi listu\n");
        printf("6. Sortiraj po prezimenima\n");
        printf("7. Upisi listu u datoteku\n");
        printf("8. Ucitaj listu iz datoteke\n");
        printf("9. Obrisi element\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");

        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesite ime, prezime i godinu: ");
            fgets(buffer, sizeof(buffer), stdin);
            sscanf(buffer, "%s %s %d", firstName, lastName, &birthYear);
            addToStart(&head, firstName, lastName, birthYear);
            break;
        case 2:
            printf("Unesite ime, prezime i godinu: ");
            fgets(buffer, sizeof(buffer), stdin);
            sscanf(buffer, "%s %s %d", firstName, lastName, &birthYear);
            addToEnd(&head, firstName, lastName, birthYear);
            break;
        case 3:
            printf("Iza kojeg prezimena zelite dodati? ");
            fgets(targetLastName, sizeof(targetLastName), stdin);
            targetLastName[strcspn(targetLastName, "\n")] = 0;
            printf("Unesite ime, prezime i godinu nove osobe: ");
            fgets(buffer, sizeof(buffer), stdin);
            sscanf(buffer, "%s %s %d", firstName, lastName, &birthYear);
            if (addAfter(head, targetLastName, firstName, lastName, birthYear) != SUCCESS)
                printf("Prezime '%s' nije pronadeno.\n", targetLastName);
            break;
        case 4:
            printf("Ispred kojeg prezimena zelite dodati? ");
            fgets(targetLastName, sizeof(targetLastName), stdin);
            targetLastName[strcspn(targetLastName, "\n")] = 0;
            printf("Unesite ime, prezime i godinu nove osobe: ");
            fgets(buffer, sizeof(buffer), stdin);
            sscanf(buffer, "%s %s %d", firstName, lastName, &birthYear);
            if (addBefore(&head, targetLastName, firstName, lastName, birthYear) != SUCCESS)
                printf("Prezime '%s' nije pronadeno.\n", targetLastName);
            break;
        case 5:
            printList(head);
            break;
        case 6:
            sortByLastName(head);
            printf("Lista sortirana po prezimenima.\n");
            break;
        case 7:
            printf("Ime datoteke: ");
            fgets(fileName, sizeof(fileName), stdin);
            fileName[strcspn(fileName, "\n")] = 0;
            if (writeToFile(head, fileName) != SUCCESS)
                printf("Nekaj ti nece.\n");
            break;
        case 8:
            printf("Ime datoteke: ");
            fgets(fileName, sizeof(fileName), stdin);
            fileName[strcspn(fileName, "\n")] = 0;
            if (readFromFile(&head, fileName) != SUCCESS)
                printf("Nekaj ti nece.\n");
            break;
        case 9:
            printf("Unesite prezime elementa za brisanje: ");
            fgets(targetLastName, sizeof(targetLastName), stdin);
            targetLastName[strcspn(targetLastName, "\n")] = 0;
            if (deletePerson(&head, targetLastName) != SUCCESS)
                printf("Element '%s' nije pronaden.\n", targetLastName);
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
