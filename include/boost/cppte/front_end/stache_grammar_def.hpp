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
#include <boost/spirit/include/phoenix_operator.hpp>

namespace boost { namespace cppte { namespace front_end
{
   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;

   template <typename Iterator>
   stache_grammar<Iterator>::stache_grammar()
      : stache_grammar::base_type(stache_root)
   {
      qi::_1_type _1;
      qi::_a_type _a;
      qi::_r1_type _r1;
      qi::char_type char_;
      qi::lexeme_type lexeme;
      qi::alpha_type alpha;
      qi::alnum_type alnum;
      qi::string_type string;
      qi::attr_type attr;
      qi::omit_type omit;
      qi::lit_type lit;
      qi::eps_type eps;
      qi::matches_type matches;
      qi::no_skip_type no_skip;


      stache_root =
            eps
         >> *stache_node
         ;

      stache_node =
           section
         | variable
         | no_skip[literal_text]
         ;

      identifier =
         lexeme[alpha >> *(alnum | char_('_'))]
         ;

      variable =
         lexeme[lit("{{") >> matches['&']]
         >> identifier
         >> "}}"
         ;

      section %=
            matches[&lit("{{^")]
         >> section_begin[_a = _1]
         >> *stache_node
         >> section_end(_a)
         ;

      section_begin =
            lexeme[lit("{{") >> (lit('#') | '^')]
         >> identifier
         >> "}}"
         ;

      section_end =
            lit("{{")
         >> '/'
         >> omit[string(_r1)]
         >> "}}"
         ;

      literal_text =
         +(char_ - "{{")
         ;
   };
}}}

#endif
