#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "prompt.h"

void *
bla(void *)
{
   return NULL;
}

void
launch_prompt(void)
{
   pthread_t prompt_thread;
   int r;
   r = pthread_create(&prompt_thread, NULL, &bla, NULL);
   if (r != 0) {
      fprintf(stderr, "error on pthread_create!");
      exit(EXIT_FAILURE);
   }
   pthread_join(prompt_thread, NULL);
}
