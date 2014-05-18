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
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_AST_ADAPTED_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_AST_ADAPTED_HPP

#include <boost/boostache/frontend/stache_ast.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
   boost::boostache::frontend::ast::variable,
   (bool, is_unescaped)
   (boost::boostache::frontend::ast::identifier, value)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::boostache::frontend::ast::section,
   (bool, is_inverted)
   (boost::boostache::frontend::ast::identifier, name)
   (boost::boostache::frontend::ast::stache_node_list, nodes)
)

#endif

