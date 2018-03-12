#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <lmdbxx/lmdb++.h>

class MemoryMappedDatabase {
public:
  MemoryMappedDatabase() : m_env(lmdb::env::create()) {}
  bool open(const char * path, bool readonly = false) {
    m_env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    m_open_path = path;
    m_open_path += "tti.mdb";
    m_env.open(m_open_path.c_str(), 0, 0664);
    m_readonly = readonly;
    return true;
  };
  bool close() {};
  bool flush() {};
  //******************
  //****** GET *******
  //******************
  bool get(const char* kbuf, size_t ksiz, char* vbuf, size_t max) {
    auto rtxn = lmdb::txn::begin(m_env, nullptr, MDB_RDONLY);
    auto dbi = lmdb::dbi::open(rtxn, nullptr);
    lmdb::val key, value;
    key.assign( kbuf);
    dbi.get(rtxn, key, value);

    memcpy(vbuf,
           value.data(),
           (std::min)(max, value.size()));
    rtxn.abort();
  };
  bool get(const char* kbuf, size_t ksiz, std::string * out) {
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
  bool get(const std::string & kstr,
           std::string * vstr) {
    return get(kstr.c_str(),
               kstr.size() + 1,
               vstr);
  }
  //******************
  //****** SET *******
  //******************
  bool set(const char* kbuf, size_t ksiz, const char* vbuf, size_t vsiz) {
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
  bool set(const std::string & kstr, const  std::string & vstr) {
    return set(kstr.c_str(),
               kstr.size() + 1,
               vstr.c_str(),
               vstr.size() + 1);
  };
  //******************
  //***** CHECK ******
  //******************
  //bool check(const char* kbuf, size_t ksiz); //TODO: implementation missing
  //bool check(const std::string & kstr){
  //  return check(kstr.c_str(),
  //             kstr.size() + 1);
  //};
  //******************
  //***** COUNT ******
  //******************
  //size_t count(); //TODO: implementation missing
protected:
  lmdb::env m_env;
  std::string m_open_path;
  bool m_readonly;
};

int main() {
  /* Create and open the LMDB environment: */
  MemoryMappedDatabase mmd;
  mmd.open("./");

  std::cout << std::endl << "Insert some values" << std::endl << std::endl;
  /* Insert some key/value pairs in a write transaction: */
  {
    mmd.set("username", "jhacker");
    mmd.set("email", "jhacker@example.org");
    mmd.set("fullname", "J. Random Hacker");
  }

  std::cout << std::endl << "Read values" << std::endl << std::endl;
  {
    {
      std::string value;
      mmd.get("username", &value);
      std::cout << "Read \"username\" value: " << value  << std::endl;
    }
    {
      std::string value;
      mmd.get("email", &value);
      std::cout << "Read \"email\" value: " << value  << std::endl;
    }
    {
      std::string value;
      mmd.get("fullname", &value);
      std::cout << "Read \"fullname\" value: " << value  << std::endl;
    }
  }


  std::cout << std::endl << "Update some values" << std::endl << std::endl;
  {
    mmd.set("fullname", "J. Random \'Jocking\' Hacker");
  }
  
  std::cout << std::endl << "Read values" << std::endl << std::endl;
  {
    {
      std::string value;
      mmd.get("username", &value);
      std::cout << "Read \"username\" value: " << value  << std::endl;
    }
    {
      std::string value;
      mmd.get("email", &value);
      std::cout << "Read \"email\" value: " << value  << std::endl;
    }
    {
      std::string value;
      mmd.get("fullname", &value);
      std::cout << "Read \"fullname\" value: " << value<< std::endl;
    }
  }

  std::cout << std::endl << "Read single value" << std::endl << std::endl;
  {
    std::string value;
    mmd.get("fullname", &value);
    std::cout << "Read \"fullname\" value: " << value << std::endl << std::endl;
  }
  /* The enviroment is closed automatically. */
  return EXIT_SUCCESS;
}
