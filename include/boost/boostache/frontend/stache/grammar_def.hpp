/**
 *  \file stache/grammar_def.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_GRAMMAR_DEF_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_GRAMMAR_DEF_HPP

#include <boost/boostache/frontend/stache/ast_adapted.hpp>
#include <boost/boostache/frontend/stache/grammar.hpp>

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
#include <boost/spirit/include/qi_skip.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_plus.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/support_argument.hpp>

namespace boost { namespace boostache { namespace frontend { namespace stache
{
   namespace qi = boost::spirit::qi;
   namespace spirit = boost::spirit;

   template <typename Iterator>
   grammar<Iterator>::grammar()
      : grammar::base_type(node_list)
   {
      spirit::_1_type _1;
      spirit::_a_type _a;
      spirit::_r1_type _r1;
      spirit::eol_type eol;
      qi::alnum_type alnum;
      qi::alpha_type alpha;
      qi::blank_type blank;
      qi::attr_type attr;
      qi::char_type char_;
      qi::lexeme_type lexeme;
      qi::lit_type lit;
      qi::matches_type matches;
      qi::no_skip_type no_skip;
      qi::skip_type skip;
      qi::omit_type omit;


      stache_node =
            end_of_line
         |  blank_text
         |  no_skip[literal_text]
         |  comment
         |  variable
         |  variable_unescaped
         |  section
         |  partial
         ;


      node_list =
         *stache_node
         ;

      literal_text =
         +(char_ - (eol|"{{"))
         ;

      blank_text =
         no_skip[+blank]
         ;

      end_of_line =
            char_('\n')
         >> (   (char_("\r") >> attr(ast::eol{"\n\r"}))
              | attr(ast::eol{"\n"}) );
         ;

      comment =
            lit("{{")
         >> '!'
         >> omit[*(char_ - "}}")]
         >> "}}"
         ;

      identifier =
         lexeme[alpha >> *(alnum | char_('_'))]
         ;

      variable =
            lit("{{")
         >> skip(qi::space_type{})
            [
                  matches['&']
               >> identifier
               >> "}}"
            ]
         ;

      variable_unescaped =
            lit("{{{")
         >> skip(qi::space_type{})
            [
                  attr(true)
               >> identifier
               >> "}}}"
            ]
         ;

      section %=
            matches[&(lit("{{") >> '^')]
         >> section_begin[_a = _1]
         >> *stache_node
         >> section_end(_a)
         ;

      section_begin =
            lit("{{")
         >> skip(qi::space_type{})
            [
                  (lit('#') | '^')
               >> identifier
               >> "}}"
//         >> omit[ no_skip[ (*char_(" ") >> eol) ] ]
            ]
         ;

      section_end =
            lit("{{")
         >> skip(qi::space_type{})
            [
                  '/'
               >> lit(_r1)
               >> "}}"
            ]
//         >> omit[ no_skip[ (*char_(" ") >> eol) ] ]
         ;

      partial =
            lit("{{")
         >> skip(qi::space_type{})
            [
                  '>'
               >> identifier
               >> "}}"
            ]
         ;
   };
}}}}

#endif
