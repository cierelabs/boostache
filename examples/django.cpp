/**
*  \file django.cpp
*
*  A simple example of how to use boostache.
*
*  Copyright 2015 Jeroen Habraken
*  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
*
*  Distributed under the Boost Software License, Version 1.0. (See accompanying
*  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/django.hpp>
#include <boost/boostache/frontend/django/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/model/helper.hpp>
#include <iostream>
#include <sstream>

namespace boostache = boost::boostache;

struct my_node_t;
using map_t = std::map<std::string, my_node_t>;
using list_t = std::vector<my_node_t>;
struct my_node_t : boost::spirit::extended_variant<
    bool
    , std::string
    , map_t
    , list_t
>
{
    my_node_t() : base_type() {}
    my_node_t(bool rhs) : base_type(rhs) {}
    my_node_t(std::string const & rhs) : base_type(rhs) {}
    my_node_t(char const * rhs) : base_type(std::string{ rhs }) {}
    my_node_t(map_t const & rhs) : base_type(rhs) {}
    my_node_t(list_t const & rhs) : base_type(rhs) {}
};

int django1()
{
    // ------------------------------------------------------------------
    // Describe the input template. We are going to use django format.
    std::string input(
        "My name is {{another.name}}. "
        "{# This is a comment #}"
        "I am {{pet}} years old."
        "{%% if dontshowme %%}"
        "Yep"
        "{%% else %%}"
        "Nope"
        "{%% endif %%}\n");
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // The data model definition
    map_t data = {
        { "contacts" , map_t{ { "foo","gorp" } } },
    { "foo"      , "bar" },
    { "me"       , "Daniel" },
    { "pet"      , "turtles" },
    { "lpet"     , "Turtles" },
    { "people"   , list_t{ map_t{ { "name"    , "Tom" },
    { "job"     , "sweep floors" } },
    map_t{ { "name"    , "Sue" },
    { "job"     , "write code" } }
    }
    },
    { "title"      , "Multiple Mustaches" },
    { "comment"    , "this shouldn't be here" },
    { "showme"     , true },
    { "showme2"    , true },
    { "dontshowme" , false },
    { "next_more"  , "I like {{pet}}." },
    { "another"    , map_t{ { "name"   , "Sam" },
    { "ok"     , true },
    { "not_ok" , false } }
    },
    };
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // Load the template.
    // This parses the input and compiles the result. The return is the
    // compiled data structure
    using boostache::load_template;

    auto iter = input.begin();
    auto templ = load_template<boostache::format::django>(iter, input.end());
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // Apply the compiled template and the data model to generate the result
    std::stringstream stream;
    boostache::generate(stream, templ, data);
    // ------------------------------------------------------------------

    // print the result
    std::cout << stream.str();

    return 0;
}


extern std::string django2_input;

int django2()
{
   // ------------------------------------------------------------------
   // Describe the input template. We are going to use django format.
    // input moved to django_input.cpp
  // std::string input(
	 //  //"try fifdgrst.sefdshcond.another.name: {{fifdgrst.sefdshcond.another.name}}. \n"
	 //  //"try another.name: {{another.name}}.\n"
	 //  //"{%% for contact in people %%}"
	 //  //"My name is {{fifdgrst.sefdshcond.another.name}}.\n"
	 //  //"{%% endfor %%}"
	 //  //"-----------------------\n"
	 //  //"{%% for contact in people %%}"
	 //  //"{{contact.job}}\n"
	 //  ////"now from a nested context: people\n"
	 //  ////"try fifdgrst.sefdshcond.another.name: {{fifdgrst.sefdshcond.another.name}}. \n"
	 //  ////"try another.name: {{another.name}}.\n"
	 //  //"{%% endfor %%}"
	 //  "{%% for contact in people %%}"
	 //  //"{%% for more_contact in more_people %%}"
	 //  "{{contat.name}} does {{contact.job}}\n"
	 //  //"now from a nested context: people\n"
	 //  //"try fifdgrst.sefdshcond.another.name: {{fifdgrst.sefdshcond.another.name}}. \n"
	 //  //"try another.name: {{another.name}}.\n"
	 //  //"{%% endfor %%}"
	 //  "{%% endfor %%}"

	 //  //       "{# This is a comment #}"
  ////       "I am {{pet}} years old."
		////"{%% for contact in people %%}"
	 ////  "a contact\n"
	 ////  "My name is still {{another.name}}. \n\n"
	 ////  "{%% endfor %%}"
  ////       "{%% if dontshowme %%}"
  ////       "Yep"
  ////       "{%% else %%}"
  ////       "Nope"
  ////       "{%% endif %%}\n"
  // );
  // // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // The data model definition
   map_t data = {
      {"contacts" , map_t{{"foo","gorp"}}},
      {"foo"      , "bar"},
      {"me"       , "Daniel"},
      {"pet"      , "turtles"},
      {"lpet"     , "Turtles"},
      {"people"   , list_t{ map_t{{"name"    , "Tom"},
                                  {"job"     , "sweep floors"} },
                            map_t{{"name"    , "Sue"},
                                  {"job"     , "write code"} }
         }
      },
      {"title"      , "Multiple Mustaches"},
      {"comment"    , "this shouldn't be here"},
      {"showme"     , true},
      {"showme2"    , true},
      {"dontshowme" , false},
      {"next_more"  , "I like {{pet}}."},
      {"another"    , map_t{{"name"   , "Sam"},
                            {"ok"     , true },
                            {"not_ok" , false}}
      },
	  { "more_people"   , list_t{ map_t{ { "name"    , map_t{ { "first", "Peter" },{ "last", "Smith"}} },
	                                    { "job"     , "lazy person" } },
								   map_t{ { "name"    , map_t{ { "first", "Barbara" },{ "last", "Miller"}} },
								   { "job"     , "chef" } }
							   }
	  },
   { "nester" ,
       map_t{ { "nested",
            list_t{ map_t{ { "name"    , "Tom" },
                            { "job"     , "sweep floors" } },
                   map_t{ { "name"    , "Sue" },
                            { "job"     , "write code" } }
   } } } },
   };


   
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template.
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = django2_input.begin();
   auto templ = load_template<boostache::format::django>(iter, django2_input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to generate the result
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   // print the result
   std::cout << stream.str();

   return 0;
}

