#include "Stiva.h"

Stiva* CreazaStiva()
{
    Stiva *s = (Stiva*)malloc(sizeof(Stiva));
    s->bottom = s->top = NULL;
    s->size = 0;
    return s;
}

void ElibStiva(Stiva *s, void (*freeElem)(void*))
{
    while ( STIVAGOALA(s) == 0 )
        pop(s, freeElem);

    free(s->bottom);
    free(s->top);
    free(s);
}

void push(Stiva *s, void *info)
{
    if ( s->top == NULL && s->bottom == NULL )
    {
        s->bottom = CreazaNod(NULL, NULL, NULL);
        s->top =    CreazaNod(NULL, NULL, NULL);

        s->bottom->urm = s->top;
        s->top->prev   = s->bottom;
    }

    Nod* n = CreazaNod(info, NULL, TopElem(s));
    TopElem(s)->urm = n;
    TopElem(s) = n;
    s->size++;
}

void pop(Stiva *s, void (*freeElem)(void*))
{
    if( STIVAGOALA(s) )
        return;

    Nod *del = TopElem(s);

    TopElem(s) = TopElem(s)->prev;
    if( freeElem != NULL )
        freeElem(del->info);
    free(del);

    s->size--;
}

void* top(Stiva *s)
{
    return TopElem(s)->info;
}

void* cautaStiva(Stiva *s, void *info, int (*cmp)(void*, void*))
{
    void *el = NULL;

    Stiva *aux = CreazaStiva();
    while( !STIVAGOALA(s) )
    {
        if( cmp(info, top(s)) == 0 )
            el = top(s);
        push(aux, top(s));
        pop(s, NULL);
    }

    while(!STIVAGOALA(aux))
    {
        push(s, top(aux));
        pop(aux, NULL);
    }

    ElibStiva(aux, NULL);
    return el;
}
