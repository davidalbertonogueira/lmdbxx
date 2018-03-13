
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <lmdbxx\memmappeddb.h>

int main() {
  /* Create and open the LMDB environment: */
  MemoryMappedDatabase mmd;
  mmd.open("./");

  std::cout << std::endl << "Insert some values" << std::endl << std::endl;
  /* Insert some key/value pairs in a write transaction: */
  {
    std::cout << "No. records: " << mmd.count() << std::endl;
    mmd.set("username", "jhacker");
    std::cout << "No. records: " << mmd.count() << std::endl;
    mmd.set("email", "jhacker@example.org");
    std::cout << "No. records: " << mmd.count() << std::endl;
    mmd.set("fullname", "J. Random Hacker");
    std::cout << "No. records: " << mmd.count() << std::endl;
  }

  std::cout << std::endl << "Read values" << std::endl << std::endl;
  {
    std::string value1; mmd.get("username", &value1);
    std::cout << "Read \"username\" value: " << value1 << std::endl;
    std::string value2; mmd.get("email", &value2);
    std::cout << "Read \"email\" value: " << value2 << std::endl;
    std::string value3; mmd.get("fullname", &value3);
    std::cout << "Read \"fullname\" value: " << value3 << std::endl;
  }

  std::cout << "\"fullname\" exists? = " << std::boolalpha << mmd.check("fullname") << std::endl;
  std::cout << "\"shortname\" exists? = " << std::boolalpha << mmd.check("shortname") << std::endl << std::endl;


  std::cout << std::endl << "Update some values" << std::endl << std::endl;
  mmd.set("fullname", "J. Random \'Jocking\' Hacker");
  std::cout << "No. records: " << mmd.count() << std::endl;

  std::cout << std::endl << "Read values" << std::endl << std::endl;
  {
    std::string value1; mmd.get("username", &value1);
    std::cout << "Read \"username\" value: " << value1 << std::endl;
    std::string value2; mmd.get("email", &value2);
    std::cout << "Read \"email\" value: " << value2 << std::endl;
    std::string value3; mmd.get("fullname", &value3);
    std::cout << "Read \"fullname\" value: " << value3 << std::endl;
  }

  std::cout << std::endl << "Read single value" << std::endl << std::endl;
  {
    std::string value; mmd.get("fullname", &value);
    std::cout << "Read \"fullname\" value: " << value << std::endl << std::endl;
  }

  std::cout << std::endl << "Remove one key\/pair" << std::endl << std::endl;
  mmd.remove("fullname");
  std::cout << "No. records: " << mmd.count() << std::endl;

  return EXIT_SUCCESS;
}
