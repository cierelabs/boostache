/**
 *  \file exampe4.cpp
 *
 *  A simple example of how to use boostache.
 *
 *  Copyright 2015 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/vm/printer.hpp>
#include <boost/boostache/stache.hpp>
#include <boost/boostache/model/helper.hpp>
#include <boost/boostache/model/extensions/google_protobuf.hpp>
#include <iostream>
#include <sstream>

#include "example5.pb.h"

namespace boostache = boost::boostache;

void simple_one()
{
   // ------------------------------------------------------------------
   // Describe the input template. We are going to use mustache format.
   std::string input("My name is {{name}}. I am {{age}} years old.\n");
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // The data description. Just a simple protobuf.
   Simple data;
   data.set_name("Jeroen");
   data.set_age(42);

   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template.
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Print the template tree.
   std::cout << "----------------------------------- template" << std::endl;
   print(std::cout, templ);
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to generate the result
   std::cout << "----------------------------------- log" << std::endl;
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   // print the result
   std::cout << "----------------------------------- output" << std::endl;
   std::cout << stream.str();
}

void multi_one()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input(
                      "Invoice"
                      "\n"
                      "{{#lines}}"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description. invoice_items is a list of maps that
   // describe each item.
   Invoice data;

   // first line
   auto mushroom1 = data.add_lines();
   mushroom1->set_item_code("Muchomurka Zelena");
   mushroom1->set_description("Nice green and very toxic mushroom");
   mushroom1->set_amount("$1");

   // second line
   auto mushroom2 = data.add_lines();
   mushroom2->set_item_code("Hrib hnedy");
   mushroom2->set_description("Tasty mushroom");
   mushroom2->set_amount("$3");

   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end());
   std::cout << "----------------------------------- template" << std::endl;
   print(std::cout, templ);
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::cout << "----------------------------------- log" << std::endl;
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   std::cout << "----------------------------------- output" << std::endl;
   std::cout << stream.str();
}

void compl_one()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input(
                      "Invoice {{invoice_number}}"
                      "\n"
                      "{{# company}}"
                      "Company: {{name}}\n"
                      "         {{street}}\n"
                      "         {{city}}, {{state}}  {{zip}}\n"
                      "{{/ company}}"
                      "------------------------------------------------\n"
                      "{{#lines}}"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description.
   Invoice data;
   data.set_invoice_number(1234);

   // company
   auto company = data.mutable_company();
   company->set_name("FizSoft");
   company->set_street("42 Level St.");
   company->set_city("Ytic");
   company->set_state("CA");
   company->set_zip("98765");

   // lines
   auto line1 = data.add_lines();
   line1->set_item_code("1234-2");
   line1->set_description("Jolt Case");
   line1->set_amount("$23");
   auto line2 = data.add_lines();
   line2->set_item_code("1235-1");
   line2->set_description("Computer");
   line2->set_amount("$9");
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end());
   std::cout << "----------------------------------- template" << std::endl;
   print(std::cout, templ);
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::cout << "----------------------------------- log" << std::endl;
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   std::cout << "----------------------------------- output" << std::endl;
   std::cout << stream.str();
}

void nest_one()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input(
                      "-= prologue =- "
                      "{{#lines}}"
                      "\n{{title}} - "
                      "{{#items}}"
                      "{{name}}"
                      "="
                      "{{value}}"
                      ", "
                      "{{/items}}"
                      ""
                      "{{/lines}}"
                      "\n-= epilogue =-\n"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description.
   Nested data;

   // first line
   auto line1 = data.add_lines();
   auto item11 = line1->add_items();
   item11->set_name("name1");
   item11->set_value("value1");
   auto item12 = line1->add_items();
   item12->set_name("name2");
   item12->set_value("value2");
   line1->set_title("first line");

   // second line
   auto line2 = data.add_lines();
   auto item21 = line2->add_items();
   item21->set_name("name1");
   item21->set_value("value1");
   auto item22 = line2->add_items();
   item22->set_name("name2");
   item22->set_value("value2");
   auto item23 = line2->add_items();
   item23->set_name("name3");
   item23->set_value("value3");
   line2->set_title("second line");
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end());
   std::cout << "----------------------------------- template" << std::endl;
   print(std::cout, templ);
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::cout << "----------------------------------- log" << std::endl;
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   std::cout << "----------------------------------- output" << std::endl;
   std::cout << stream.str();
}

void else_one()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input(
                      "Invoice"
                      "\n"
                      "{{#lines}}"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "{{/lines}}"
                      "{{^lines}}"
                      "there are no items\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description. invoice_items is a list of maps that
   // describe each item.
   Simple data;

   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end());
   std::cout << "----------------------------------- template" << std::endl;
   print(std::cout, templ);
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::cout << "----------------------------------- log" << std::endl;
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   std::cout << "----------------------------------- output" << std::endl;
   std::cout << stream.str();
}

int main()
{
   std::cout << "=================================== simple_one" << std::endl;
   simple_one();
   std::cout << "=================================== multi_one" << std::endl;
   multi_one();
   std::cout << "=================================== compl_one" << std::endl;
   compl_one();
   std::cout << "=================================== nest_one" << std::endl;
   nest_one();
   std::cout << "=================================== else" << std::endl;
   else_one();
}

