/**
 *  \file exampe1.cpp
 *
 *  A simple example of how to use boostache.
 *
 *  Copyright 2015 Michael Caisse : ciere.com
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
#include <boost/boostache/model/extensions/property_tree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>

namespace boostache = boost::boostache;

void simple_one()
{
   // ------------------------------------------------------------------
   // Describe the input template. We are going to use mustache format.
   std::string input("My name is {{name}}. I am {{age}} years old.\n");
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // The data description. Just a simple property tree.
   boost::property_tree::ptree data;
   data.put("name", "Jeroen");
   data.put("age", 42);
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

   // first line
   boost::property_tree::ptree mushroom1;
   mushroom1.put("item_code", "Muchomurka Zelena");
   mushroom1.put("description", "Nice green and very toxic mushroom");
   mushroom1.put("amount", "$1");

   // second line
   boost::property_tree::ptree mushroom2;
   mushroom2.put("item_code", "Hrib hnedy");
   mushroom2.put("description", "Tasty mushroom");
   mushroom2.put("amount", "$3");

   // make list of lines
   boost::property_tree::ptree mushrooms;
   mushrooms.push_back(std::make_pair("", mushroom1));
   mushrooms.push_back(std::make_pair("", mushroom2));

   // the data - lines is list of sections
   boost::property_tree::ptree data;
   data.add_child("lines", mushrooms);
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
   std::string json = "{"
                      "\"invoice_number\": 1234,"
                      "\"company\": {"
                          "\"name\": \"FizSoft\","
                          "\"street\": \"42 Level St.\","
                          "\"city\": \"Ytic\","
                          "\"state\": \"CA\","
                          "\"zip\": \"98765\""
                      "},"
                      "\"lines\": ["
                          "{"
                          "\"item_code\": \"1234-2\","
                          "\"description\": \"Jolt Case\","
                          "\"amount\": \"$23\""
                          "},"
                          "{"
                          "\"item_code\": \"1235-1\","
                          "\"description\": \"Computer\","
                          "\"amount\": \"$9\""
                          "}"
                      "]"
                      "}";

   std::istringstream json_stream(json);
   boost::property_tree::ptree data;
   boost::property_tree::read_json(json_stream, data);

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

   // first line
   boost::property_tree::ptree line1;
   boost::property_tree::ptree items1;
   boost::property_tree::ptree item11, item12;
   item11.put("name", "name1");
   item11.put("value", "value1");
   item12.put("name", "name2");
   item12.put("value", "value2");
   items1.push_back(std::make_pair("", item11));
   items1.push_back(std::make_pair("", item12));
   line1.put("title", "first line");
   line1.add_child("items", items1);

   // second line
   boost::property_tree::ptree line2;
   boost::property_tree::ptree items2;
   boost::property_tree::ptree item21, item22, item23;
   item21.put("name", "name1");
   item21.put("value", "value1");
   item22.put("name", "name2");
   item22.put("value", "value2");
   item23.put("name", "name3");
   item23.put("value", "value3");
   items2.push_back(std::make_pair("", item21));
   items2.push_back(std::make_pair("", item22));
   items2.push_back(std::make_pair("", item23));
   line2.put("title", "second line");
   line2.add_child("items", items2);

   // make list of lines
   boost::property_tree::ptree lines;
   lines.push_back(std::make_pair("", line1));
   lines.push_back(std::make_pair("", line2));

   // the data - lines is list of sections
   boost::property_tree::ptree data;
   data.add_child("lines", lines);

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
   boost::property_tree::ptree data;
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

