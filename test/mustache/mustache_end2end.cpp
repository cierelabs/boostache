/**
 *  \file test/mustache/mustache_compiler.cpp
 *
 *  Link with shared/parser_test to utilize loading and parsing test files.
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <boost/test/unit_test.hpp>
#include <boost/spirit/include/support_extended_variant.hpp>

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/frontend/file_mapper.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/stache.hpp>
#include <boost/boostache/model/stache_model.hpp>
#include <boost/boostache/model/helper.hpp>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

namespace boostache = boost::boostache;
namespace fe = boost::boostache::frontend;


struct my_node_t;
using map_t = std::map<std::string,my_node_t>;
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
   my_node_t(char const * rhs) : base_type(std::string{rhs}) {}
   my_node_t(map_t const & rhs) : base_type(rhs) {}
   my_node_t(list_t const & rhs) : base_type(rhs) {}
};


std::string print_ast(std::string const & filename)
{
   std::ifstream file(filename.c_str());
   if(!file)
   {
      BOOST_CHECK_MESSAGE(false, "Failed to open " << filename);
      return "";
   }

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
      // KingDuckZ add
      {"hello"      , list_t{ map_t{{"items",
                                     list_t{ map_t{{"name", "red"}},
                                             map_t{{"name", "green"}},
                                             map_t{{"name", "blue"}} } } },
                              map_t{{"items",
                                     list_t{ map_t{{"name", "yellow"}},
                                             map_t{{"name", "black"}},
                                             map_t{{"name", "white"}} } } } }
      },
      {"lol"        , "hahahah" }
   };
   // ------------------------------------------------------------------

   // load and compile the template
   auto templ = boostache::load_template<boostache::format::stache>(file, boostache::frontend::file_mapper<char>());
   std::ostringstream stream;
   boostache::generate(stream, templ, data);
   return stream.str();
}


std::string test_dir = "mustache/end2end_test_dir";
