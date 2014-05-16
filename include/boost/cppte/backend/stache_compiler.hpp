/**
 *  \file stache_compiler.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_BACKEND_STACHE_COMPILER_HPP
#define BOOST_CPPTE_BACKEND_STACHE_COMPILER_HPP

#include <boost/cppte/vm/engine_ast.hpp>
#include <boost/cppte/backend/compiler.hpp>
#include <boost/cppte/backend/detail/stache_compiler.hpp>

namespace boost { namespace cppte { namespace backend
{
   inline vm::ast::node_list compile(front_end::ast::stache_root const & ast)
   {
      return stache_compiler::compile(ast);
   }
}}}

#endif

