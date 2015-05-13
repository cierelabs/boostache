/**
 *  \file grammar_def.hpp
 *
 *  Copyright 2015 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_DJANGO_GRAMMAR_DEF_HPP
#define BOOST_BOOSTACHE_FRONT_END_DJANGO_GRAMMAR_DEF_HPP

#include <boost/boostache/frontend/django/ast_adapted.hpp>
#include <boost/boostache/frontend/django/grammar.hpp>

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
#include <boost/spirit/include/qi_list.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_matches.hpp>
#include <boost/spirit/include/qi_no_skip.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_plus.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/support_argument.hpp>

namespace boost { namespace boostache { namespace frontend { namespace django
{
   namespace qi = boost::spirit::qi;
   namespace spirit = boost::spirit;

   template <typename Iterator>
   grammar<Iterator>::grammar()
      : grammar::base_type(node_list)
   {
      qi::alnum_type alnum;
      qi::alpha_type alpha;
      qi::attr_type attr;
      qi::char_type char_;
      qi::lexeme_type lexeme;
      qi::lit_type lit;
      qi::no_skip_type no_skip;
      qi::omit_type omit;

      node =
            no_skip[literal_text]
         |  comment
         |  variable
         ;

      node_list =
         *node
         ;

      literal_text =
         +(char_ - (lit("{{") | "{%" | "{#"))
         ;

      comment =
            lit("{#")
         >> omit[*(char_ - "#}")]
         >> "#}"
         ;

      identifier =
         alpha >> *(alnum | char_('_'))
         ;

      variable =
            lit("{{")
         >> lexeme[identifier % "."]
         >> "}}"
         ;
   };
}}}}

#endif
