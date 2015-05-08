/**
 *  \file vm/printer.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_PRINTER_HPP
#define BOOST_BOOSTACHE_VM_PRINTER_HPP

#include <boost/variant/apply_visitor.hpp>
#include <boost/boostache/vm/engine_ast.hpp>

namespace boost { namespace boostache { namespace vm { namespace ast
{
   namespace detail
   {
      class printer
      {
      public:
         typedef void result_type;

         printer(std::ostream& out)
            : out(out)
         {}

         void operator()(undefined) const
         {
            out << "[<UNDEFINED>]";
         }

         void operator()(literal const & v) const
         {
            out << "[<literal node> : " << v.value << "]";
         }

         void operator()(variable const & v) const
         {
            out << "[<variable> : " << v.name << "]";
         }

         void operator()(render const & v) const
         {
            out << "[<render> : " << v.name << "]";
         }

         void operator()(for_each const & v) const
         {
            out << "[<for_each> : " << std::endl;
            boost::apply_visitor(*this, v.value);
            out << "\n</for_each>]" << std::endl;
         }

         void operator()(condition const & v) const
         {}

         void operator()(if_then_else const & v) const
         {
            out << "[<if> : --------------------------- " << std::endl;
            //boost::apply_visitor(*this, v.condition_);
            out << "[<then> : --------------------------- " << std::endl;
            boost::apply_visitor(*this, v.then_);
            out << "\n]\n";
            out << "[<else> : --------------------------- " << std::endl;
            boost::apply_visitor(*this, v.else_);
            out << "\n</if>]" << std::endl;
         }

         void operator()(node_list const & v) const
         {
            for(auto const & node : v.nodes)
            {
               boost::apply_visitor(*this, node);
            }
         }

      private:
         std::ostream& out;
      };
   }

   inline void print(std::ostream& out, node const& root)
   {
      detail::printer p(out);
      boost::apply_visitor(p, root);
   }
}}}}

#endif
