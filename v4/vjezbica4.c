#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024
#define FILE_NOT_OPENED (-1)
#define SUCCESS 0

struct _Term;
typedef struct _Term* Position;
typedef struct _Term
{
    int coefficient;
    int exponent;
    Position next;
} Term;

int readPolynomial(const char* fileName, Position head);
int addPolynomial(Position p1, Position p2, Position result);
int multiplyPolynomial(Position p1, Position p2, Position result);
int insertTerm(Position head, int coefficient, int exponent);
void printPolynomial(Position head);
void freePolynomial(Position head);

int main()
{
    Term poly1Head = { 0, 0, NULL };
    Term poly2Head = { 0, 0, NULL };
    Term sumHead = { 0, 0, NULL };
    Term productHead = { 0, 0, NULL };

    if (readPolynomial("poly1.txt", &poly1Head) != SUCCESS)
        return FILE_NOT_OPENED;
    if (readPolynomial("poly2.txt", &poly2Head) != SUCCESS)
        return FILE_NOT_OPENED;

    addPolynomial(&poly1Head, &poly2Head, &sumHead);
    multiplyPolynomial(&poly1Head, &poly2Head, &productHead);

    printf("Suma: ");
    printPolynomial(&sumHead);

    printf("Produkt: ");
    printPolynomial(&productHead);

    freePolynomial(&poly1Head);
    freePolynomial(&poly2Head);
    freePolynomial(&sumHead);
    freePolynomial(&productHead);

    return 0;
}

int readPolynomial(const char* fileName, Position head)
{
    FILE* file = fopen(fileName, "r");
    if (!file)
        return FILE_NOT_OPENED;

    int coef = 0, exp = 0;
    while (fscanf(file, "%d %d", &coef, &exp) == 2)
    {
        insertTerm(head, coef, exp);
    }

    fclose(file);
    return SUCCESS;
}

int insertTerm(Position head, int coefficient, int exponent)
{
    Position prev = head;
    Position current = head->next;

    while (current != NULL && current->exponent > exponent)
    {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->exponent == exponent)
    {
        current->coefficient += coefficient;
    }
    else
    {
        Position newTerm = (Position)malloc(sizeof(Term));
        if (!newTerm) return -2;
        newTerm->coefficient = coefficient;
        newTerm->exponent = exponent;
        newTerm->next = current;
        prev->next = newTerm;
    }
    return SUCCESS;
}

int addPolynomial(Position p1, Position p2, Position result)
{
    Position tmp = p1->next;
    while (tmp)
    {
        insertTerm(result, tmp->coefficient, tmp->exponent);
        tmp = tmp->next;
    }

    tmp = p2->next;
    while (tmp)
    {
        insertTerm(result, tmp->coefficient, tmp->exponent);
        tmp = tmp->next;
    }

    return SUCCESS;
}

int multiplyPolynomial(Position p1, Position p2, Position result)
{
    Position t1 = p1->next;
    while (t1)
    {
        Position t2 = p2->next;
        while (t2)
        {
            insertTerm(result, t1->coefficient * t2->coefficient, t1->exponent + t2->exponent);
            t2 = t2->next;
        }
        t1 = t1->next;
    }
    return SUCCESS;
}

void printPolynomial(Position head)
{
    Position tmp = head->next;
    while (tmp)
    {
        printf("%dx^%d", tmp->coefficient, tmp->exponent);
        if (tmp->next) printf(" + ");
        tmp = tmp->next;
    }
    printf("\n");
}

void freePolynomial(Position head)
{
    Position tmp = head->next;
    while (tmp)
    {
        Position toDelete = tmp;
        tmp = tmp->next;
        free(toDelete);
    }
}
