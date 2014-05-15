/**
 *  \file stache_skipper_def.hpp
 * 
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_SKIPPER_DEF_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_SKIPPER_DEF_HPP

#include <boost/cppte/frontend/stache_skipper.hpp>

namespace boost { namespace cppte { namespace front_end
{
   namespace qi = boost::spirit::qi;

   template <typename Iterator>
   stache_skipper<Iterator>::stache_skipper()
      : stache_skipper::base_type(start)
   {
      qi::blank_type blank;
      qi::char_type char_;
      qi::lit_type lit;

      start =
           blank
         | ( lit("{{!") >> *(char_ - "}}") >> "}}" )
         ;
   }
}}}
#endif

