/**
 *  \file unwrap_variant.cpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Tests for generic variant unwrapping
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/variant/variant.hpp>
#include <boost/spirit/include/support_extended_variant.hpp>
#include <boost/boostache/model/helper.hpp>
#include <iostream>
#include <string>
#include <map>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace bstache = boost::boostache;
namespace extn =  bstache::extension;

struct foo
{};

struct bar
{
   int value;
};



namespace boost { namespace boostache { namespace extension
{
   template <>
   struct test_category<foo> : mpl::identity<unused_attribute> {};
}}}


namespace boost { namespace boostache { namespace extension
{
   bool test(std::string const & name, bar b)
   {
      return !!b.value;
   }

   template <typename Stream>
   void render(Stream & stream, foo f, std::string const & name, category_attribute)
   {
      stream << "<got a foo> ";
   }

   template <typename Stream>
   void render(Stream & stream, bar b, std::string const & name, plain_attribute)
   {
      stream << "< got a bar(" << b.value << ") >";
   }
}}}




typedef boost::variant< foo, bar > my_variant;
typedef boost::variant< foo, int > your_variant;
typedef boost::variant< foo, bar, bool, std::string > another_variant;


using extn::test;
using extn::render;

BOOST_AUTO_TEST_CASE(unwrap_variant_test)
{
   {
      my_variant v;
      BOOST_CHECK(!test("foo",v));

      v = bar{12};
      BOOST_CHECK(test("foo",v));

      v = bar{0};
      BOOST_CHECK(!test("foo",v));
   }

   {
      your_variant v;
      v = 12;
      BOOST_CHECK(test("foo",v));

      v = 0;
      BOOST_CHECK(!test("foo",v));

      v = foo{};
      BOOST_CHECK(!test("foo",v));
   }

   {
      another_variant v;
      v = true;
      BOOST_CHECK(test("foo",v));

      v = false;
      BOOST_CHECK(!test("foo",v));

      v = bar{42};
      BOOST_CHECK(test("foo",v));

      v = bar{0};
      BOOST_CHECK(!test("foo",v));

      v = foo{};
      BOOST_CHECK(!test("foo",v));

      v = std::string{"test"};
      BOOST_CHECK(test("foo",v));

      v = std::string{};
      BOOST_CHECK(!test("foo",v));

      std::map<std::string,another_variant> m;
      m["foo"] = v;
      BOOST_CHECK(!test("foo",m));

      BOOST_CHECK(!test("bar",m));

      v = std::string{"test"};
      m["bar"] = v;
      BOOST_CHECK(test("bar",m));
   }

}


// BOOST_AUTO_TEST_CASE(unwrap_variant_render)
// {
//    {
//       my_variant v;
//       render(std::cout,v,"foo");

//       v = bar{12};
//       render(std::cout,v,"foo");

//       v = bar{0};
//       render(std::cout,v,"foo");
//       BOOST_CHECK(false);
//    }

//    {
//       your_variant v;
//       v = 12;
//       render(std::cout,v,"foo");

//       v = 0;
//       render(std::cout,v,"foo");

//       v = foo{};
//       render(std::cout,v,"foo");
//    }

//    {
//       another_variant v;
//       v = true;
//       render(std::cout,v,"foo");

//       v = false;
//       render(std::cout,v,"foo");

//       v = bar{42};
//       render(std::cout,v,"foo");

//       v = bar{0};
//       render(std::cout,v,"foo");

//       v = foo{};
//       render(std::cout,v,"foo");
//    }

// }


