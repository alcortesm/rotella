// -*- Mode: c++ -*-
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdexcept>

/* prevent compiler warnings about unused variables */
#define UNUSED(x) ((void)(x))

// string and int conversions
int         string_to_int(std::string & s) throw (std::invalid_argument);
std::string uint16_to_string(uint16_t i);
uint16_t    int_to_uint16(int i) throw (std::invalid_argument);

// c string handling
void to_lower(char * s);

// error throwing
void throw_fname_errno(const char * fname) throw (std::runtime_error);
void throw_fname(const char * fname, const char * reason) throw (std::runtime_error);

#endif // UTILS_H
