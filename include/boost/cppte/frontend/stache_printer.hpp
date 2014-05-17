/**
 *  \file stache_printer.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_PRINTER_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_PRINTER_HPP

#include <boost/variant/apply_visitor.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>

namespace boost { namespace cppte { namespace front_end { namespace ast
{
   namespace detail
   {
      class stache_printer
      {
      public:
         typedef void result_type;

         stache_printer(std::ostream& out)
            : out(out)
         {}

         void operator()(undefined) const
         {
            out << "WHOA! we have an undefined" << std::endl;
         }

         void operator()(comment) const
         {
         }

         void operator()(literal_text const & v) const
         {
            out << v;
         }

         void operator()(variable const & v) const
         {
            out << "{{";
            if(v.is_unescaped)
            {
               out << "&";
            }
            out << v.value << "}}";
         }

         void operator()(section const & v) const
         {
            out << "{{";
            if(v.is_inverted) {  out << "^";   }
            else            {  out << "#";   }
            out << v.name << "}}";

            for(auto const & node : v.nodes)
            {
               boost::apply_visitor(*this, node);
            }

            out << "{{/" << v.name << "}}";
         }

         void operator()(partial const & v) const
         {
            out << "{{>" << v << "}}";
         }

      private:
         std::ostream& out;
      };
   }

   inline void print(std::ostream& out, stache_root const& root)
   {
      detail::stache_printer p(out);
      for(auto const & node : root.nodes)
      {
         boost::apply_visitor(p, node);
      }
   }
}}}}

#endif
