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

#include <type_traits>

#include <boost/variant/apply_visitor.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>

namespace boost { namespace cppte { namespace front_end { namespace ast
{
    template <typename model_type>
    std::string get_variable_value(const model_type &, const std::string &key)
    {
        // TODO(burlog): do something better
        return "undefined:" + key;
    }

    template <template <typename submodel_type> class model_printer_type, typename model_type>
    struct call_me_back_please {
        template <typename type>
        void operator()(const type &,
                        const std::string &key,
                        std::ostream &,
                        const section &)
        {
            // TODO(burlog): do something better
            throw std::runtime_error("undefined:" + key);
        }
    };

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

      template <typename model_type>
      class stache_model_printer;

      template <typename submodel_range_type>
      class stache_model_printer_callback
      {
      public:
         void operator()(std::ostream& out, const submodel_range_type &submodels, section const & v) const
         {
             for (const auto &submodel: submodels) {
                 typedef typename std::remove_const<typename std::remove_reference<decltype(submodel)>::type>::type sub_t;
                 stache_model_printer<sub_t> submodel_printer(out, submodel);
                 for( const auto& node : v.nodes)
                 {
                     boost::apply_visitor(submodel_printer, node);
                 }
             }
         }

      };

      template <typename model_type>
      class stache_model_printer
      {
      public:
         typedef void result_type;

         stache_model_printer(std::ostream& out, const model_type &model)
            : out(out)
            , model(model)
         {}

         void operator()(comment) const {}

         void operator()(partial) const {}

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
             // TODO(burlog): optional
             out << get_variable_value(model, v.value);

				//stache_model_visitor visitor;
				//// TODO: Escaping.
				//auto location = model.find(v.value);
				//if( location != model.end() )
				//{
				//	out << boost::apply_visitor(visitor, location->second);
				//}
				//else
				//{
				//	out << "<<<<Undefined variable \"" << v.value << "\">>>>";
				//}
         }

         void operator()(section const & v) const
         {
             // TODO(burlog): optional
             call_me_back_please<stache_model_printer_callback, model_type>()(model, v.name, out, v);
				//// TODO: Inverted
				//auto location = model.find(v.name);
				//if( location != model.end() )
				//{
				//	stache_model_printer section_printer(out, boost::get<stache_model>(location->second));
				//	for( const auto& node : v.nodes )
				//	{
				//		boost::apply_visitor(section_printer, node);
				//	}
				//}
         }

      private:
         std::ostream& out;
         const model_type &model;
      };

   }

   template <typename model_type>
   inline void print(std::ostream& out, stache_root const& root, const model_type& model)
   {
      detail::stache_model_printer<model_type> p(out, model);
      for(const auto& node : root)
      {
         boost::apply_visitor(p, node);
      }
   }
}}}}

#endif
