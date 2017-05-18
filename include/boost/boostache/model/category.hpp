/**
 *  \file category.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_CATEGORY_HPP
#define BOOST_BOOSTACHE_MODEL_CATEGORY_HPP


namespace boost { namespace boostache { namespace extension
{
   // --------------------------------
   // classifications
   struct category_attribute {};
   struct unused_attribute : category_attribute {};
   struct plain_attribute : category_attribute {};
   struct sequence_attribute : category_attribute {};
   struct associative_attribute : category_attribute {};
   struct tuple_attribute : category_attribute {};
   struct variant_attribute : category_attribute {};
   struct optional_attribute : category_attribute {};
   struct stacked_context_attribute : category_attribute {};

}}}

#endif
