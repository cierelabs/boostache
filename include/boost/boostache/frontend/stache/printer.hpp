/**
 *  \file stache_printer.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_PRINTER_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_PRINTER_HPP

#include <boost/variant/apply_visitor.hpp>
#include <boost/boostache/frontend/stache/ast.hpp>

namespace boost { namespace boostache { namespace frontend { namespace stache { namespace ast
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
            out << "WHOA! we have an undefined" << std::endl;
         }

         void operator()(blank_text const & v) const
         {
            out << "<blank_text>" << v;
         }

         void operator()(eol const & v) const
         {
            out << "<eol>" << v;
         }

         void operator()(comment) const
         {
            out << "<comment>";
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
            else              {  out << "#";   }
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
