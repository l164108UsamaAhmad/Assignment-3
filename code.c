#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include<time.h>

int counter1=0;
int counter2=0;


int buffer1[1000];
int buffer2[1000];

sem_t mutex1;
sem_t mutex2;
sem_t mutex3;
sem_t mutex4;

struct fileread {
  char * filename;
};


void *Readfromfile1(void * arg)
{
    struct fileread * access1 = arg;
  
  FILE * f1;
    
  f1 = fopen(access1->filename, "r");
  int ch1;

  if (f1 == NULL) {
    printf("Sorry Unable to open file1\n");
  } 
  else 
  {
     int ch1=getc(f1);
     while (ch1!=EOF)
     {
       sem_wait(&mutex1);
       buffer1[counter1]=ch1;
       int ch1=getc(f1);
       counter1++;
int j;
       for( j=0;j<10;j++)
       {
          sem_post(&mutex3);
       }
    }
  }
  //fclose(f1);
 pthread_exit(NULL);
}
void* processC(void * arg)
{
  while(1)
{
int o;
    for( o=0;o<10;o++)
     {
       sem_wait(&mutex2);
     }
int k;
     for( k=0;k<counter2;k++)
     {
       buffer2[k]=buffer1[k];
	        
     }
	  sem_post(&mutex4);
	 
     
 }  
pthread_exit(NULL);

}
void* Readfromfile2(void * arg)
{
counter2=counter1;
  struct fileread * access2 = arg;
   FILE * f2;

  f2 = fopen(access2->filename, "r");
  int ch;

  if (f2 == NULL) {
    printf("Sorry Unable to open file1\n");
  } else {

  int ch=getc(f2);
   while (ch!=EOF)
   {
int z;
     for( z=0;z<10;z++)
      {
        sem_wait(&mutex3);
      }
       buffer1[counter2]=ch;
       int ch=getc(f2);
      //buffer1[counter2]=ch;
       counter2++;
       sem_post(&mutex2);
    }
  }
fclose(f2);
pthread_exit(NULL);
}




void* processD(void * arg)
{
while(1)
{int k;
    sem_wait(&mutex4);
    for( k=0;k<counter2;k++)
     {
       putchar(buffer2[k]);
     }
int v;
       for( v=0;v<10;v++)
       {
           sem_post(&mutex1);
       }
 }
pthread_exit(NULL);
   
}

int main(int args, char ** argv) {

  pthread_t tid1, tid2 ,tid3, tid4;

  struct fileread thread1;
  struct fileread thread2;
  thread1.filename = argv[1];
  thread2.filename = argv[2];

  sem_init( & mutex1, 0, 10);
  sem_init( & mutex2, 0, 0);
  sem_init( & mutex3, 0, 10);
  sem_init( & mutex4, 0, 0);

  if (pthread_create(&tid1,NULL,&Readfromfile1,&thread1) <0) {
    printf("Sorry cant make thread1\n");

  }
  if(pthread_create( & tid2, NULL, & Readfromfile2,&thread2) <0) {
    printf("Sorry can,t thread2\n");
  }


  if(pthread_create( & tid3, NULL, & processC,NULL) <0) {
    printf("Sorry can,t thread2\n");
  }

  if(pthread_create( & tid4, NULL, & processD,NULL) <0) {
    printf("Sorry can,t thread2\n");
  }
  if (pthread_join(tid1, NULL) < 0) {
    printf("Sorry can,t join thread1\n");
  }

  if (pthread_join(tid2, NULL) < 0) {
    printf("Sorry can,t join thread2\n");
  }

  if (pthread_join(tid3, NULL) < 0) {
    printf("Sorry can,t join thread2\n");
  }
  if (pthread_join(tid4, NULL) < 0) {
    printf("Sorry can,t join thread2\n");
  }
  sem_destroy(&mutex1);
  sem_destroy(&mutex2);
  
  sem_destroy(&mutex3);
  sem_destroy(&mutex4);
 
  return 0;

}

