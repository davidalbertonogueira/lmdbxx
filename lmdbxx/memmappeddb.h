/**
* @file memmappeddb.h
* @brief Memory Mapped DB.
*
* @authors David Nogueira
* @date  12/03/2018
*/
#ifndef MEMMAPPEDDB_H
#define MEMMAPPEDDB_H
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "lmdb++.h"

//namespace lmdbxx {
class MemoryMappedDatabase {
public:
  MemoryMappedDatabase() : m_env(lmdb::env::create()) {}
  //******************
  //****** OPEN ******
  //******************
  bool open(const char * path, bool readonly = false);

  //******************
  //***** CLOSE ******
  //******************
  bool close();

  //******************
  //***** FLUSH ******
  //******************
  bool flush(bool force=false);

  //******************
  //***** CHECK ******
  //******************
  bool check(const char* kbuf, size_t ksiz);
  bool check(const std::string & kstr);

  //******************
  //****** GET *******
  //******************
  bool get(const char* kbuf, size_t ksiz, char* vbuf, size_t max);
  bool get(const char* kbuf, size_t ksiz, std::string * out);
  bool get(const std::string & kstr, std::string * vstr);
  //******************
  //****** SET *******
  //******************
  bool set(const char* kbuf, size_t ksiz, const char* vbuf, size_t vsiz);
  bool set(const std::string & kstr, const  std::string & vstr);

  //******************
  //***** REMOVE *****
  //******************
  bool remove(const char* kbuf, size_t ksiz);
  bool remove(const std::string & kstr);


  //******************
  //***** COUNT ******
  //******************
  size_t count();


  //******************
  //**** ITERATOR **** 
  //******************



protected:
  lmdb::env m_env;
  std::string m_open_path;
  bool m_readonly;
};

//}
#endif
