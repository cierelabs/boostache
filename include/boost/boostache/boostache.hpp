/**
 *  \file boostache.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_BOOSTACHE_HPP
#define BOOST_BOOSTACHE_BOOSTACHE_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/frontend/parse.hpp>
#include <boost/boostache/backend/django_compiler.hpp>
#include <boost/boostache/backend/stache_compiler.hpp>
#include <boost/boostache/vm/generate.hpp>
#include <boost/boostache/vm/unificator.hpp>
#include <istream>


namespace boost { namespace boostache
{
   template <typename Format, typename Iterator>
   inline vm::ast::node load_template(Iterator & begin, Iterator const & end)
   {
      return backend::compile(frontend::parse<Format>(begin,end));
   }

   template <typename Format>
   inline vm::ast::node load_template(std::istream & input)
   {
      return backend::compile(frontend::parse<Format>(input));
   }



   template<typename Variant>
   struct stack_t {
       template<typename Arg>
       stack_t(Arg&& arg, stack_t const* parent)
           : current(std::forward<Arg>(arg))
           , parent(parent) {
       }

       Variant current;
       stack_t const* parent;
   };

   template <typename Stream, typename Context>
   void generate( Stream & stream
                , vm::ast::node const & templ
       , Context const & context
//       , stack_t const & stack
   )
   {
        using uniform_data_t = typename vm::uniform_data_t<Context>;

        stack_t<uniform_data_t>* stack = nullptr;
        vm::generate(stream,templ, context, stack);
   }
}}

#endif
