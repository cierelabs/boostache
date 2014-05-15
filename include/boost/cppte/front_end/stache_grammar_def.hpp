/**
 *  \file stache_grammar.hpp
 * 
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_GRAMMAR_DEF_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_GRAMMAR_DEF_HPP

#include <boost/cppte/front_end/stache_ast_adapted.hpp>
#include <boost/cppte/front_end/stache_skipper_def.hpp>
#include <boost/cppte/front_end/stache_grammar.hpp>
#include <boost/spirit/include/qi.hpp>

namespace boost { namespace cppte { namespace front_end
{
   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;

   template <typename Iterator>
   stache_grammar<Iterator>::stache_grammar()
      : stache_grammar::base_type(stache_root)
   {
      qi::char_type char_;
      qi::lexeme_type lexeme;
      qi::alpha_type alpha;
      qi::alnum_type alnum;
      qi::attr_type attr;
      qi::omit_type omit;
      qi::lit_type lit;


      stache_root =
           *stache_node
         ;

      stache_node =
           section
         | variable
         | literal_text
         ;

      identifier =
         lexeme[ alpha >> *(alnum | '_') ]
         ;

      variable =
            lit("{{")
         >> attr(true)
         >> identifier
         >> "}}"
         ;

      section =
            omit[section_begin]
         >> *stache_node
//         >> section_end
         ;

      section_begin =
            lit("{{")
         >> '#'
         >> identifier
         >> "}}"
         ;

      // section_end =
      //       lit("{{"}
      //    >> '^'
      //    >> string(id)
      //    >> "}}"
      //    ;

      literal_text =
         *(char_ - "{{")
         ;
   };
}}}

#endif

