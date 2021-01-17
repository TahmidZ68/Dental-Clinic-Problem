#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#ifdef __unix__
#endif
#include <unistd.h>
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))

#define LIST_INIT_SIZE 1000000
#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999

int listMaxSize;
int * list;
int length;

void initializeList()
{
    listMaxSize = LIST_INIT_SIZE;
    list = (int*)malloc(sizeof(int)*listMaxSize);
    length = 10000 ;
}

void checkSizeOfList()
{
    int * tempList ;
    if (length == listMaxSize)
    {
        //allocate new memory space for tempList
        listMaxSize = 2 * listMaxSize ;
        tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
        int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        free(list) ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
    }

}
int insertItem(int newitem)
{
    checkSizeOfList();

    list[length] = newitem ; //store new item
    length++ ;
    return SUCCESS_VALUE ;
}

void *patient(void *num);
void *dentist(void *);
void randwait(int secs);

sem_t waitingRoom;
sem_t dentistChair;
sem_t dentistPillow;
sem_t seatBelt;

int allDone = 0;

int main(int argc, char *argv[])
{
    system("cls");
    system("color B");
    printf("\n\n\n\t\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");

    printf("\n\n\n\t\t\t\t\t          Dental Clinic Problem\n");
    printf("\n\t\t\t\t\t\t      Created by \n");
    printf("\n\t\t\t\t\t                Tahmid \n");

    printf("\n\n\n\t\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD");
    printf("\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\n");
    puts("\n\t\t\t\t\t        Press any key to continue\n\n");

    getch();
    initializeList();
    pthread_t btid;
    pthread_t tid[length];
    long RandSeed;
    int i, numPatients, numChairs;
    int Number[length];

    printf("\t\t\t\t\t      Enter the number of Patients : ");
    scanf("%d",&numPatients) ;
    printf("\n\n\t\t\t\t\t      Enter the number of Chairs : ");
    scanf("%d",&numChairs);

    checkSizeOfList();

    for (i=0; i<length; i++)
    {
        Number[i] = i;
    }

    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&dentistChair, 0, 1);
    sem_init(&dentistPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    pthread_create(&btid, NULL, dentist, NULL);

    for (i=0; i<numPatients; i++)
    {
        pthread_create(&tid[i], NULL, patient, (void *)&Number[i]);
        sleep(1);
    }

    for (i=0; i<numPatients; i++)
    {
        pthread_join(tid[i],NULL);
        sleep(1);
    }

    allDone = 1;
    sem_post(&dentistPillow);
    pthread_join(btid,NULL);
}

void *patient(void *number)
{
    system("color E");
    int num = *(int *)number;

    printf("\n\n\t\t\t\t\t    Patient %d leaving for Dental clinic.\n", (num+1));
    randwait(2);
    printf("\n\n\t\t\t\t\t    Patient %d arrived at Dental clinic.\n", (num+1));

    sem_wait(&waitingRoom);
    printf("\n\n\t\t\t\t\t     Patient %d entering waiting room.\n", (num+1));

    sem_wait(&dentistChair);
    sem_post(&waitingRoom);

    printf("\n\n\t\t\t\t\t     Patient %d waking the Dentist.\n", (num+1));
    sem_post(&dentistPillow);
    sem_wait(&seatBelt);
    sem_post(&dentistChair);

    printf("\n\n\t\t\t\t\t    Patient %d leaving Dental clinic.\n", (num+1));
}

void *dentist(void *work)
{
    while (!allDone)
    {
        printf("\n\n\t\t\t\t\t        The Dentist is sleeping\n");
        sem_wait(&dentistPillow);

        if (!allDone)
        {
            printf("\n\n\t\t\t\t\t     The Dentist is doing treatment\n");
            randwait(2);
            printf("\n\n\t\t\t\t\t     The Dentist has finished treatment.\n");
            sem_post(&seatBelt);
        }
        else
        {
            printf("\n\n\t\t\t\t\t     The Dentist is going home for the day.\n");
        }
    }
}

void randwait(int secs)
{
    int len;
    len = (int) ((1 * secs) + 1);
    sleep(len);
}

