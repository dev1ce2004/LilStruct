#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define FILE_NOT_OPENED (-1)
#define SUCCESS 0
#define MALLOC_ERROR (-2)

struct _StackNode;
typedef struct _StackNode* Position;
typedef struct _StackNode
{
    double value;
    Position next;
} StackNode;

int push(Position head, double value);
int pop(Position head, double* value);
int evaluatePostfix(const char* fileName, double* result);
int freeStack(Position head);

int main()
{
    double result = 0;

    if (evaluatePostfix("postfix.txt", &result) != SUCCESS)
    {
        printf("Error\n");
        return FILE_NOT_OPENED;
    }

    printf("Rezultat: %.2lf\n", result);

    return 0;
}

int push(Position head, double value)
{
    Position newNode = (Position)malloc(sizeof(StackNode));
    if (!newNode) return MALLOC_ERROR;

    newNode->value = value;
    newNode->next = head->next;
    head->next = newNode;

    return SUCCESS;
}

int pop(Position head, double* value)
{
    if (head->next == NULL) return -1;

    Position temp = head->next;
    *value = temp->value;
    head->next = temp->next;
    free(temp);

    return SUCCESS;
}

int evaluatePostfix(const char* fileName, double* result)
{
    FILE* file = fopen(fileName, "r");
    if (!file) return FILE_NOT_OPENED;

    StackNode stackHead = { 0, NULL };
    char token[MAX_LINE] = { 0 };
    double a = 0, b = 0;
    int error = 0;

    while (fscanf(file, "%s", token) != EOF)
    {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
        {
            if ((error = push(&stackHead, atof(token))) != SUCCESS)
            {
                fclose(file);
                freeStack(&stackHead);
                return error;
            }
        }
        else
        {
            if ((error = pop(&stackHead, &b)) != SUCCESS || (error = pop(&stackHead, &a)) != SUCCESS)
            {
                fclose(file);
                freeStack(&stackHead);
                return -3;
            }

            switch (token[0])
            {
            case '+': error = push(&stackHead, a + b); break;
            case '-': error = push(&stackHead, a - b); break;
            case '*': error = push(&stackHead, a * b); break;
            case '/': error = push(&stackHead, a / b); break;
            default:
                fclose(file);
                freeStack(&stackHead);
                return -4;
            }
            if (error != SUCCESS)
            {
                fclose(file);
                freeStack(&stackHead);
                return error;
            }
        }
    }

    if ((error = pop(&stackHead, result)) != SUCCESS)
    {
        fclose(file);
        freeStack(&stackHead);
        return error;
    }

    freeStack(&stackHead);
    fclose(file);
    return SUCCESS;
}

int freeStack(Position head)
{
    Position tmp = head->next;
    while (tmp)
    {
        Position toDelete = tmp;
        tmp = tmp->next;
        free(toDelete);
    }
    head->next = NULL;
    return SUCCESS;
}
