/**
 *  \file django.hpp
 *
 *  Copyright 2015 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_DJANGO_HPP
#define BOOST_BOOSTACHE_FRONT_END_DJANGO_HPP

#include <boost/boostache/frontend/django/grammar.hpp>
#include <boost/boostache/frontend/django/ast.hpp>
#include <boost/spirit/include/qi_char_class.hpp>

namespace boost { namespace boostache { namespace format
{
   struct django
   {
      template <typename Iterator>
      using grammar_t = frontend::django::grammar<Iterator>;

      using ast_t = frontend::django::ast::root;
      using skipper_t = boost::spirit::qi::space_type;
   };
}}}


#endif
