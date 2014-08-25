/**
 *  \file tags.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_TAGS_HPP
#define BOOST_BOOSTACHE_VM_TAGS_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace boostache { namespace vm { namespace tag
{
   struct false_ : boost::mpl::false_  {};
   struct true_  : boost::mpl::true_  {};

   // tag used with foreach construct to indicate the type statically supports 
   // begin/end
   struct static_true  {} ;

   // tag used with foreach construct to indicate the type dynamically supports 
   // begin/end
   struct dynamic_true {} ;
}}}}

#endif
