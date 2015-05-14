/**
 *  \file django/printer.hpp
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_DJANGO_PRINTER_HPP
#define BOOST_BOOSTACHE_FRONT_END_DJANGO_PRINTER_HPP

#include <boost/variant/apply_visitor.hpp>
#include <boost/boostache/frontend/django/ast.hpp>

namespace boost { namespace boostache { namespace frontend { namespace django { namespace ast
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

         template <typename T>
         void operator()(T& v) const
         {
            out << "WHOA! We have an unimplemented type: "
                << typeid(v).name() << std::endl;
         }

         void operator()(undefined) const
         {
            out << "undefined" << std::endl;
         }

         void operator()(comment) const
         {
            out << "{# some comment #}";
         }

         void operator()(literal_text const & v) const
         {
            out << v;
         }

         void operator()(variable const & var) const
         {
            out << "{% ";
            auto iter = var.begin();
            auto iter_end = var.end();
            while(iter != iter_end)
            {
               out << *iter;
               if(++iter != iter_end)
               {
                  out << ".";
               }
            }
         }

         void operator()(if_elif_else const & v) const
         {
            // TODO
         }

      private:
         std::ostream& out;
      };
   }

   inline void print(std::ostream& out, node_list const& nodes)
   {
      detail::printer p(out);
      for(auto const & node : nodes)
      {
         boost::apply_visitor(p, node);
      }
   }
}}}}}

#endif
