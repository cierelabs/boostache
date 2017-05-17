/**
 *  \file ast_adapted.hpp
 *
 *  Fusion adaption for the stache ast
 *
 *  Copyright 2015 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_DJANGO_AST_ADAPTED_HPP
#define BOOST_BOOSTACHE_FRONT_END_DJANGO_AST_ADAPTED_HPP

#include <boost/boostache/frontend/django/ast.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
   boost::boostache::frontend::django::ast::condition,
   (boost::boostache::frontend::django::ast::variable, condition)
   (boost::boostache::frontend::django::ast::node_list, body)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::boostache::frontend::django::ast::if_elif_else,
   (boost::boostache::frontend::django::ast::condition, if_)
   (std::vector<boost::boostache::frontend::django::ast::condition>, elif)
   (boost::optional<boost::boostache::frontend::django::ast::node_list>, else_)
)

#endif

