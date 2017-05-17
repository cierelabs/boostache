/**
 *  \file stache_compiler.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_BACKEND_STACHE_COMPILER_HPP
#define BOOST_BOOSTACHE_BACKEND_STACHE_COMPILER_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/backend/compiler.hpp>
#include <boost/boostache/backend/detail/django_compiler.hpp>

namespace boost { namespace boostache { namespace backend
{
   inline vm::ast::node compile(frontend::django::ast::root const & ast)
   {
      return django_compiler::compile(ast);
   }
}}}

#endif
