/**
* @file memmappeddb.cpp
* @brief See memmappeddb.h for documentation.
*
* @authors David Nogueira
* @date  12/03/2018
*/
#include "memmappeddb.h"

//namespace lmdbxx {
bool MemoryMappedDatabase::open(const char * path, bool readonly ) {
  m_env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
  m_open_path = path;
  m_open_path += "tti.mdb";
  m_env.open(m_open_path.c_str(), 0, 0664);
  m_readonly = readonly;
  return true;
};

bool MemoryMappedDatabase::close() {
  //TODO: implementation missing
};

bool MemoryMappedDatabase::flush() {
  //TODO: implementation missing
};

bool MemoryMappedDatabase::check(const char* kbuf, size_t ksiz) {
  //TODO: implementation missing
};
bool MemoryMappedDatabase::check(const std::string & kstr) {
  return check(kstr.c_str(),
               kstr.size() + 1);
};


bool MemoryMappedDatabase::get(const char* kbuf, size_t ksiz, char* vbuf, size_t max) {
  auto rtxn = lmdb::txn::begin(m_env, nullptr, MDB_RDONLY);
  auto dbi = lmdb::dbi::open(rtxn, nullptr);
  lmdb::val key, value;
  key.assign(kbuf);
  dbi.get(rtxn, key, value);

  memcpy(vbuf,
         value.data(),
         (std::min)(max, value.size()));
  rtxn.abort();
};
bool MemoryMappedDatabase::get(const char* kbuf, size_t ksiz, std::string * out) {
  auto rtxn = lmdb::txn::begin(m_env, nullptr, MDB_RDONLY);
  auto dbi = lmdb::dbi::open(rtxn, nullptr);
  lmdb::val key, value;
  key.assign(kbuf);
  dbi.get(rtxn, key, value);
  out->resize(value.size());
  memcpy(&((*out)[0]),
         value.data(),
         value.size());
  rtxn.abort();
};
bool MemoryMappedDatabase::get(const std::string & kstr,
                               std::string * vstr) {
  return get(kstr.c_str(),
             kstr.size() + 1,
             vstr);
}
bool MemoryMappedDatabase::set(const char* kbuf, size_t ksiz, const char* vbuf, size_t vsiz) {
  if (m_readonly)
    return false;
  auto wtxn = lmdb::txn::begin(m_env);
  auto dbi = lmdb::dbi::open(wtxn, nullptr);
  lmdb::val key, value;
  key.assign(kbuf);
  value.assign(vbuf);
  dbi.put(wtxn, key, value);
  wtxn.commit();
};
bool MemoryMappedDatabase::set(const std::string & kstr, const  std::string & vstr) {
  return set(kstr.c_str(),
             kstr.size() + 1,
             vstr.c_str(),
             vstr.size() + 1);
};

bool MemoryMappedDatabase::remove(const char* kbuf, size_t ksiz) {
  //TODO: implementation missing  
};
bool MemoryMappedDatabase::remove(const std::string & kstr) {
  return remove(kstr.c_str(),
                kstr.size() + 1);
};

size_t MemoryMappedDatabase::count() {
  //TODO: implementation missing  
};

//}