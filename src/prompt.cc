#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "prompt.h"
#include "utils.h"

void *
bla(void *)
{
   return NULL;
}

void
launch_prompt(void) throw (std::runtime_error)
{
   pthread_t prompt_thread;
   int r;
   r = pthread_create(&prompt_thread, NULL, &bla, NULL);
   if (r != 0) { // some pthread_create error
      if (r == EAGAIN) {
         throw_fname("pthread_create", "not enough system resources");
      } else {
         fprintf(stderr, "pthread_create: unknown error\n");
         exit(EXIT_FAILURE);
      }
   }

   r = pthread_join(prompt_thread, NULL);
   if (r != 0) { // some pthread_join error
      switch (r) {
      case ESRCH:
         fprintf(stderr, "pthread_join: thread not found\n");
         exit(EXIT_FAILURE);
         break;
      case EINVAL:
         fprintf(stderr, "pthread_join: detached thread or already joined by another thread\n");
         exit(EXIT_FAILURE);
         break;
      case EDEADLK:
         fprintf(stderr, "pthread_join: thread tried to join to itself\n");
         exit(EXIT_FAILURE);
         break;
      default:
         fprintf(stderr, "pthread_join: unknown error\n");
         exit(EXIT_FAILURE);
      }
   }
}
