#include "Coada.h"

Coada* CreazaCoada()
{
    Coada *c = (Coada*)malloc(sizeof(Coada));
    c->front = c->back = NULL;
    c->size = 0;
    return c;
}

void ElibCoada(Coada* c, void (*freeElem)(void*))
{
    while( !COADAGOALA(c) )
        dequeue(c, freeElem);

    if( c->back != NULL && c->front != NULL )
    {
        free(c->back);
        free(c->front);
    }

    free(c);
}

void enqueue(Coada *c, void *info)
{
    if( c->back == NULL && c->front == NULL )
    {
        c->front = CreazaNod(NULL, NULL, NULL);
        c->back =  CreazaNod(NULL, NULL, NULL);

        c->front->urm = c->back;
        c->back->prev = c->front;
    }

    Nod *n = CreazaNod(info, c->back, BackElem(c));
    BackElem(c)->urm = n;
    BackElem(c) = n;
    c->size++;
}

void dequeue(Coada *c, void (*freeElem)(void*))
{
    if( COADAGOALA(c) )
        return;

    Nod *del = FrontElem(c);

    FrontElem(c) = FrontElem(c)->urm;
    if( c->size == 1 )
        BackElem(c) = c->front;

    if( freeElem != NULL )
        freeElem(del->info);
    free(del);

    c->size--;
}

void* front(Coada *c)
{
    return FrontElem(c)->info;
}

void enqueueSortat(Coada *c, void *info, int (*cmp)(void*, void*))
{
    if( COADAGOALA(c) )
    {
        enqueue(c, info);
        return;
    }

    Coada *aux = CreazaCoada();

    while( !COADAGOALA(c) && cmp( front(c), info ) < 0 )
    {
        enqueue(aux, front(c));
        dequeue(c, NULL);
    }
    enqueue(aux, info);
    while( !COADAGOALA(c) )
    {
        enqueue(aux, front(c));
        dequeue(c, NULL);
    }

    while( !COADAGOALA(aux) )
    {
        enqueue(c, front(aux));
        dequeue(aux, NULL);
    }

    ElibCoada(aux, NULL);
}

void* cautaCoada(Coada *c, void *info, int (*cmp)(void*, void*))
{
    void* el = NULL;

    Coada *aux = CreazaCoada();
    while( !COADAGOALA(c) )
    {
        if( cmp(info, front(c)) == 0 && el == NULL )
            el = front(c);

        enqueue(aux, front(c));
        dequeue(c, NULL);
    }

    while( !COADAGOALA(aux) )
    {
        enqueue(c, front(aux));
        dequeue(aux, NULL);
    }

    ElibCoada(aux, NULL);
    return el;
}
