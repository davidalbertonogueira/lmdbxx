
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
    mmd.set("username", "jhacker");
    mmd.set("email", "jhacker@example.org");
    mmd.set("fullname", "J. Random Hacker");
  }

  std::cout << std::endl << "Read values" << std::endl << std::endl;
  {
    {
      std::string value;
      mmd.get("username", &value);
      std::cout << "Read \"username\" value: " << value << std::endl;
    }
    {
      std::string value;
      mmd.get("email", &value);
      std::cout << "Read \"email\" value: " << value << std::endl;
    }
    {
      std::string value;
      mmd.get("fullname", &value);
      std::cout << "Read \"fullname\" value: " << value << std::endl;
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
      std::cout << "Read \"username\" value: " << value << std::endl;
    }
    {
      std::string value;
      mmd.get("email", &value);
      std::cout << "Read \"email\" value: " << value << std::endl;
    }
    {
      std::string value;
      mmd.get("fullname", &value);
      std::cout << "Read \"fullname\" value: " << value << std::endl;
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
