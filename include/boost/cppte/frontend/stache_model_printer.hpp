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
		template <typename visitor>
		void apply_visitor_to_root(const visitor& v, const std::vector<stache_node>& root)
		{
			for( const auto& node : root )
			{
				boost::apply_visitor(v, node);
			}
		}

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
			std::string operator()(const stache_model_vector& v) const
			{
				return "Oops.  Don't know how to format a stache model vector.\n";
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

			void operator()(const comment& c) const
			{
				// Nothing to do.
			}

			void operator()(const partial& p) const
			{
				// TODO:  Something.
			}

         void operator()(section const & v) const
         {
				auto location = model.find(v.name);
				bool have_value = (location != model.end());
				if( have_value && !v.is_inverted )
				{
					const stache_model_vector* vec = boost::get<stache_model_vector>(&(location->second));
					if( vec )
					{
						for( const auto& entry : *vec )
						{
							const stache_model* m = boost::get<stache_model>(&entry);
							if( m )
							{
								stache_model_printer section_printer(out, *m);
								apply_visitor_to_root(section_printer, v.nodes);
							}
							else
							{
								apply_visitor_to_root(*this, v.nodes);
							}
						}
					}
					else
					{
						stache_model_printer section_printer(out, boost::get<stache_model>(location->second));
						apply_visitor_to_root(section_printer, v.nodes);
					}
				}
				else if( !have_value && v.is_inverted )
				{
					apply_visitor_to_root(stache_model_printer(out, stache_model()), v.nodes);
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
		apply_visitor_to_root(p, root);
   }
}}}}

#endif
