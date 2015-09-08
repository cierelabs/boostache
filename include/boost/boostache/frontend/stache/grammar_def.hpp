/**
 *  \file stache/grammar_def.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *  Copyright 2015 Michele Santullo
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_GRAMMAR_DEF_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_GRAMMAR_DEF_HPP

#include <boost/boostache/frontend/stache/ast_adapted.hpp>
#include <boost/boostache/frontend/stache/grammar.hpp>
#include <boost/boostache/frontend/parse.hpp>

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
#include <boost/spirit/include/support_argument.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <utility>

namespace boost { namespace boostache { namespace frontend { namespace stache
{
   namespace qi = boost::spirit::qi;
   namespace spirit = boost::spirit;
   namespace phx = boost::phoenix;

   template <typename Format, typename Iterator, typename PartialFunctor>
   grammar<Format, Iterator, PartialFunctor>::grammar(const PartialFunctor& partial_mapper)
      : grammar<Format, Iterator, PartialFunctor>(PartialFunctor(partial_mapper))
   {
   }

   template <typename Format, typename Iterator, typename PartialFunctor>
   grammar<Format, Iterator, PartialFunctor>::grammar (PartialFunctor&& partial_mapper)
      : grammar::base_type(node_list),
      partial_mapper(std::move(partial_mapper))
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
      qi::_val_type _val;


      stache_node =
            no_skip[literal_text]
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
         +(char_ - "{{")
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
         >> lit(_r1)
         >> "}}"
         ;

      partial =
            lit("{{")
         >> '>'
         >> identifier[_val = phx::bind(&grammar<Format, Iterator, PartialFunctor>::on_partial, this, _1, phx::placeholders::_2, phx::placeholders::_3)]
         >> "}}"
         ;
   };

   template <typename Format, typename Iterator, typename PartialFunctor>
   ast::partial grammar<Format, Iterator, PartialFunctor>::on_partial (const ast::identifier& name, const boost::fusion::unused_type& context, bool& pass) const {
      grammar<Format, Iterator, PartialFunctor> gramm(partial_mapper);
      const auto text = partial_mapper(name);
      auto beg = text.begin();

      ast::partial retval;
      retval.name = name;
      retval.nodes = boost::boostache::frontend::parse<Format>(beg, text.end(), partial_mapper);
      pass = (text.end() == beg);

      return retval;
   }
}}}}

#endif
