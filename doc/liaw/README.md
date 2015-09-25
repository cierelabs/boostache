Title: 2014 Library in a Week - c++ templating engine

Library in a week 2014 will attempt to build a C++ template engine library.  Templating 
engines are very popular libraries that provide a way to separate an application 
from output.  Often the output is a html or text that may be developed by an 
non-programmer or other development team.


Consider the following program which uses the ctemplate library:

    //user.tpl
    Hello {{NAME}}!

	//user.cpp
    #include <cstdlib>
    #include <iostream>  
    #include <string>
    #include <ctemplate/template.h>  

    int main() {
	  std::string user = getenv("USER");
      ctemplate::TemplateDictionary dict("example");
      dict["NAME"] = user;
      std::string output;
      ctemplate::ExpandTemplate("example.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);
      std::cout << output;
      return 0;
    }
	

In this example the output is simple text, but the template file could be replaced
with html or json and the application would not need to be changed.

A templating engine has two languages - the markup language the allows 
a text template to be filled in by an application and the library language 
for the application to be developed.  In our case the library language will
be C++11.

The workshop will work like this.  On day 1 I will provide motivation
and an overview of the library development. We will split into individuals
and groups to attack various aspects of the library and markup language 
design.  Some groups may look at existing libraries for inspiration providing
presentations on the best aspects of the other libraries. From that point
forward it will be up to the group to direct the remainder of the workshop.

Collaboration Tools: 

Library in a week 2014 will use a git repository for collaboration and sharing
of information.

git clone https://github.com/JeffGarland/liaw2014.git

Also the group will use an email list for discussions beyond face to face
meetings.

Level: Beginner to Expert 

Attendees should have a basic background in C++. 


