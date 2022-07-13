/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if(!q){
      return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->total = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements? */
    /* Free queue structure */
    list_ele_t *next;
    for(list_ele_t *ele = q->head; ele; ele = next) {
      next = ele->next;
      free(ele);
    }
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(queue_t *q, int v)
{
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    newh->value = v;
    newh->next = NULL;
    if(!newh){
      return false;
    }
    if(!q){
      return false;
    }
    if(q->total == 0){
      q->head = newh;
      q->tail = newh;
      q->total = 1;
      
    }else
    {
      newh->next = q->head;
      q->head = newh;
      q->total += 1;
    }
    
    /* What if malloc returned NULL? */
    
    
    
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(queue_t *q, int v)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    newh->value = v;
    newh->next = NULL;
    if(!newh){
      return false;
    }
    if(!q){
      return false;
    }
    if(q->total == 0){
      q->head = newh;
      q->tail = newh;
      q->total = 1;
      
    }else
    {
      q->tail->next = newh;
      q->tail = newh;
      q->total += 1;
    }

    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
bool q_remove_head(queue_t *q, int *vp)
{
    /* You need to fix up this code. */
    if(!q  || q->total == 0){
      return false;
    }
    q->total -= 1;
    *vp = q->head->value;
    if(q->total == 1){
      
      
      q->head = NULL;
      q->tail = NULL;
    }else{
      q->head = q->head->next;
    }
    
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if(!q  || q->total == 0){
      return 0;
    }
    return q->total;
}

/*
  Reverse elements in queue.

  Your implementation must not allocate or free any elements (e.g., by
  calling q_insert_head or q_remove_head).  Instead, it should modify
  the pointers in the existing data structure.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
    list_ele_t *tmp = NULL;
    list_ele_t *ne = NULL;
    list_ele_t *cur = q->head;
    while(!cur){
      ne = cur->next;
      cur->next = tmp;
      tmp = cur;
      cur = ne;
    } 
    tmp = q->head;
    q->head = q->tail;
    q->tail = tmp;
}

