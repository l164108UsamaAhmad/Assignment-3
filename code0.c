#include<string.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include<sys/syscall.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h> 
#include <sys/shm.h>
#include <time.h>
#include <stdlib.h>

int CpatientCount=0;
int FpatientCount=0;

struct s_threadId
{
    int               ready;  
    pthread_mutex_t   mutex;    
    pthread_cond_t    cond;   
    pid_t             id;     
};

void *Fpatient(void* arg);
void *Cpatient(void* arg);

int main()
{
    pthread_t thread; 
    struct s_threadId threadId;
    pthread_cond_init(&threadId.cond, NULL);  
    pthread_mutex_init(&threadId.mutex, NULL);
    int N,i;
	printf("Enter number of potential CPatients enter in hospital \n");
	scanf("%d",&N);

    printf("----------------Test for Corona in Patients---------- \n");
    printf("Following are the results:- \n");
    for(i=0;i<N;i++)
	{
        int j=rand()%2;
		if(j==0)
		{
 		    pthread_mutex_lock(&threadId.mutex);
            threadId.ready = 0;
            pthread_create(&thread, NULL, &Fpatient, &threadId); 
            }
        else if(j==1)
		{
            pthread_mutex_lock(&threadId.mutex);
            threadId.ready = 0;
            pthread_create(&thread, NULL, &Cpatient, &threadId);
        }
        else 
        {
            while (!threadId.ready) 
            {
                pthread_cond_wait(&threadId.cond, &threadId.mutex);
            }
        }
        printf("\npatient id =%d\n",threadId.id);
        pthread_mutex_unlock(&threadId.mutex);
        pthread_join(thread, NULL);
    }
    pthread_mutex_destroy(&threadId.mutex);
    pthread_cond_destroy(&threadId.cond);
    printf("\nNo of Corona patients : %d\n",CpatientCount);
    printf("\nNo of Flue patients : %d\n",FpatientCount);
}

void *Fpatient(void* arg)
{
    FpatientCount++;
    struct s_threadId *thId = arg;
    pthread_mutex_lock(&thId->mutex);
    thId->id = syscall(SYS_gettid);
    printf("Flue patient. You are Safe Congratulations!!\n");
    thId->ready = 1;
    pthread_cond_signal(&thId->cond);
    pthread_mutex_unlock(&thId->mutex);
    return NULL;
   }


void *Cpatient(void* arg)
{
    int recover=0,effected=0; 	
    CpatientCount++;
    struct s_threadId *thId = arg;
    pthread_mutex_lock(&thId->mutex);
    thId->id =0;
    thId->id= syscall(SYS_gettid);
   
    if(rand()%2==0)
	{
        printf("He is Suffering From Corona\n");
        printf("The patient is Tested Again For Corona\n");
        if(rand()%2==0)
		{
            effected=1;
            recover=0;
            printf("Case Positive patient Not recovered\n");
        }
        else
        {
            effected=0;
            recover=1;
            printf("Case Negative patient Recovered\n");
        }
	}
    else 
    {
        printf("He is Not Suffering From Corona\n");
        printf("The patient is Tested Again For Corona\n");
        if(rand()%2==0)
		{
            effected=0;
            recover=1;
            printf("Case Negative and not a Corona patient\n");
        }
        else
        {
            effected=1;
            recover=0;
            printf("Case postive and he is a Corona Patient\n");
        }
    }
    
    if  (effected==1&&recover==0)
    {
        printf("Result = Not recovered\n");
    }
    
    thId->ready = 1;
  	pthread_cond_signal(&thId->cond);
	pthread_mutex_unlock(&thId->mutex);
  	return NULL;

}
