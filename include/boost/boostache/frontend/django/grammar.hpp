/**
 *  \file grammar.hpp
 *
 *  Copyright 2015 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_DJANGO_GRAMMAR_HPP
#define BOOST_BOOSTACHE_FRONT_END_DJANGO_GRAMMAR_HPP

#include <boost/boostache/frontend/django/ast.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_rule.hpp>

namespace boost { namespace boostache { namespace frontend { namespace django
{
   namespace qi = boost::spirit::qi;

   template <typename Iterator>
   struct grammar
      : qi::grammar<Iterator, ast::node_list(), qi::space_type>
   {
      grammar();

      qi::rule<Iterator, ast::node_list(), qi::space_type>
         node_list
         ;

      qi::rule<Iterator, ast::node(), qi::space_type>
         node
         ;

      qi::rule<Iterator, ast::identifier()>
         identifier
         ;

      qi::rule<Iterator, ast::comment(), qi::space_type>
         comment
         ;

      qi::rule<Iterator, ast::literal_text()>
         literal_text
         ;

      qi::rule<Iterator, ast::variable(), qi::space_type>
         variable
         ;

      qi::rule<Iterator, ast::condition(), qi::space_type>
         condition
         ;

      qi::rule<Iterator, ast::if_elif_else(), qi::space_type>
         if_elif_else
         ;
   };
}}}}

#endif

