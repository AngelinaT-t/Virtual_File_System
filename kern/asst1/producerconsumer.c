/* This file will contain your solution. Modify it as you wish. */
#include <types.h>
#include <lib.h>
#include <synch.h>
#include "producerconsumer_driver.h"

/* Declare any variables you need here to keep track of and
   synchronise your bounded. A sample declaration of a buffer is shown
   below. It is an array of pointers to items.

   You can change this if you choose another implementation.
   However, you should not have a buffer bigger than BUFFER_SIZE
*/

data_item_t * item_buffer[BUFFER_SIZE];

int count = 0;
struct lock *count_lock;
struct cv *full, *empty;


/* consumer_receive() is called by a consumer to request more data. It
   should block on a sync primitive if no data is available in your
   buffer. It should not busy wait! */

data_item_t * consumer_receive(void)
{
      data_item_t * item;

      lock_acquire(count_lock);
      while(count == 0){
         cv_wait(empty,count_lock);
      }
      item = item_buffer[count-1];
      count--;
      cv_signal(full,count_lock);
      lock_release(count_lock);

      return item;
}

/* procucer_send() is called by a producer to store data in your
   bounded buffer.  It should block on a sync primitive if no space is
   available in your buffer. It should not busy wait!*/

void producer_send(data_item_t *item)
{
        //(void) item; /* Remove this when you add your code */
        lock_acquire(count_lock);
        while (count == BUFFER_SIZE){
           cv_wait(full,count_lock);
        } 
        item_buffer[count] = item;
        count++; 
        cv_signal(empty,count_lock); 
        lock_release(count_lock);
}




/* Perform any initialisation (e.g. of global data) you need
   here. Note: You can panic if any allocation fails during setup */

void producerconsumer_startup(void)
{
   count_lock = lock_create("count lock");
   if (count_lock == NULL) panic("count_lock dead");
   empty = cv_create("empty");
   if (empty == NULL) panic("empty dead");
   full = cv_create("full");
   if (full == NULL) panic("full dead");

}

/* Perform any clean-up you need here */
void producerconsumer_shutdown(void)
{
   lock_destroy(count_lock);
   cv_destroy(empty);
   cv_destroy(full);

}
