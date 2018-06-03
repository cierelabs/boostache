/**
 *  \file detail/django_compiler.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
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
            for(auto iter = ++v.rbegin(); iter != v.rend(); ++iter)
            {
               vm::ast::select_context select;
               select.tag = *iter;
               select.body = std::move(body);
			   select.is_context_local = std::next(iter) != v.rend();
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

         vm::ast::node operator()(fe::django::ast::if_elif_else const & if_elif_else) const
         {
            vm::ast::node_list then_;
            for(auto const & node : if_elif_else.if_.body)
            {
               then_.nodes.push_back(boost::apply_visitor(*this, node));
            }

            vm::ast::if_then_else if_then_else;
            if_then_else.condition_.name = if_elif_else.if_.condition_.back();
            if_then_else.then_ = std::move(then_);
            if(static_cast<bool>(if_elif_else.else_))
            {
               vm::ast::node_list else_;
               for(auto const & node : if_elif_else.else_.get())
               {
                  else_.nodes.push_back(boost::apply_visitor(*this, node));
               }
               if_then_else.else_ = std::move(else_);
            }

            return if_then_else;
         }

		 vm::ast::node operator()(fe::django::ast::for_in const & for_in) const
		 {
			 vm::ast::node_list vm_ast;
			 for (auto const & node : for_in.body)
			 {
				 vm_ast.nodes.push_back(boost::apply_visitor(*this, node));
			 }

			 vm::ast::for_each for_each_;
			 for_each_.name = for_in.iterator;
			 for_each_.value = vm_ast;

			 vm::ast::node body = for_each_;
			 for (auto iter = for_in.set.rbegin(); iter != for_in.set.rend(); ++iter)
			 {
				 vm::ast::select_context select;
				 select.tag = *iter;
				 select.body = std::move(body);
                 select.is_context_local = std::next(iter) != for_in.set.rend();
                 body = std::move(select);
			 }
			 return body;

			 //out << "WHOA! for_in not yet implemented" << std::endl;
			 //return vm::ast::node{};
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

