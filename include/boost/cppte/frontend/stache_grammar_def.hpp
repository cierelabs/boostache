/**
 *  \file stache_grammar.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_GRAMMAR_DEF_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_GRAMMAR_DEF_HPP

#include <boost/cppte/frontend/stache_ast_adapted.hpp>
#include <boost/cppte/frontend/stache_grammar.hpp>

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_alternative.hpp>
#include <boost/spirit/include/qi_and_predicate.hpp>
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_difference.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_kleene.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_matches.hpp>
#include <boost/spirit/include/qi_no_skip.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_plus.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/support_argument.hpp>

namespace boost { namespace cppte { namespace front_end
{
   namespace qi = boost::spirit::qi;
   namespace spirit = boost::spirit;

   template <typename Iterator>
   stache_grammar<Iterator>::stache_grammar()
      : stache_grammar::base_type(stache_root)
   {
      spirit::_1_type _1;
      spirit::_a_type _a;
      spirit::_r1_type _r1;
      qi::alnum_type alnum;
      qi::alpha_type alpha;
      qi::attr_type attr;
      qi::char_type char_;
      qi::lexeme_type lexeme;
      qi::lit_type lit;
      qi::matches_type matches;
      qi::no_skip_type no_skip;
      qi::omit_type omit;
      qi::string_type string;


      stache_root =
         *stache_node
         ;

      stache_node =
            comment
         |  variable
         |  variable_unescaped
         |  section
         |  partial
         |  no_skip[literal_text]
         ;

      identifier =
         lexeme[alpha >> *(alnum | char_('_'))]
         ;

      comment =
            lit("{{")
         >> '!'
         >> omit[*(char_ - "}}")]
         >> "}}"
         ;

      variable =
            lit("{{")
         >> matches['&']
         >> identifier
         >> "}}"
         ;

      variable_unescaped =
            lit("{{{")
         >> attr(true)
         >> identifier
         >> "}}}"
         ;

      section %=
            matches[&(lit("{{") >> '^')]
         >> section_begin[_a = _1]
         >> *stache_node
         >> section_end(_a)
         ;

      section_begin =
            lit("{{")
         >> (lit('#') | '^')
         >> identifier
         >> "}}"
         ;

      section_end =
            lit("{{")
         >> '/'
         >> omit[string(_r1)]
         >> "}}"
         ;

      partial =
            lit("{{")
         >> '>'
         >> identifier
         >> "}}"
         ;

      literal_text =
         +(char_ - "{{")
         ;
   };
}}}

#endif
