//hello.cpp

#include <cstdlib>
#include <iostream>  
#include <string>
#include <ctemplate/template.h>  

int main() {
  std::string user = getenv("USER");
  ctemplate::TemplateDictionary dict("example");
  dict["NAME"] = user;
  std::string output;
  ctemplate::ExpandTemplate("hello.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);
  std::cout << output;
  return 0;
}
