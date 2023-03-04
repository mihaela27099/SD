#ifndef NOD_H_INCLUDED
#define NOD_H_INCLUDED

#include <stdlib.h>
#include <string.h>

typedef struct _nod
{
    void *info;
    struct _nod *urm, *prev;
}Nod;

Nod* CreazaNod(void *info, Nod *urm, Nod *prev);
Nod* CreazaNod_Copy(void *info, int size, Nod *urm, Nod *prev);

#endif // NOD_H_INCLUDED
