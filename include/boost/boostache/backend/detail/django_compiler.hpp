/**
 *  \file detail/django_compiler.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_BACKEND_DETAIL_DJANGO_COMPILER_HPP
#define BOOST_BOOSTACHE_BACKEND_DETAIL_DJANGO_COMPILER_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/frontend/django/ast.hpp>
#include <boost/boostache/backend/detail/django_compiler.hpp>

namespace boost { namespace boostache { namespace backend { namespace django_compiler
{
   namespace fe = boost::boostache::frontend;
   namespace detail
   {
      class django_visit
      {
      public:
         typedef vm::ast::node result_type;

         django_visit(std::ostream& out)
            : out(out)
         {}

         vm::ast::node operator()(fe::django::ast::undefined) const
         {
            out << "WHOA! we have an undefined" << std::endl;
            return vm::ast::node{};
         }

         vm::ast::node operator()(fe::django::ast::literal_text const & v) const
         {
            return vm::ast::literal{v};
         }

         vm::ast::node operator()(fe::django::ast::variable const & v) const
         {
            vm::ast::node body = vm::ast::render{v.back()};
            for(auto iter = --v.rend(); iter != v.rbegin(); --iter)
            {
               vm::ast::select_context select;
               select.tag = *iter;
               select.body = std::move(body);
               body = std::move(select);
            }
            return body;
         }

         vm::ast::node operator()(fe::django::ast::comment const & v) const
         {
            return vm::ast::literal{};
         }

         vm::ast::node operator()(fe::django::ast::node_list const & nodes) const
         {
            vm::ast::node_list node_list;
            for(auto const & node : nodes)
            {
               node_list.nodes.push_back(boost::apply_visitor(*this, node));
            }
            return node_list;
         }

         vm::ast::node operator()(fe::django::ast::root const & nodes) const
         {
            vm::ast::node_list node_list;
            for(auto const & node : nodes)
            {
               node_list.nodes.push_back(boost::apply_visitor(*this, node));
            }
            return node_list;
         }

      private:
         std::ostream& out;
      };
   }

   inline vm::ast::node compile(fe::django::ast::root const & ast)
   {
      detail::django_visit visit(std::cout);
      return visit(ast);
   }
}}}}

#endif

