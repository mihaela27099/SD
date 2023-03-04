#ifndef COADA_H_INCLUDED
#define COADA_H_INCLUDED

#include "Nod.h"
#include "Stiva.h"

#define COADAGOALA(c) ((c)->size == 0)
#define FrontElem(c)  ((c)->front->urm)
#define BackElem(c)   ((c)->back->prev)

typedef struct _coada
{
    Nod *front, *back;
    int size;
}Coada;

Coada* CreazaCoada();
void ElibCoada(Coada* c, void (*freeElem)(void*));

void enqueue(Coada *c, void *info);
void dequeue(Coada *c, void (*freeElem)(void*));
void* front(Coada *c);

void enqueueSortat(Coada *c, void *info, int (*cmp)(void*, void*));
void* cautaCoada(Coada *c, void *info, int (*cmp)(void*, void*));

#endif // COADA_H_INCLUDED
