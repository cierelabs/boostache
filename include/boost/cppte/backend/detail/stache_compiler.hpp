/**
 *  \file detail/stache_compiler.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_BACKEND_DETAIL_STACHE_COMPILER_HPP
#define BOOST_CPPTE_BACKEND_DETAIL_STACHE_COMPILER_HPP

#include <boost/cppte/vm/engine_ast.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>
#include <boost/cppte/backend/compiler.hpp>
#include <boost/cppte/backend/detail/stache_compiler.hpp>

namespace boost { namespace cppte { namespace backend { namespace stache_compiler
{
   namespace detail
   {
      class stache_visit
      {
      public:
         typedef vm::ast::node result_type;

         stache_visit(std::ostream& out)
            : out(out)
         {}

         vm::ast::node operator()(front_end::ast::undefined) const
         {
            out << "WHOA! we have an undefined" << std::endl;
            return vm::ast::node{};
         }

         vm::ast::node operator()(front_end::ast::literal_text const & v) const
         {
            return vm::ast::literal{v};
         }

         vm::ast::node operator()(front_end::ast::variable const & v) const
         {
            return vm::ast::render{v.value};
         }

         vm::ast::node operator()(front_end::ast::section const & v) const
         {
            vm::ast::node_list vm_ast;
            for(auto const & node : v.nodes)
            {
               vm_ast.nodes.push_back(boost::apply_visitor(*this, node));
            }

            vm::ast::if_then_else if_block;
            if_block.condition_.name = v.name;

            if(v.is_inverted)  { if_block.else_ = vm_ast; }
            else               { if_block.then_ = vm_ast; }

            return if_block;
         }

         vm::ast::node operator()(front_end::ast::comment const & v) const
         {
            return vm::ast::literal{};
         }         

         vm::ast::node operator()(front_end::ast::partial const & v) const
         {
            return vm::ast::literal{};
         }         

      private:
         std::ostream& out;
      };
   }

   inline vm::ast::node_list compile(front_end::ast::stache_root const & ast)
   {
      vm::ast::node_list vm_root;
      for(auto const & node : ast.nodes)
      {
         detail::stache_visit visit(std::cout);
         vm_root.nodes.push_back( boost::apply_visitor(visit, node) );
      }

      return vm_root;
   }
}}}}

#endif

