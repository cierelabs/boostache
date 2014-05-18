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
         std::string operator()(const stache_string_function& f) const
         {
            return operator()(f());
         }
         std::string operator()(const stache_bool_function& f) const
         {
            return f() ? "true" : "false";
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

         stache_model_printer(std::ostream& out, const stache_model& model, const stache_model_printer* parent = nullptr)
            : out(out)
            , model(model)
            , parent(parent)
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
            // TODO: Escaping.
            if( auto location = lookup(v.value) )
            {
               stache_model_visitor visitor;
               out << boost::apply_visitor(visitor, *location);
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
            const stache_variant* location = lookup(v.name);
            bool positive = have_value(location);
            if( positive && !v.is_inverted )
            {
               if (auto vec = get<stache_model_vector>(location))
               {
                  for( const auto& entry : *vec )
                  {
                     const stache_model* m = boost::get<stache_model>(&entry);
                     if( m )
                     {
                        stache_model_printer section_printer(out, *m, this);
                        apply_visitor_to_root(section_printer, v.nodes);
                     }
                     else
                     {
                        apply_visitor_to_root(*this, v.nodes);
                     }
                  }
               }
               else if (auto model = get<stache_model>(location))
               {
                  stache_model_printer section_printer(out, *model, this);
                  apply_visitor_to_root(section_printer, v.nodes);
               }
               else
               {
                  // This is the odd case that they requested a section, but it was
                  // some non-mapped type.  We can handle this by recursively calling
                  // the printer with the nodes of the section.  Dynamic lookup can
                  // still find the section tag for proper display within the
                  // section.
                  apply_visitor_to_root(*this, v.nodes);
               }
            }
            else if( !positive && v.is_inverted )
            {
               apply_visitor_to_root(stache_model_printer(out, stache_model(), this), v.nodes);
            }
         }

      private:
         template <typename T>
         const T* get(const stache_variant* location) const
         {
            return boost::get<T>(location);
         }

         bool have_value(const stache_variant* location) const
         {
            if( location != nullptr )
            {
               if( auto f = get<stache_bool_function>(location) )
               {
                  return (*f)();
               }
               return true;
            }
            return false;
         }

         // Recursive lookup to any parent printers to support scoped name lookup.
         const stache_variant* lookup(const std::string& name) const
         {
            auto location = model.find(name);
            if( location != model.end() )
            {
               return &(location->second);
            }
            else if( parent )
            {
               return parent->lookup(name);
            }
            return nullptr;
         }

         std::ostream& out;
         const stache_model& model;
         const stache_model_printer* parent;
      };
   }

   inline void print(std::ostream& out, stache_node_list const& root, const stache_model& model)
   {
      detail::stache_model_printer p(out, model);
      apply_visitor_to_root(p, root);
   }
}}}}

#endif
