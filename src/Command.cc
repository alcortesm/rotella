#include "Command.h"
#include "utils.h"
#include "DebugStream.h"
extern DebugStream debug;

#include <string.h>
#include <stdlib.h>
#include <sstream>

using std::string;
using std::invalid_argument;
using std::stringstream;
using std::endl;

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
   to[len] = '\0';
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


// drop repeated spaces
// changes tabs into spaces
// and drop initial and ending spaces
// static
string
Command::clean_line(const string & rLine)
{
   if (rLine.size() == 0)
      return rLine;

   char * copy = (char *) calloc(rLine.size()+1, sizeof(char));
   if (!copy) {
      perror("calloc");
      exit(EXIT_FAILURE);
   }
   strcpy(copy, rLine.c_str());

   // turn tabs into spaces
   {
      char * aux = (char *) calloc(strlen(copy)+1, sizeof(char));
      if (!aux) {
         perror("calloc");
         exit(EXIT_FAILURE);
      }
      turn_tabs_into_spaces(copy, aux);
      strcpy(copy, aux);
      free(aux);
      //      debug << "clean_line() after turn tabs into spaces:\n\t\"" << std::string(copy) << "\"" << std::endl;
   }

   // drop repeated spaces
   {
      char * aux = (char *) calloc(strlen(copy)+1, sizeof(char));
      if (!aux) {
         perror("calloc");
         exit(EXIT_FAILURE);
      }
      drop_repeated_spaces(copy, aux);
      strcpy(copy, aux);
      free(aux);
      //      debug << "clean_line() after dropping repeated spaces:\n\t\"" << std::string(copy) << "\"" << std::endl;
   }


   // drop '\n' at the end
   {
      char * aux = (char *) calloc(strlen(copy)+1, sizeof(char));
      if (!aux) {
         perror("calloc");
         exit(EXIT_FAILURE);
      }
      drop_ending_eol(copy, aux);
      strcpy(copy, aux);
      free(aux);
      //      debug << "clean_line() after dropping '\\n' at the end:\n\t\"" << std::string(copy) << "\"" << std::endl;
   }

   // drop space at the start
   {
      char * aux = (char *) calloc(strlen(copy)+1, sizeof(char));
      if (!aux) {
         perror("calloc");
         exit(EXIT_FAILURE);
      }
      drop_space_at_start(copy, aux);
      strcpy(copy, aux);
      free(aux);
      //      debug << "clean_line() after dropping space at start:\n\t\"" << std::string(copy) << "\"" << std::endl;
   }

   // drop space at the end
   {
      char * aux = (char *) calloc(strlen(copy)+1, sizeof(char));
      if (!aux) {
         perror("calloc");
         exit(EXIT_FAILURE);
      }
      drop_space_at_end(copy, aux);
      strcpy(copy, aux);
      free(aux);
      //      debug << "clean_line() after dropping space at end:\n\t\"" << std::string(copy) << "\"" << std::endl;
   }

   //   debug << "clean_line() puts \"" << std::string(clean) << "\"" << std::endl;

   string r = string(copy);
   free(copy);
   return r;
}

Command::Command(const string& rLine) throw (invalid_argument)
{
   string clean = clean_line(rLine);

   // tokenize clean into the vector mArg
   string tmp;
   stringstream ss(clean);
   ss >> mVerb; // the first word is the verb
   while (ss >> tmp) // then the arguments
      mArg.push_back(tmp);
   
   mNumArgs = mArg.size();
   debug << "Creating [Command \"" << (Command) *this << "\"]" << endl;
}

const string&
Command::rVerb() const
{
   return mVerb;
}

unsigned int
Command::NumArgs() const
{
   return mNumArgs;
}

const string&
Command::rArg(unsigned int i) const throw (invalid_argument)
{
   if (i >= mNumArgs)
      throw invalid_argument("no such argument");
   return mArg[i];
}

std::ostream &
operator<<(std::ostream & os, Command c)
{
   os << c.rVerb();
   for (unsigned int i=0; i<c.NumArgs(); i++) os << " " << c.rArg(i);
   return os;
}

std::istream &
operator>>(std::istream & is, Command & c)
{
   UNUSED(c);
   return is;
}

bool
operator==(Command & a, Command & b)
{
   UNUSED(a);
   UNUSED(b);
   return true;
}

bool
operator!=(Command & a, Command & b)
{
   return !(a==b);
}
