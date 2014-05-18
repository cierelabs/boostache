/**
 *  \file parse.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_TAGS_HPP
#define BOOST_BOOSTACHE_FRONT_END_TAGS_HPP

namespace boost { namespace boostache
{
   template <typename TemplateFormat>
   TemplateFormat::ast_t parse()
   {
      TemplateFormat::ast_t ast;
      grammar<TemplateFormat::grammar_t> grammar;
      
      // what shall we do with a parser error?
      // what shall we do if the input isn't fully parsed?
      parse(grammar,ast);
      return ast;
   }
}}

#endif

