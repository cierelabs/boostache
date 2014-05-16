//invoice.cpp

#include <cstdlib>
#include <iostream>  
#include <string>
#include <ctemplate/template.h>  

int main() {


  ctemplate::TemplateDictionary dict("example");
  dict["COMPANY"] = "Joes Hardware";

  std::string output;
  ctemplate::ExpandTemplate("invoice.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);
  std::cout << output;
  return 0;
}
