#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/boostache/model/stache_model.hpp>
#include <boost/boostache/model/stache_model_printer.hpp>
#include <boost/boostache/simple_parser.hpp>

using namespace boost::boostache::frontend;
using boost::boostache::model::stache_model;
using boost::boostache::model::stache_model_vector;
using boost::boostache::model::stache_string_function;
using boost::boostache::model::stache_bool_function;


namespace
{
   std::string print( const stache::ast::root& ast
                    , const stache_model& model)
   {
      std::ostringstream out;
      boost::boostache::model::ast::print(out, ast, model);
      return out.str();
   }

   stache::ast::root parse(const std::string& text)
   {
      stache::ast::root ast;
      if( !boost::boostache::simple_parse_template(text, ast) )
      {
         throw std::runtime_error("Parse failed");
      }
      return ast;
   }
}

BOOST_AUTO_TEST_CASE(test_model_build)
{
   stache_model model;
   model["NAME"] = "Bob";
   model["LOCATION"] = stache_model { { "CITY", "Aspen" }, { "STATE", "Colorado" }, { "COUNTRY", "United States" } };
   model["GENERATED_STRING"] = stache_string_function { []() { return "abc"; } };
   model["GENERATED_BOOL"] = stache_bool_function { []() { return true; } };

   BOOST_CHECK_EQUAL("Bob", boost::get<std::string>(model["NAME"]));
   BOOST_CHECK_THROW(boost::get<int>(model["NAME"]), boost::bad_get);
   BOOST_CHECK_EQUAL("Aspen", boost::get<std::string>(boost::get<stache_model>(model["LOCATION"])["CITY"]));
   BOOST_CHECK_EQUAL("abc", boost::get<stache_string_function>(model["GENERATED_STRING"])());
   BOOST_CHECK_EQUAL(true, boost::get<stache_bool_function>(model["GENERATED_BOOL"])());
}

BOOST_AUTO_TEST_CASE(test_simple_model_formatting)
{
   stache_model model;
   model["NAME"] = "Boosties";
   stache::ast::root ast = parse("Hello, {{NAME}}!");
   std::string result = print(ast, model);
   BOOST_CHECK_EQUAL("Hello, Boosties!", result);
}

BOOST_AUTO_TEST_CASE(test_section_printing)
{
   stache_model model;
   model["USER"] = stache_model_vector {
      stache_model {
         { "NAME", "Bob" },
         { "LOCATION", "Earth" },
         { "FAVORITES", stache_model {
               { "FOOD", "Pizza" },
               { "MUSIC", "Classical" }
            }
         }
      },
      stache_model {
         { "NAME", "George" },
         { "LOCATION", "Mars" },
         { "FAVORITES", stache_model {
               { "FOOD", "Red sand" },
               { "MUSIC", "Wind" }
            }
         }
      }
   };
   stache::ast::root ast = parse(
      "{{#USER}}"
      "user.name={{NAME}}\n"
      "user.location={{LOCATION}}\n"
      "{{#FAVORITES}}"
      "user.favorite.food={{FOOD}}\n"
      "user.favorite.music={{MUSIC}}\n"
      "{{/FAVORITES}}"
      "{{/USER}}");

   BOOST_CHECK_EQUAL(
      "user.name=Bob\n"
      "user.location=Earth\n"
      "user.favorite.food=Pizza\n"
      "user.favorite.music=Classical\n"
      "user.name=George\n"
      "user.location=Mars\n"
      "user.favorite.food=Red sand\n"
      "user.favorite.music=Wind\n",
      print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_inversion_on_empty_model_simple)
{
   stache::ast::root ast = parse("{{^entries}}No entries found!\n{{/entries}}");
   BOOST_CHECK_EQUAL("No entries found!\n", print(ast, stache_model()));
}

BOOST_AUTO_TEST_CASE(test_mixed_inversion_on_empty_model)
{
   stache::ast::root ast = parse(
      "{{#entries}}Have entry with value {{value}}\n{{/entries}}"
      "{{^entries}}No entries found!\n{{/entries}}");

   BOOST_CHECK_EQUAL("No entries found!\n", print(ast, stache_model()));
}

BOOST_AUTO_TEST_CASE(test_non_empty_model_simple)
{
   stache::ast::root ast = parse("{{#entries}}Have entry with value {{value}}\n{{/entries}}");

   stache_model model = {
      { "entries", stache_model_vector {
            stache_model { { "value", "abc"} },
            stache_model { { "value", "def"} }
         }
      }
   };

   BOOST_CHECK_EQUAL(
      "Have entry with value abc\n"
      "Have entry with value def\n",
      print(ast, model) );
}

BOOST_AUTO_TEST_CASE(test_mixed_inversion_on_non_empty_model)
{
   stache::ast::root ast = parse(
      "{{#entries}}Have entry with value {{value}}\n{{/entries}}"
      "{{^entries}}No entries found!\n{{/entries}}");

   stache_model model = {
      { "entries", stache_model_vector {
            stache_model { { "value", "abc"} },
            stache_model { { "value", "def"} }
         }
      }
   };

   BOOST_CHECK_EQUAL(
      "Have entry with value abc\n"
      "Have entry with value def\n",
      print(ast, model) );
}
