#include "Command.h"
#include "DebugStream.h"
extern DebugStream debug;

#include <iostream>
#include <cassert>
#include <sstream>

using std::invalid_argument;
using std::string;
using std::endl;
using std::cerr;
using std::stringstream;

// static
void
Command::Test()
{
   // test clean_line
   {
      string s = string("");
      string result = string("");
      string clean  = clean_line(s);
      assert(clean == result);
   }

   {
      string s = string("hello");
      string result = string("hello");
      string clean  = clean_line(s);
      assert(clean == result);
   }

   {
      string s = string("hello first");
      string result = string("hello first");
      string clean  = clean_line(s);
      assert(clean == result);
   }

   {
      string s = string("hello first\n");
      string result = string("hello first");
      string clean  = clean_line(s);
      assert(clean == result);
   }

   {
      string s = string("hello first second");
      string result = string("hello first second");
      string clean  = clean_line(s);
      assert(clean == result);
   }

   {
      string s = string("hello 1 2 3 4 5 6 7 8 ");
      string result = string("hello 1 2 3 4 5 6 7 8");
      string clean  = clean_line(s);
      assert(clean == result);
   }

   {
      string s = string("    		hello   first 	second 	  \n");
      string result = string("hello first second");
      string clean  = clean_line(s);
      assert(clean == result);
   }

   // test ctor, accesors and operator<<
   // find bla  -> OK
   {
      Command * p_c;
      {
         string verb = "find";
         string arg0 = "bla";
         string s = string(verb + string(" ") + arg0);
         string result = string(verb + string(" ") + arg0);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 1);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(1);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // get 3  -> OK
   {
      Command * p_c;
      {
         string verb = "get";
         string arg0 = "3";
         string s = string(verb + string(" ") + arg0);
         string result = string(verb + string(" ") + arg0);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 1);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(1);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info  -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string s = string(verb);
         string result = string(verb);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 0);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            p_c->rArg(0);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(1);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info uploads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "uploads";
         string s = string(verb + string(" ") + arg0);
         string result = string(verb + string(" ") + arg0);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 1);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(1);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info downloads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "downloads";
         string s = string(verb + string(" ") + arg0);
         string result = string(verb + string(" ") + arg0);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 1);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(1);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info hosts -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "hosts";
         string s = string(verb + string(" ") + arg0);
         string result = string(verb + string(" ") + arg0);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 1);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(1);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info uploads downloads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "uploads";
         string arg1 = "downloads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 2);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(2);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info uploads hosts -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "uploads";
         string arg1 = "hosts";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 2);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(2);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info downloadsuploads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "downlodas";
         string arg1 = "uploads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 2);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(2);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info downloads hosts -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "downloads";
         string arg1 = "hosts";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 2);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(2);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info hosts uploads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "hosts";
         string arg1 = "uploads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 2);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(2);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info hosts downloads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "hosts";
         string arg1 = "downloads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 2);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(2);
            assert(false);
         } catch (invalid_argument & ia) {
         }

         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info uploads downloads hosts -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "uploads";
         string arg1 = "downloads";
         string arg2 = "hosts";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 3);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(2) == arg2);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(3);
            assert(false);
         } catch (invalid_argument & ia) {
         }
         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info uploads hosts downloads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "uploads";
         string arg1 = "hosts";
         string arg2 = "downloads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 3);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(2) == arg2);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(3);
            assert(false);
         } catch (invalid_argument & ia) {
         }
         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info downloads uploads hosts -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "downloads";
         string arg1 = "uploads";
         string arg2 = "hosts";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 3);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(2) == arg2);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(3);
            assert(false);
         } catch (invalid_argument & ia) {
         }
         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info downloads hosts uploads-> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "downloads";
         string arg1 = "hosts";
         string arg2 = "uploads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 3);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(2) == arg2);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(3);
            assert(false);
         } catch (invalid_argument & ia) {
         }
         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info hosts uploads downloads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "hosts";
         string arg1 = "uploads";
         string arg2 = "downloads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 3);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(2) == arg2);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(3);
            assert(false);
         } catch (invalid_argument & ia) {
         }
         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   // info hosts downloads uploads -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "hosts";
         string arg1 = "downloads";
         string arg2 = "uploads";
         string s = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 3);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(2) == arg2);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(3);
            assert(false);
         } catch (invalid_argument & ia) {
         }
         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }
   //   info   hosts       downloads  uploads   -> OK
   {
      Command * p_c;
      {
         string verb = "info";
         string arg0 = "hosts";
         string arg1 = "downloads";
         string arg2 = "uploads";
         string s = string(string(" 	") + verb + string("   		") + arg0
                           + string(" 			   ") + arg1
                           + string("    ") + arg2 + string("  	\n"));
         string result = string(verb + string(" ") + arg0 + string(" ") + arg1 + string(" ") + arg2);
         try {
            p_c = new Command(s);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         assert(p_c->rVerb() == verb); 
         assert(p_c->NumArgs() == 3);

         stringstream ss(stringstream::out);
         ss << *p_c;
         //         debug << ss.str() << endl;
         assert(ss.str().compare(result) == 0);

         try {
            assert(p_c->rArg(0) == arg0);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(1) == arg1);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            assert(p_c->rArg(2) == arg2);
         } catch (invalid_argument & ia) {
            assert(false);
         }

         try {
            p_c->rArg(3);
            assert(false);
         } catch (invalid_argument & ia) {
         }
         try {
            p_c->rArg(123);
            assert(false);
         } catch (invalid_argument & ia) {
         }
      }
      delete p_c;
   }

   return;
}
