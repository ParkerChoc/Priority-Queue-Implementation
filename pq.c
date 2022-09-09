/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:    Parker Choc
 * Email:   chocp@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "pq.h"

struct node
{
    int priority;
    void* value;
};

struct pq
{
    struct node* array;
    int size;
    int capacity;
};

//Function for testing purposes. Only works with int pointers
void print(struct pq* pq)
{
    for(int i = 0; i < pq->size; i++)
    {
        printf("Index: [%d]", i);
        printf(" Value: %d", *((int *)pq->array[i].value));
        printf(" Priority: %d\n", pq->array[i].priority);
    }
    printf("\n");
}

struct pq* pq_create()
{
    struct pq* queue = malloc(sizeof(struct pq));
    queue->array = malloc(sizeof(struct node));
    queue->size = 0;
    queue->capacity = 1;
    return queue;
}

void pq_free(struct pq* pq)
{
    free(pq->array);
    free(pq);
}

int pq_isempty(struct pq* pq)
{
    if(pq->size == 0)
        return 1;
    return 0;
}

void pq_resize(struct pq* pq)
{
  pq->capacity *= 2;
  struct node* new_array = malloc(pq->capacity * sizeof(struct node));

  for (int i = 0; i < pq->size; i++) 
    new_array[i] = pq->array[i];

  free(pq->array);
  pq->array = new_array;
}

void swap(struct pq* pq, int a, int b)
{
    struct node temp = pq->array[a];
    pq->array[a] = pq->array[b];
    pq->array[b] = temp;
}

int f_p_i(int child_index)  //find parent index
{
    if(child_index <= 0)    //root node
        return -1;
    
    if(child_index % 2 == 0)    //even index: right child
        return (child_index - 2) / 2;
    else    //odd index: left child
        return (child_index - 1) / 2;
}

void percolate_up(struct pq* pq, int c) //c: child index
{
    if(c <= 0)
        return;
    int p = f_p_i(c);
    while(pq->array[p].priority > pq->array[c].priority)
    {
        swap(pq, c, p);
        c = p;
        p = f_p_i(c);
        if(p < 0)
            return;
    }
}

void pq_insert(struct pq* pq, void* value, int priority)
{
	//Part 1: add node to array
    if(pq->size == pq->capacity)
        pq_resize(pq);
    pq->array[pq->size].value = value;
    pq->array[pq->size].priority = priority;

    //Part 2: percolate upwards
    percolate_up(pq, pq->size);
    pq->size++;
}

void* pq_first(struct pq* pq)
{
    return pq->array[0].value;
}

int pq_first_priority(struct pq* pq)
{
    return pq->array[0].priority;
}

int lowest_priority_child(struct pq* pq, int i)
{
    //Returns index of lowest priority child
    if(pq->array[(2 * i) + 2].priority < pq->array[(2 * i) + 1].priority)
        return (2 * i) + 2; //right child is smaller
    else 
        return (2 * i) + 1; //left child is smaller
}

void percolate_down(struct pq* pq, int i)
{
    if(pq->size > (2 * i) + 2)    //has left and right children
    {
        if(pq->array[lowest_priority_child(pq, i)].priority < pq->array[i].priority)
        {
            int next = lowest_priority_child(pq, i);
            swap(pq, i, next);
            i = next;
            percolate_down(pq, next);
        }
        else
            return; //lowest child priority > parent priority        
    }
    else if(pq->size == (2 * i) + 2)    //has just a left child
    {
        if(pq->array[(2 * i) + 1].priority < pq->array[i].priority)
        {
            swap(pq, i, (2 * i) + 1);
            return;
        }
        else
            return;
    }
    else    //No children
        return;
}

void* pq_remove_first(struct pq* pq)
{
    void* first = pq_first(pq);
    if(pq->size > 0)
        pq->array[0] = pq->array[pq->size - 1];
    else
    {
        printf("This queue is empty\n");
        return NULL;
    }
    percolate_down(pq, 0);
    pq->size--;
    return first;
}
