/**
 *  \file generate.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_GENERATE_HPP
#define BOOST_BOOSTACHE_VM_GENERATE_HPP

#include <boost/boostache/vm/detail/engine_visitor.hpp>

namespace boost { namespace boostache { namespace vm
{
   template <typename Stream, typename Template, typename Context>
   void generate( Stream & stream
                , Template const & templ
                , Context const & context)
   {
      vm::detail::engine_visitor_base<Stream,Context> engine(stream,context);
      engine(templ);
   }
}}}

#endif
