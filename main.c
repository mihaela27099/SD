#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "Stiva.h"
#include "Coada.h"

typedef struct _thread
{
    int id;
}Thread;

typedef struct _task
{
    int id;
    Thread *thread;
    char priorietate;
    int timpExecutie;
    int timpRamas;
}Task;

void elibThread(void *x)
{
    free((Thread*)x);
}

void elibTask(void *x)
{
    free(((Task*)x)->thread);
    free((Task*)x);
}

int minID(int *ids)
{
    for( int i = 1; i <= 32767; i++ )
        if( ids[i] == 0 )
            return i;
    return -1;
}

int cmpTasks(void *a, void *b)
{
    Task *task1 = (Task*)a;
    Task *task2 = (Task*)b;

    if( task1->priorietate == task2->priorietate )
    {
        if( task1->timpExecutie == task2->timpExecutie )
            return task1->id - task2->id;
        else
            return task1->timpExecutie - task2->timpExecutie;
    }
    else
        return task2->priorietate - task1->priorietate;
}

int cmpId(void *a, void *b)
{
    Task *task1 = (Task*)a;
    Task *task2 = (Task*)b;

    return task1->id - task2->id;
}

int cmpThread(void *a, void *b)
{
    Task *task1 = (Task*)a;
    Task *task2 = (Task*)b;

    return task1->thread->id - task2->thread->id;
}

void printWaiting(Coada *c, FILE *fr)
{
    Coada *aux = CreazaCoada();
    fprintf(fr,"====== Waiting queue =======\n[");

    while( !COADAGOALA(c) )
    {
        Task* task = (Task*)front(c);
        dequeue(c, NULL);

        fprintf(fr,"(%d: priority = %d, remaining_time = %d)", task->id, task->priorietate, task->timpRamas);

        if( c->size >= 1 )
            fprintf(fr,",\n");

        enqueue(aux, (void*)task);
    }
    fprintf(fr,"]\n");

    while(!COADAGOALA(aux))
    {
        enqueue(c, front(aux));
        dequeue(aux, NULL);
    }
    ElibCoada(aux, NULL);
}

void printRunning(Coada *c, FILE *fr)
{
    Coada *aux = CreazaCoada();
    fprintf(fr,"====== Running in parallel =======\n[");

    while( !COADAGOALA(c) )
    {
        Task* task = (Task*)front(c);
        dequeue(c, NULL);

        fprintf(fr,"(%d: priority =%d, remaining_time = %d, running_thread = %d)", task->id, task->priorietate, task->timpRamas, task->thread->id);

        if( c->size >= 1 )
            fprintf(fr,",\n");

        enqueue(aux, (void*)task);
    }
    fprintf(fr,"]\n");

    while(!COADAGOALA(aux))
    {
        enqueue(c, front(aux));
        dequeue(aux, NULL);
    }
    ElibCoada(aux, NULL);
}

void printFinished(Coada *c, FILE *fr)
{
    Coada *aux = CreazaCoada();
    fprintf(fr,"====== Finished queue =======\n[");

    while( !COADAGOALA(c) )
    {
        Task* task = (Task*)front(c);
        dequeue(c, NULL);

        fprintf(fr,"(%d: priority = %d, executed_time = %d)", task->id, task->priorietate, task->timpExecutie);

        if( c->size >= 1 )
            fprintf(fr,",\n");

        enqueue(aux, (void*)task);
    }
    fprintf(fr,"]\n");

    while(!COADAGOALA(aux))
    {
        enqueue(c, front(aux));
        dequeue(aux, NULL);
    }
    ElibCoada(aux, NULL);
}

void stergeTerminate(Coada *running, Coada *finished, Stiva *threads, int *ids)
{
    Coada *aux = CreazaCoada();

    while(!COADAGOALA(running))
    {
        Task *task = (Task*)front(running);
        dequeue(running, NULL);

        if( task->timpRamas <= 0 )
        {
            push(threads, (void*)task->thread);
            task->thread = NULL;
            ids[task->id] = 0;
            enqueue(finished, (void*)task);
        }
        else
            enqueue(aux, (void*)task);
    }

    while( !COADAGOALA(aux) )
    {
        enqueue(running, front(aux));
        dequeue(aux, NULL);
    }

    ElibCoada(aux, NULL);
}

int incepeTaskuri(Coada *waiting, Coada *running, Stiva *threads)
{
    int incep = 0;
    while( !STIVAGOALA(threads) && !COADAGOALA(waiting) )
    {
        Task *t = (Task*)front(waiting);
        t->thread = top(threads);

        pop(threads, NULL);
        dequeue(waiting, NULL);

        enqueue(running, (void*)t);
        incep++;
    }
    return incep;
}

void decrementeazaTimp(Coada *running, int timp)
{
    Coada *aux = CreazaCoada();

    while(!COADAGOALA(running))
    {
        Task *task = (Task*)front(running);
        dequeue(running, NULL);

        task->timpRamas -= timp;
        enqueue(aux, (void*)task);
    }

    while( !COADAGOALA(aux) )
    {
        enqueue(running, front(aux));
        dequeue(aux, NULL);
    }

    ElibCoada(aux, NULL);

}

void run(int timp, int q, Coada *waiting, Coada *running, Coada *finished, Stiva *threads, int *ids)
{
    for(int i = 0; i < timp; i += q)
    {
        decrementeazaTimp(running, q);
        stergeTerminate(running, finished, threads, ids);
        incepeTaskuri(waiting, running, threads);
    }
}

int runFinal(Coada *waiting, Coada *running, Coada *finished, Stiva *threads, int *ids)
{
    int timp = 0;
    while( !COADAGOALA(waiting) || !COADAGOALA(running) )
    {
        decrementeazaTimp(running, 1);
        stergeTerminate(running, finished, threads, ids);
        incepeTaskuri(waiting, running, threads);
        timp++;
    }

    return timp;
}

int main(int argc, char **argv)
{
    FILE* f  = fopen(argv[1], "r");
    FILE* fr = fopen(argv[2], "w");
    Stiva *threads = CreazaStiva();
    Coada *waiting = CreazaCoada();
    Coada *running = CreazaCoada();
    Coada *finished = CreazaCoada();

    int *idOcupat = malloc(sizeof(int)*32768);
    for(int i = 0; i < 32768; i++)
        idOcupat[i] = 0;

    int q, n;
    fscanf(f, "%d", &q);
    fscanf(f, "%d\n", &n);
    n *= 2;

    for( int i = n - 1; i >= 0; i-- )
    {
        Thread *thread = (Thread*)malloc(sizeof(Thread));
        thread->id = i;
        push(threads, thread);
    }

    char command[1024];
    while( fgets(command, 1023, f) )
    {
        command[strlen(command) - 1] = '\0';
        char *arg = strtok(command, " ");

        if( strcmp(arg, "add_tasks") == 0 )
        {
            int nr = atoi(strtok(NULL, " "));
            int execTime = atoi(strtok(NULL, " "));
            int p = atoi(strtok(NULL, " "));

            for(int i = 0; i < nr; i++)
            {
                Task* task = (Task*)malloc(sizeof(Task));
                task->id = minID(idOcupat);
                idOcupat[task->id] = 1;
                task->priorietate = p;
                task->timpExecutie = execTime;
                task->timpRamas = execTime;
                task->thread = NULL;

                enqueueSortat(waiting, (void*)task, cmpTasks);
                fprintf(fr,"Task created successfully : ID %d.\n", task->id);
            }
        }

        if( strcmp(arg, "get_task") == 0 )
        {
            int id = atoi(strtok(NULL, " "));
            Task fake;
            fake.id = id;

            Task* found = cautaCoada(running, (void*)&fake, cmpId);
            if( found != NULL )
                fprintf(fr,"Task %d is running (remaining_time = %d).\n", found->id, found->timpRamas);
            else
            {
                found = cautaCoada(waiting, (void*)&fake, cmpId);
                if( found != NULL )
                    fprintf(fr,"Task %d is waiting (remaining_time = %d).\n", found->id, found->timpRamas);
                else
                {
                    found = cautaCoada(finished, (void*)&fake, cmpId);
                    if( found != NULL )
                        fprintf(fr,"Task %d is finished (executed_time = %d).\n", found->id, found->timpExecutie);
                    else
                        fprintf(fr,"Task %d not found.\n", id);
                }
            }
        }

        if( strcmp(arg, "get_thread") == 0 )
        {
            int id = atoi(strtok(NULL, " "));
            if( id < n )
            {
                Task fake;
                fake.thread = (Thread*)malloc(sizeof(Thread));
                fake.thread->id = id;

                Task *found = cautaCoada(running, (void*)&fake, cmpThread);
                if( found != NULL )
                    fprintf(fr,"Thread %d is running task %d (remaining_time = %d).\n", found->thread->id, found->id, found->timpRamas);
                else
                    fprintf(fr,"Thread %d is idle.\n", id);

                free(fake.thread);
            }
        }

        if( strcmp(arg, "print") == 0 )
        {
            arg = strtok(NULL, " ");
            if( strcmp( arg, "waiting" ) == 0 )
                printWaiting(waiting, fr);
            else if( strcmp(arg, "running") == 0 )
                printRunning(running, fr);
            else
                printFinished(finished, fr);
        }

        if( strcmp(arg, "run") == 0 )
        {
            int timp = atoi(strtok(NULL, " "));
            fprintf(fr,"Running tasks for %d ms...\n", timp);

            run(timp, q, waiting, running, finished, threads, idOcupat);
        }

        if( strcmp(arg, "finish") == 0 )
        {
            int timpToatal = runFinal(waiting, running, finished, threads, idOcupat);
            fprintf(fr,"Total time: %d\n", timpToatal);
        }

    }

    fclose(f);
    fclose(fr);
    free(idOcupat);
    ElibStiva(threads, elibThread);
    ElibCoada(waiting, elibTask);
    ElibCoada(running, elibTask);
    ElibCoada(finished, elibTask);
    return 0;
}
