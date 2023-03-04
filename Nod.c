#include "Nod.h"

Nod* CreazaNod(void *info, Nod *urm, Nod *prev)
{
    Nod* n = (Nod*)malloc(sizeof(Nod));
    n->info = info;
    n->prev = prev;
    n->urm = urm;
    return n;
}

Nod* CreazaNod_Copy(void *info, int size, Nod *urm, Nod *prev)
{
    Nod* n = (Nod*)malloc(sizeof(Nod));
    memcpy(n->info, info, size);
    n->prev = prev;
    n->urm = urm;
    return n;
}
