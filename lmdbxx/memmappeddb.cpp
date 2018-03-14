/**
* @file memmappeddb.cpp
* @brief See memmappeddb.h for documentation.
*
* @authors David Nogueira
* @date  12/03/2018
*/
#include "memmappeddb.h"

//namespace lmdbxx {
bool MemoryMappedDatabase::open(const char * path, bool readonly) {
  bool retval = true;
  try {
    m_env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    m_open_path = path;
    m_open_path += "tti.mdb";
    m_env.open(m_open_path.c_str(), 0, 0664);
    m_readonly = readonly;
  } catch (lmdb::error& err) {
    std::cout << "LMDB exception Nr. " << err.code()
      << ", " << err.what() << std::endl;
    return false;
  } catch (const std::exception& err) {
    std::cout << "Exception " << err.what() << std::endl;
    return false;
  };
  return retval;
};

bool MemoryMappedDatabase::close() {
  flush();
  m_env.close();
  return true;
};

bool MemoryMappedDatabase::flush(bool force) {
  m_env.sync(force);
  return true;
};

bool MemoryMappedDatabase::check(const char* kbuf, size_t ksiz) {
  bool retval = true;
  try {
    auto rtxn = lmdb::txn::begin(m_env, nullptr, MDB_RDONLY);
    auto dbi = lmdb::dbi::open(rtxn, nullptr);
    lmdb::val key;
    key.assign(kbuf);
    retval = dbi.get(rtxn, key);
    rtxn.abort();
  } catch (lmdb::error& err) {
    std::cout << "LMDB exception Nr. " << err.code()
      << ", " << err.what() << std::endl;
    return false;
  } catch (const std::exception& err) {
    std::cout << "Exception " << err.what() << std::endl;
    return false;
  };
  return retval;
};

bool MemoryMappedDatabase::check(const std::string & kstr) {
  return check(kstr.c_str(),
               kstr.size() + 1);
};


bool MemoryMappedDatabase::get(const val& key,
                               val& data) {
  bool retval = true;
  try {
    auto rtxn = lmdb::txn::begin(m_env, nullptr, MDB_RDONLY);
    auto dbi = lmdb::dbi::open(rtxn, nullptr);
    dbi.get(rtxn, key, data);
    rtxn.abort();
  } catch (lmdb::error& err) {
    std::cout << "LMDB exception Nr. " << err.code()
      << ", " << err.what() << std::endl;
    return false;
  } catch (const std::exception& err) {
    std::cout << "Exception " << err.what() << std::endl;
    return false;
  };
  return retval;
};

bool MemoryMappedDatabase::get(const char* kbuf, size_t ksiz, char* vbuf, size_t max) {
  val key, value;
  key.assign(kbuf);

  bool retval = get(key, value);
  if (!retval)
    return false;

  memcpy(vbuf,
         value.data(),
         (std::min)(max, value.size()));
  return retval;
};
bool MemoryMappedDatabase::get(const char* kbuf, size_t ksiz, std::string * out) {
  val key, value;
  key.assign(kbuf);

  bool retval = get(key, value);
  if (!retval)
    return false;

  out->resize(value.size());
  memcpy(&((*out)[0]),
         value.data(),
         value.size());
  return retval;
};
bool MemoryMappedDatabase::get(const std::string & kstr,
                               std::string * vstr) {
  return get(kstr.c_str(),
             kstr.size() + 1,
             vstr);
}


bool MemoryMappedDatabase::set(const val& key, const val& data) {
  /* ,
     MDB_cmp_func* const cmp = nullptr);*/
  if (m_readonly)
    return false;
  bool retval = true;
  try {
    auto wtxn = lmdb::txn::begin(m_env);
    auto dbi =
      lmdb::dbi::open(wtxn);
    //if (cmp)
    //  dbi.set_compare(wtxn, cmp);
    dbi.put(wtxn, key, data);
    wtxn.commit();
  } catch (lmdb::error& err) {
    std::cout << "LMDB exception Nr. " << err.code()
      << ", " << err.what() << std::endl;
    return false;
  } catch (const std::exception& err) {
    std::cout << "Exception " << err.what() << std::endl;
    return false;
  };
  return retval;
};

bool MemoryMappedDatabase::set(const char* kbuf, size_t ksiz, const char* vbuf, size_t vsiz) {
  if (m_readonly)
    return false;
  val key, value;
  key.assign(kbuf);
  value.assign(vbuf);
  return set(key, value);
};

bool MemoryMappedDatabase::set(const std::string & kstr, const  std::string & vstr) {
  if (m_readonly)
    return false;
  return set(kstr.c_str(),
             kstr.size() + 1,
             vstr.c_str(),
             vstr.size() + 1);
};

bool MemoryMappedDatabase::remove(const char* kbuf, size_t ksiz) {
  if (m_readonly)
    return false;
  bool retval = true;
  try {
    auto wtxn = lmdb::txn::begin(m_env);
    auto dbi = lmdb::dbi::open(wtxn, nullptr);
    lmdb::val key;
    key.assign(kbuf);
    dbi.del(wtxn, key);
    wtxn.commit();
  } catch (lmdb::error& err) {
    std::cout << "LMDB exception Nr. " << err.code()
      << ", " << err.what() << std::endl;
    return false;
  } catch (const std::exception& err) {
    std::cout << "Exception " << err.what() << std::endl;
    return false;
  };
  return retval;
};
bool MemoryMappedDatabase::remove(const std::string & kstr) {
  return remove(kstr.c_str(),
                kstr.size() + 1);
};

size_t MemoryMappedDatabase::count() {
  try {
    auto wtxn = lmdb::txn::begin(m_env);
    auto dbi = lmdb::dbi::open(wtxn, nullptr);
    auto stats = dbi.stat(wtxn);
    return stats.ms_entries;
  } catch (lmdb::error& err) {
    std::cout << "LMDB exception Nr. " << err.code()
      << ", " << err.what() << std::endl;
    return -1;
  } catch (const std::exception& err) {
    std::cout << "Exception " << err.what() << std::endl;
    return -1;
  };
};

//}