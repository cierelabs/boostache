/**
*  \file django.cpp
*
*  A simple example of how to use boostache.
*
*  Copyright 2017, 2018 Tobias Loew
*
*  Distributed under the Boost Software License, Version 1.0. (See accompanying
*  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <string>

// ------------------------------------------------------------------
// Describe the input template. We are going to use django format.
 std::string django2_input(

#if 0
  //"try fifdgrst.sefdshcond.another.name: {{fifdgrst.sefdshcond.another.name}}. \n"
  //"try another.name: {{another.name}}.\n"
  //"{%% for contact in people %%}"
  //"My name is {{fifdgrst.sefdshcond.another.name}}.\n"
  //"{%% endfor %%}"
  //"-----------------------\n"
  //"{%% for contact in people %%}"
  //"{{contact.job}}\n"
  ////"now from a nested context: people\n"
  ////"try fifdgrst.sefdshcond.another.name: {{fifdgrst.sefdshcond.another.name}}. \n"
  ////"try another.name: {{another.name}}.\n"
  //"{%% endfor %%}"
  "{%% for contact in people %%}"
  //"{%% for more_contact in more_people %%}"
  "{{contact.name}} does {{contact.job}}\n"
  //"now from a nested context: people\n"
  //"try fifdgrst.sefdshcond.another.name: {{fifdgrst.sefdshcond.another.name}}. \n"
  //"try another.name: {{another.name}}.\n"
  //"{%% endfor %%}"
  "{%% endfor %%}"

  //       "{# This is a comment #}"
//       "I am {{pet}} years old."
//"{%% for contact in people %%}"
//  "a contact\n"
//  "My name is still {{another.name}}. \n\n"
//  "{%% endfor %%}"
//       "{%% if dontshowme %%}"
//       "Yep"
//       "{%% else %%}"
//       "Nope"
//       "{%% endif %%}\n"

#endif
"{{people.2.name}}\n"
"{%% for contact2 in people %%}"
"{{contact2.name}} does {{contact2.job}}\n"
"{%% endfor %%}"

//" WAIT!!!\n"
//"{%% for contact in more_people %%}"
////"{{contact.name.first}} is a {{contact.job}}\n"
//
//"{%% for contact2 in people %%}"
//"{{contact.name.first}} does {{contact2.job}}\n"
//"{%% endfor %%}"
//
//"{%% endfor %%}"

//" NOW!!!\n"
//"{{me}}\n"
//"{{another.name}}\n"
//"{%% if another.name %%}"
//"There are named-people.\n"
//"{%% else %%}"
//"No athletes.\n"
//"{%% endif %%}"
//
//
//"{{more_people}}\n"
//"{%% for bla in nester.nested %%}"
//"{%% if bla.name %%}"
//"There are named-people.\n"
//"{%% else %%}"
//"No athletes.\n"
//"{%% endif %%}"
//"{{bla.name}} does {{bla.job}}\n"
//"{%% endfor %%}"
//"{%% if foogjgh %%}"
//"There are people.\n"
//"{%% else %%}"
// "No athletes.\n"
// "{%% endif %%}"
);
