/**
 *  \file stache_grammar.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_GRAMMAR_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_GRAMMAR_HPP

#include <boost/boostache/frontend/stache/ast.hpp>

#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <string>
#include <boost/fusion/support/unused.hpp>


namespace boost { namespace boostache { namespace frontend { namespace stache
{
   namespace qi = boost::spirit::qi;

   template <typename Format, typename Iterator, typename PartialFunctor>
   struct grammar
      : qi::grammar<Iterator, ast::node_list(), qi::space_type>
   {
      explicit grammar(PartialFunctor&& partial_mapper);
      explicit grammar(const PartialFunctor& partial_mapper);

      qi::rule<Iterator, ast::node_list(), qi::space_type>
         node_list
         ;

      qi::rule<Iterator, ast::node(), qi::space_type>
         stache_node
         ;

      qi::rule<Iterator, ast::identifier(), qi::space_type>
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

      qi::rule<Iterator, ast::variable(), qi::space_type>
         variable_unescaped
         ;

      qi::rule<Iterator, ast::section(), qi::locals<std::string>, qi::space_type>
         section
         ;

      qi::rule<Iterator, ast::identifier(), qi::space_type>
         section_begin
         ;

      qi::rule<Iterator, qi::unused_type(std::string), qi::space_type>
         section_end
         ;

      qi::rule<Iterator, ast::partial(), qi::space_type>
         partial
         ;

   private:
      ast::partial on_partial (const ast::identifier& name, const boost::fusion::unused_type& context, bool& pass) const;

      PartialFunctor partial_mapper;
   };
}}}}

#endif

