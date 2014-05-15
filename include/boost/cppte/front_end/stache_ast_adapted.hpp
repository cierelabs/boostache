/**
 *  \file stache_ast_adapted.hpp
 *
 *  Fusion adaption for the stache ast
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_AST_ADAPTED_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_AST_ADAPTED_HPP

#include <boost/cppte/front_end/stache_ast.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
   boost::cppte::front_end::ast::variable,
   (bool, is_unescaped)
   (boost::cppte::front_end::ast::identifier, value)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::cppte::front_end::ast::section,
   (bool, is_inverted)
   (std::vector<boost::cppte::front_end::ast::stache_node>, nodes)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::cppte::front_end::ast::stache_root,
   (std::vector<boost::cppte::front_end::ast::stache_node>, nodes)
)

#endif

