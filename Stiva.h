#ifndef STIVA_H_INCLUDED
#define STIVA_H_INCLUDED

#include "Nod.h"

#define STIVAGOALA(s) ((s)->size == 0)
#define TopElem(s) ((s)->top->prev)

typedef struct _stiva
{
    Nod *top, *bottom;
    int size;
}Stiva;

Stiva* CreazaStiva();
void ElibStiva(Stiva *s, void (*freeElem)(void*));

void push(Stiva *s, void *info);
void pop(Stiva *s, void (*freeElem)(void*));
void* top(Stiva *s);

void* cautaStiva(Stiva *s, void *info, int (*cmp)(void*, void*));

#endif // STIVA_H_INCLUDED
