/**
 *  \file compiler.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_BACKEND_COMPILER_HPP
#define BOOST_BOOSTACHE_BACKEND_COMPILER_HPP

#include <boost/boostache/vm/engine_ast.hpp>

namespace boost { namespace boostache { namespace backend
{
   template<class T>
   vm::ast::node_list compile(T const & ast)
   {
      std::cout << "generic compile for : " << typeid(ast).name() << std::endl;
      // compiler not implemented... do something smart
      return vm::ast::node_list{};
   }
}}}

#endif
