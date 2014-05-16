/**
 *  \file stache_model_printer.hpp
 *
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Kevin Harris
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_MODEL_PRINTER_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_MODEL_PRINTER_HPP

#include <boost/variant/apply_visitor.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>

namespace boost { namespace cppte { namespace front_end { namespace ast
{
   namespace detail
   {
		struct stache_model_visitor
		{
			typedef std::string result_type;
			std::string operator()(const stache_function& f) const
			{
				return f();
			}
			std::string operator()(const std::string& s) const
			{
				return s;
			}
			std::string operator()(const stache_model& m) const
			{
				// TODO: Do something useful
				return "Oops.  Don't know how to format a stache model.\n";
			}
		};

      class stache_model_printer
      {
      public:
         typedef void result_type;

         stache_model_printer(std::ostream& out, const stache_model& model)
            : out(out)
				, model(model)
         {}

         void operator()(undefined) const
         {
            out << "WHOA! we have an undefined" << std::endl;
         }

         void operator()(literal_text const & v) const
         {
            out << v;
         }

         void operator()(variable const & v) const
         {
				stache_model_visitor visitor;
				// TODO: Escaping.
				auto location = model.find(v.value);
				if( location != model.end() )
				{
					out << boost::apply_visitor(visitor, location->second);
				}
				else
				{
					out << "<<<<Undefined variable \"" << v.value << "\">>>>";
				}
         }

         void operator()(section const & v) const
         {
				// TODO: Inverted
				auto location = model.find(v.name);
				if( location != model.end() )
				{
					stache_model_printer section_printer(out, boost::get<stache_model>(location->second));
					for( const auto& node : v.nodes )
					{
						boost::apply_visitor(section_printer, node);
					}
				}
         }

      private:
         std::ostream& out;
			const stache_model& model;
      };
   }

   inline void print(std::ostream& out, stache_root const& root, const stache_model& model)
   {
      detail::stache_model_printer p(out, model);
      for(const auto& node : root.nodes)
      {
         boost::apply_visitor(p, node);
      }
   }
}}}}

#endif
