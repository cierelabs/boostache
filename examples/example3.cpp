/**
 *  \file example2.cpp
 *
 *  This is almost like example2, except we are going to
 *  clean things up with a variant.
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/stache.hpp>
#include <boost/boostache/model/helper.hpp>
#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <variant>

using vv = std::variant<int>;

namespace boostache = boost::boostache;


// -------------------------------------------------------
// From example2 we realize that if we had a data structure
// that could map strings to a list, string, or even another
// map then we could represent a much more complete invoice.
// A key/value map with the value being able to store these
// different types starts to look like an object in some
// languages.
// This is fairly simple to do with a variant. We are going
// to use the spirit version of it because it makes
// recursive structures easier to describe.
//

struct value_t;
using object_t = std::map<std::string, value_t>;
using list_t = std::vector<value_t>;

struct user_rendered_t {
    std::string some_variable;
};

struct value_t : boost::spirit::extended_variant< double, std::string
                                                , object_t
                                                , list_t
                                                , user_rendered_t

                                                >
{
   value_t() : base_type() {}
   value_t(std::string const & rhs) : base_type(rhs) {}
   value_t(char const * rhs) : base_type(std::string{rhs}) {}
   value_t(object_t const & rhs) : base_type(rhs) {}
   value_t(list_t const & rhs) : base_type(rhs) {}
   value_t(double const & rhs) : base_type(rhs) {}
   value_t(user_rendered_t const & rhs) : base_type(rhs) {}
};
// -------------------------------------------------------

// test for user-defined rendering
namespace boost {
    namespace boostache {
        namespace extension
        {
            // renders the whole type
            template< typename Stream>
            void render(Stream & stream, user_rendered_t const & context) {
                stream << "user_defined rendering";
            }

            // outer test of type (#"user_rendered_t")
            bool test(user_rendered_t const & context){
                // the result of a #-test
                return true;
            }

            // inner test of name in user_rendered_t (#-test)
            boost::optional<bool> test_tag(user_rendered_t const & context, std::string const & tag) {
                if (tag == "invoice_number") {
                    return boost::none;
                }
                return tag == "invoice_number";
                //return tag == "invoice_number";
            }

            // inner rendering of name in user_rendered_t 
            template< typename Stream, typename Stack, typename Global>
            bool render_name(Stream & stream, user_rendered_t const & context, Stack const* stack, Global const* global, std::string const & name) {
                stream << "user_rendered_t rendering of: " << name;
                return true;
            }

        }
    }
}


int example3()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input( 
                      "Invoice {{invoice_number}}"
                      "\n"
                      "{{# company}}"
                      "Company: {{name}}\n"
					   "Invoice again: {{invoice_number}}\n"
					   "{{#invoice_number}}\n"
					   "has invoice\n"
					   "{{/invoice_number}}\n"
					   "{{#paied}}\n"
					   "has paied\n"
					   "{{/paied}}\n"
					   "{{^paied}}\n"
					   "has not paied\n"
					   "{{/paied}}\n"
					   "         {{street}}\n"
                      "         {{city}}, {{state}}  {{zip}}\n"
                      "{{/ company}}"
                      "------------------------------------------------\n"
                      "{{#lines}}"
                      "  just to be sure that we mentioned the invoice number: {{invoice_number}}\n"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "  content of add_lines ...\n"
                      "  {{add_lines}}\n "
                      "  ... and the invoice_number for each element of add_lines\n"
                      "{{#add_lines}} "
       "{{#invoice_number}}\n"
       "first has invoice\n"
       "{{/invoice_number}}\n"
       "{{#invoice_number_opt}}\n"
       "second has invoice\n"
       "{{/invoice_number_opt}}\n"
       "{{#innvoice_number}}\n"
       "third has invoice\n"
       "{{/innvoice_number}}\n"
       "just rendering invoice_number: {{invoice_number}} "
                      " {{/add_lines}}\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description.

   object_t invoice = 
   { {"invoice_number",4.425},
       //{"company"       , object_t{{"name"   , "FizSoft"},
       //                            {"street" , "42 Level St."},
       //                            {"city"   , "Ytic"},
       //                            {"state"  , "CA"},
       //                            {"zip"    , "98765"} }},
       {"lines"         , list_t{ 
                                  //object_t{{"item_code"    , "1234-2"},
                                  //         {"description"  , "Jolt Case"},
                                  //         {"amount"       , "$23"}},
                                  //object_t{{"item_code"    , "1235-1"},
                                  //         {"description"  , "Computer"},
                                  //         {"amount"       , "$9"}},
                                  //object_t{{"add_lines"    , list_t {1,2,3,4} }},
                                  object_t{{"add_lines"    , user_rendered_t {"some-value"} }}


   }}
   };
                      
   invoice["test"] = value_t{ 42.425 };
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::stringstream stream;
   boostache::generate(stream, templ, invoice);
   // ------------------------------------------------------------------

   std::cout << stream.str();

   return 0;
}
