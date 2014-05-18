/**
 *  \file stache_skipper.hpp
 * 
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_SKIPPER_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_SKIPPER_HPP

#include <boost/spirit/include/qi.hpp>

namespace boost { namespace boostache { namespace frontend
{
   namespace qi = boost::spirit::qi;

   template <typename Iterator>
   struct stache_skipper : qi::grammar<Iterator>
   {
      stache_skipper();
      qi::rule<Iterator> start;
   };
}}}
#endif

