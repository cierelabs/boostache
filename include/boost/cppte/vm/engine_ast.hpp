/**
 *  \file engine_ast.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_VM_ENGINE_AST_HPP
#define BOOST_CPPTE_VM_ENGINE_AST_HPP

#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <vector>

namespace boost { namespace cppte { namespace vm { namespace ast
{
   struct literal;
   struct variable;
   struct for_each;
   struct if_then_else;
   struct node_list;

   struct undefined {};


   struct literal
   {
      literal(){}
      literal(std::string const & v) : value(v) {}
      std::string value;
   };

   struct variable
   {
      variable(){}
      variable(std::string const & v) : id(v) {}
      std::string id;
   };

   struct node : boost::spirit::extended_variant<
        undefined
      , literal
      , variable
      , boost::recursive_wrapper<for_each>
      , boost::recursive_wrapper<if_then_else>
      , boost::recursive_wrapper<node_list> >
   {
      node() : base_type() {}
      node(literal const & rhs) : base_type(rhs) {}
      node(variable const & rhs) : base_type(rhs) {}
      node(for_each const & rhs) : base_type(rhs) {}
      node(if_then_else const & rhs) : base_type(rhs) {}
      node(node_list const & rhs) : base_type(rhs) {}
   };

   struct for_each
   {
      node value;
   };

   struct condition
   {
      // lhs
      // rhs
      // op
   };

   struct if_then_else
   {
      condition condition_;
      node if_;
      node else_;
   };

   struct node_list
   {
      std::vector<node> nodes;
   };

}}}}

#endif
