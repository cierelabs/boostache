/**
 *  \file traits.cpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Tests for boostache traits
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/variant/variant.hpp>
#include <boost/spirit/include/support_extended_variant.hpp>
#include <boost/boostache/vm/traits.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


struct foo
{};

typedef boost::variant< foo, int > my_variant;

struct ext_variant : boost::spirit::extended_variant<
     foo
   , int >
{};

BOOST_AUTO_TEST_CASE(not_variant_trait)
{
   BOOST_CHECK(!boost::boostache::vm::trait::is_variant<foo>::value);
}

BOOST_AUTO_TEST_CASE(variant_trait)
{
   BOOST_CHECK(boost::boostache::vm::trait::is_variant<my_variant>::value);
}

BOOST_AUTO_TEST_CASE(extended_variant_trait)
{
   BOOST_CHECK(boost::boostache::vm::trait::is_variant<ext_variant>::value);
}
