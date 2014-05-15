/**
 *  \file stache_grammar.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_GRAMMAR_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_GRAMMAR_HPP

#include <boost/cppte/frontend/stache_ast.hpp>
#include <boost/cppte/frontend/stache_skipper.hpp>
#include <boost/spirit/include/qi.hpp>

namespace boost { namespace cppte { namespace front_end
{
   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;

   template <typename Iterator>
   struct stache_grammar
      : qi::grammar<Iterator, ast::stache_root(), stache_skipper<Iterator>>
   {
      stache_grammar();

      qi::rule<Iterator, ast::stache_root(), stache_skipper<Iterator>>
         stache_root
         ;

      qi::rule<Iterator, ast::stache_node(), stache_skipper<Iterator>>
         stache_node
         ;

      qi::rule<Iterator, ast::identifier(), stache_skipper<Iterator>>
         identifier
         ;

      qi::rule<Iterator, ast::literal_text()>
         literal_text
         ;

      qi::rule<Iterator, ast::variable(), stache_skipper<Iterator>>
         variable
         ;

      qi::rule<Iterator, ast::section(), qi::locals<std::string>, stache_skipper<Iterator>>
         section
         ;

      qi::rule<Iterator, ast::identifier(), stache_skipper<Iterator>>
         section_begin
         ;

      qi::rule<Iterator, qi::unused_type(std::string), stache_skipper<Iterator>>
         section_end
         ;
   };
}}}

#endif

