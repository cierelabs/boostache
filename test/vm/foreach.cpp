/**
 *  \file test/mustache/mustache_compiler.cpp
 *
 *  Test the foreach magic
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <boost/boostache/vm/detail/foreach.hpp>
#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/spirit/include/support_extended_variant.hpp>
#include <boost/optional.hpp>
#include <vector>
#include <map>
#include <sstream>
#include <typeinfo>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace boostache = boost::boostache;
namespace vm = boostache::vm;

template <typename T>
constexpr const char * type_string(T const &)
{
   return "unknown";
};

constexpr const char * type_string(int const &)
{
   return "int";
};

template <typename T>
constexpr const char * type_string(std::vector<T> const &)
{
   return "vector<T>";
};

template <typename T>
constexpr const char * type_string(std::map<std::string,T> const &)
{
   return "map<std::string,T>";
};

template <typename T>
constexpr const char * type_string(boost::optional<T> const &)
{
   return "optional<T>";
};


namespace boost { namespace boostache { namespace vm
{
   template <typename Stream, typename Template, typename Context>
   void generate(Stream & stream, Template const & templ, Context const & context)
   {
      stream << "generate for context " << type_string(context) << std::endl;
   }
}}}


BOOST_AUTO_TEST_CASE(int_foreach)
{
   vm::ast::for_each for_each_node{"test", vm::ast::literal{"test node"}};
   int ctx = 42;

   std::ostringstream stream;
   vm::detail::foreach(stream, for_each_node, ctx);
   BOOST_CHECK_EQUAL( stream.str()
                    , "generate for context int\n"
      );
}

BOOST_AUTO_TEST_CASE(vector1_foreach)
{
   using vector_t = std::vector<int>;

   vm::ast::for_each for_each_node{"test", vm::ast::literal{"test node"}};
   vector_t ctx = {1,2,3,4};

   std::ostringstream stream;
   vm::detail::foreach(stream, for_each_node, ctx);
   BOOST_CHECK_EQUAL( stream.str()
                    , "generate for context int\n"
                      "generate for context int\n"
                      "generate for context int\n"
                      "generate for context int\n"
      );
}

BOOST_AUTO_TEST_CASE(map1_foreach)
{
   using map_t = std::map<std::string,int>;

   vm::ast::for_each for_each_node{"test", vm::ast::literal{"test node"}};
   map_t ctx = {{"test",42}};

   std::ostringstream stream;
   vm::detail::foreach(stream, for_each_node, ctx);
   BOOST_CHECK_EQUAL( stream.str()
                    , "generate for context map<std::string,T>\n"
      );
}

BOOST_AUTO_TEST_CASE(map2_foreach)
{
   using vector_t = std::vector<int>;
   using map_t = std::map<std::string,vector_t>;

   vm::ast::for_each for_each_node{"test", vm::ast::literal{"test node"}};
   map_t ctx = {{"test",{1,2,3,4}}};

   std::ostringstream stream;
   vm::detail::foreach(stream, for_each_node, ctx);
   BOOST_CHECK_EQUAL( stream.str()
                    , "generate for context map<std::string,T>\n"
      );
}

BOOST_AUTO_TEST_CASE(map3_foreach)
{
   using map_t = std::map<std::string,std::string>;

   vm::ast::for_each for_each_node{"test", vm::ast::literal{"test node"}};
   map_t ctx = {{"test","bar"}};

   std::ostringstream stream;
   vm::detail::foreach(stream, for_each_node, ctx);
   BOOST_CHECK_EQUAL( stream.str()
                    , "generate for context map<std::string,T>\n"
      );
}



// ---------------------------------------------------------------------
struct v1_t : boost::spirit::extended_variant<
     int
   , std::string
   , std::vector<int>
   >
{
   v1_t() : base_type() {}
   v1_t(int rhs) : base_type(rhs) {}
   v1_t(std::string const & rhs) : base_type(rhs) {}
   v1_t(const char * rhs) : base_type(std::string{rhs}) {}
   v1_t(std::vector<int> const & rhs) : base_type(rhs) {}
};


BOOST_AUTO_TEST_CASE(variant1_foreach)
{
   vm::ast::for_each for_each_node{"test", vm::ast::literal{"test node"}};
   v1_t ctx;

   {
      ctx = "gorp";
      std::ostringstream stream;
      vm::detail::foreach(stream, for_each_node, ctx);
      BOOST_CHECK_EQUAL( stream.str()
                       , "generate for context unknown\n"
         );
   }

   {
      ctx = std::vector<int>{1,2,3,4};
      std::ostringstream stream;
      vm::detail::foreach(stream, for_each_node, ctx);
      BOOST_CHECK_EQUAL( stream.str()
                       , "generate for context int\n"
                         "generate for context int\n"
                         "generate for context int\n"
                         "generate for context int\n"
         );
   }
}
// ---------------------------------------------------------------------


BOOST_AUTO_TEST_CASE(optional_foreach)
{
   vm::ast::for_each for_each_node{"test", vm::ast::literal{"test node"}};

   {
      boost::optional<int> ctx;
      std::ostringstream stream;
      vm::detail::foreach(stream, for_each_node, ctx);
      BOOST_CHECK_EQUAL( stream.str()
                       , "generate for context optional<T>\n"
         );
   }
   {
      boost::optional<int> ctx = 42;
      std::ostringstream stream;
      vm::detail::foreach(stream, for_each_node, ctx);
      BOOST_CHECK_EQUAL( stream.str()
                       , "generate for context int\n"
         );
   }

   {
      boost::optional<std::vector<int>> ctx;
      std::ostringstream stream;
      vm::detail::foreach(stream, for_each_node, ctx);
      BOOST_CHECK_EQUAL( stream.str()
                       , "generate for context optional<T>\n"
         );
   }

   {
      boost::optional<std::vector<int>> ctx = std::vector<int>{1,2,3,4};
      std::ostringstream stream;
      vm::detail::foreach(stream, for_each_node, ctx);
      BOOST_CHECK_EQUAL( stream.str()
                       , "generate for context int\n"
                         "generate for context int\n"
                         "generate for context int\n"
                         "generate for context int\n"
         );
   }
}
