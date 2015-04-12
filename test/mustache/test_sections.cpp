/**
 *  \file test_sections.cpp
 *
 *  Copyright 2014 Dan Nuffer
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "test_template_fixture.hpp"

// Tests that a simple mustache tag is replaced
BOOST_FIXTURE_TEST_CASE(TestSectionsMustache, TemplateFixture)
{
   template_string = "Hi I am {{name}}.\n"
                     "{{# showme}}"
                     "I like {{pet}}."
                     "{{/ showme}}"
                     "{{# dontshowme}}"
                     "If you see this, something went wrong."
                     "{{/ dontshowme}}";

   set_tag_value("name", "Daniel");
   set_tag_value("pet", "turtles");
   set_tag_value("showme", true);
   set_tag_value("dontshowme", false);

   generate_template();

   std::string expected = "Hi I am Daniel.\n";
   expected += "I like turtles.";
   BOOST_CHECK_EQUAL(expected, result);
}

