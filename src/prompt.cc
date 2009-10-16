#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>

#include "prompt.h"
#include "utils.h"
#include "DebugStream.h"
extern DebugStream debug;

const int LINESZ = 1024;
const char * COMMAND_EXIT = "exit";

int
line_is_too_big(const char * line)
{
   UNUSED(line);
   return 0;
}

int
is_not_command(const char * line)
{
   UNUSED(line);
   return 0;
}

bool
is_space(char c)
{
   if (c == ' ')
      return true;
   else
      return false;
}

void
turn_tabs_into_spaces(const char * from, char * to)
{
   size_t len = strlen(from);

   for (size_t i=0; i<len; i++) {
      if (from[i] == '\t')
         to[i] = ' ';
      else
         to[i] = from[i];
   }
   to[len + 1] = '\0';
}

void
drop_repeated_spaces(const char * from, char * to)
{
   size_t len = strlen(from);
   bool prev_was_space = false;

   size_t j = 0;
   for (size_t i=0; i<len; i++) {
      if (prev_was_space && is_space(from[i]))
         continue;
      if (is_space(from[i]))
         prev_was_space = true;
      else
         prev_was_space = false;
      to[j++] = from[i];
   }
   to[j] = '\0';
}

void
drop_ending_eol(const char * from, char * to)
{
   size_t len = strlen(from);

   strcpy(to, from);
   if (to[len-1] == '\n')
      to[len-1] = '\0';
}


void
drop_space_at_start(const char * from, char * to)
{
   if (is_space(from[0]))
      strcpy(to, from+1);
   else
      strcpy(to, from);
}


void
drop_space_at_end(const char * from, char * to)
{
   size_t len = strlen(from);

   strcpy(to, from);
   if (to[len-1] == ' ')
      to[len-1] = '\0';
}

// copy line to clean_line droopping repeated spaces and such
void
clean_line(const char * line, char * clean)
{
   //   debug << "clean_line() gets:\n\t\"" << std::string(line) << "\"" << std::endl;

   char tmp[LINESZ];
   memset(tmp, '\0', LINESZ);
   strcpy(tmp, line);

   // turn tabs into spaces
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      turn_tabs_into_spaces(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after turn tabs into spaces:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop repeated spaces
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      drop_repeated_spaces(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping repeated spaces:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop '\n' at the end
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      drop_ending_eol(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping '\\n' at the end:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop space at the start
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      drop_space_at_start(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping space at start:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop space at the end
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      drop_space_at_end(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping space at end:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   strcpy(clean, tmp);

   //   debug << "clean_line() puts \"" << std::string(clean) << "\"" << std::endl;

   return;
}

int
is_command_exit(const char * line)
{
   if (memcmp(line, COMMAND_EXIT, strlen(COMMAND_EXIT)) == 0)
      return 1;
   else
      return 0;
}

void
read_clean_line(char * clean)
{
   char line[LINESZ];
   memset(line, '\0', LINESZ);

   char * p;
   p = fgets(line, LINESZ, stdin);
   if (p == NULL) memcpy(line, COMMAND_EXIT, strlen(COMMAND_EXIT)+1); // EOF detected

   //   debug << "read \"" << std::string(line) << "\" from stdin" << std::endl;

   if (line_is_too_big(line)) {
      fprintf(stderr, "error: line too long on stdin!\n");
      exit(EXIT_FAILURE);
   }

   clean_line(line, clean);
   //   debug << "line cleaned to \"" << std::string(clean) << "\"" << std::endl;
}

void *
prompt_loop(void *)
{
   char line[LINESZ];

   while (1) {
      int r;
      r = fputs("> ", stdout);
      if (r == EOF) {
         fprintf(stderr, "fputs: unknown error\n");
         exit(EXIT_FAILURE);
      }

      read_clean_line(line);

      if (is_not_command(line)) {
         fprintf(stderr, "error: not a command!\n");
         exit(EXIT_FAILURE);
      }

      if (is_command_exit(line)) {
         r = fputs("bye!\n", stdout);
         if (r == EOF) {
            fprintf(stderr, "fputs: unknown error\n");
            exit(EXIT_FAILURE);
         }
         return NULL;
      } else {
         continue;
      }

   }
   return NULL;
}

void
launch_prompt_and_wait(void) throw (std::runtime_error)
{
   int r;

   pthread_t prompt_thread;
   r = pthread_create(&prompt_thread, NULL, &prompt_loop, NULL);
   if (r != 0) { // some pthread_create error
      if (r == EAGAIN) {
         throw_fname("pthread_create", "not enough system resources");
      } else {
         fprintf(stderr, "pthread_create: unknown error\n");
         exit(EXIT_FAILURE);
      }
   }

   r = pthread_join(prompt_thread, NULL);
   if (r != 0) {
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

void
test_prompt(void)
{
   {
      const char * line = "exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = " exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "     exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = " 	exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "	exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "	  	 exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit \n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit 	\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit  		  		\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "	  	exit		 	\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit ";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit		";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "   	     exit	  			  ";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	uploads\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	uploads";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	uploads\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	   	uploads\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = " info	  		  uploads	";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	 	uploads	\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }
}
