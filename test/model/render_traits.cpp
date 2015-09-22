/**
 *  \file render_traits.cpp
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Tests for render traits to make sure we get the correct category out
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// #include <boost/variant/variant.hpp>
// #include <boost/spirit/include/support_extended_variant.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/model/render_traits.hpp>
#include <map>
#include <string>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace boostache = boost::boostache;


// ----------------------------------------------------------------------
// attribute testing
// ----------------------------------------------------------------------
template <typename T>
constexpr bool is_plain_attribute(T const &)
{
   return false;
}

constexpr bool is_plain_attribute(boostache::extension::plain_attribute const &)
{
   return true;
}

template <typename T>
constexpr bool is_variant_attribute(T const &)
{
   return false;
}

constexpr bool is_variant_attribute(boostache::extension::variant_attribute const &)
{
   return true;
}

template <typename T>
constexpr bool is_optional_attribute(T const &)
{
   return false;
}

constexpr bool is_optional_attribute(boostache::extension::optional_attribute const &)
{
   return true;
}

template <typename T>
constexpr bool is_associative_attribute(T const &)
{
   return false;
}

constexpr bool is_associative_attribute(boostache::extension::associative_attribute const &)
{
   return true;
}

template <typename T>
constexpr bool is_sequence_attribute(T const &)
{
   return false;
}

constexpr bool is_sequence_attribute(boostache::extension::sequence_attribute const &)
{
   return true;
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------



struct foo
{};

using map1_t = std::map<std::string,foo>;
using map2_t = std::map<std::string,std::string>;




BOOST_AUTO_TEST_CASE(map1_render_trait)
{
   BOOST_CHECK(
      ! is_plain_attribute(
         boostache::extension::render_category<map1_t>::type{} )
      );

   BOOST_CHECK(
      ! is_variant_attribute(
         boostache::extension::render_category<map1_t>::type{} )
      );

   BOOST_CHECK(
      ! is_optional_attribute(
         boostache::extension::render_category<map1_t>::type{} )
      );

   BOOST_CHECK(
      is_associative_attribute(
         boostache::extension::render_category<map1_t>::type{} )
      );

   BOOST_CHECK(
      ! is_sequence_attribute(
         boostache::extension::render_category<map1_t>::type{} )
      );
}

BOOST_AUTO_TEST_CASE(map2_render_trait)
{
   BOOST_CHECK(
      ! is_plain_attribute(
         boostache::extension::render_category<map2_t>::type{} )
      );

   BOOST_CHECK(
      ! is_variant_attribute(
         boostache::extension::render_category<map2_t>::type{} )
      );

   BOOST_CHECK(
      ! is_optional_attribute(
         boostache::extension::render_category<map2_t>::type{} )
      );

   BOOST_CHECK(
      is_associative_attribute(
         boostache::extension::render_category<map2_t>::type{} )
      );

   BOOST_CHECK(
      ! is_sequence_attribute(
         boostache::extension::render_category<map2_t>::type{} )
      );
}
