/**
 *  \file parse.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Generic parser entry point. Call parse with the input format
 *  as the template parameter:
 *
 *     auto ast = parse<format::stache>(...);
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_TAGS_HPP
#define BOOST_BOOSTACHE_FRONT_END_TAGS_HPP

#include <boost/spirit/include/qi_parse.hpp>

namespace boost { namespace boostache { namespace frontend
{
   template <typename Format, typename Iterator>
   typename Format::ast_t parse(Iterator & begin, Iterator const & end)
   {
      typename Format::ast_t ast;
      typename Format::template grammar_t<Iterator> grammar;
      
      // what shall we do with a parser error?
      // what shall we do if the input isn't fully parsed?
      if(!boost::spirit::qi::phrase_parse( begin, end
                                         , grammar
                                         , typename Format::skipper_t{}
                                         , ast ))
      {
         ast = typename Format::ast_t{};
      }
      return ast;
   }
}}}

#endif

