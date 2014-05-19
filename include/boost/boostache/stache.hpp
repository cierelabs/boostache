/**
 *  \file stache.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_HPP

#include <boost/boostache/frontend/stache/grammar.hpp>
#include <boost/boostache/frontend/stache/ast.hpp>
#include <boost/spirit/include/qi_char_class.hpp>

namespace boost { namespace boostache { namespace format
{
   struct stache
   {
      template <typename Iterator>
      using grammar_t = frontend::stache::grammar<Iterator>;

      using ast_t = frontend::stache::ast::root;
      using skipper_t = boost::spirit::qi::space_type;
   };
}}}


#endif
