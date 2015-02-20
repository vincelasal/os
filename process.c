#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//====================
//GLOBALS
typedef unsigned int PID;  
PID pidIndex = 0;


//=================
//Structs

typedef struct proc_t
{
	//Used to ID the process
	PID PID; //<---- :o 

	//Thread handle
  	pthread_t *threads;
	
	//For semaphore queue
	struct sem_t* next;

	//Traverse the doubly linked list.
	struct Node* next;
	struct Node* prev;
}process;




//Head of our main process list.
process* sporadicHeadPCB = NULL;




//==================
//Queue Functions
/*
 * The semaphores work as a giant queue. The masterSem is the master head of the semaphore queue.
 * Each semaphore that is created gets appended to the cue that masterSem is the head of.
 */
int enqueueProcess(process **head, process *newProcess)
{
    process *tmp;

    if (newProcess == NULL)
        return -1;

    //Case where it is the first one in the list.
    if (*head == NULL)
    {
        *head = newProcess;
        newProcess->next = NULL;
    }
    //Else the list is not empty, append this sem to the tail.
    else
    {
        tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = newProcess;
        newProcess->next = NULL;
    }
    return 1;
}




process* dequeueProcess(process **head)
{
    process *tmp = NULL;

    if (*head != NULL)
    {
        tmp = *head;
        *head = (*head)->next;
    }
    return tmp;
}




//Given a process queue, it will find the matching process
//for a given PID
process* findPID(process *head, int PID)
{
    semaphore *tmp = head;
    while (tmp != NULL)
        if (tmp->PID == PID)
            return tmp;
        else
            tmp = tmp->next;
    return tmp;
}





PID OS_Create(void (*f)(void), int arg, unsigned int level, unsigned int n)
{
	//Allocate and create 
	process *proc = malloc(1024);


	//Set some fields
	proc->PID = pidIndex;

	//Set its thread to operate on the given funciton pointer.


	//Append to the appropriate queue
	enqueueProcess(&sporadicHeadPCB, proc);

	pidIndex += 1;
	return pidIndex;
}


void OS_Terminate(void)
{

}    


void OS_Yield(void)
{

}


int  OS_GetParam(void)
{

}