/**
 *  \file detail/select_context.hpp
 *
 *  Copyright 2017 Tobias Loew : die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_STACKED_CONTEXT_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_STACKED_CONTEXT_HPP

#include <string>
#include <boost/optional.hpp>

//namespace boost { namespace boostache { namespace vm { namespace detail
//{
//	template <typename ParentContext, typename ChildContext>
//	struct stacked_context {
//		stacked_context(ParentContext const & parent, ChildContext const & child, boost::optional<std::string> bound_variable)
//			: parent(parent)
//			, child(child)
//			, bound_variable(bound_variable)
//		{}
//
//		ParentContext const & parent;
//		ChildContext const & child;
//		boost::optional<std::string> bound_variable;	
//	};
//
//	// return a parent/child stacked_context
//	template <typename ParentContext, typename ChildContext>
//	auto make_stacked_context(ParentContext const & parent, ChildContext const & child)
//	{
//		return stacked_context<ParentContext, ChildContext>{parent, child, boost::none};
//	}
//
//	template <typename ParentContext, typename ChildContext>
//	auto make_stacked_context(ParentContext const & parent, ChildContext const & child, boost::optional<std::string> bound_variable)
//	{
//		return stacked_context<ParentContext, ChildContext>{parent, child, bound_variable};
//	}
//
//
//	// merge_contexts will put the child-context on top of parent's parent context
//	// if there's no parent's parent context it returns just the child-context
//	// this is used to transfer on the correct contexts from foreach and ensures
//	// that the returned type complexity is smaller than that of parent 
//	// (for the stacked_context-case, child is always less complex than parent.child)
//	template <typename ParentContext, typename ChildContext>
//	auto merge_contexts(ParentContext const & parent, ChildContext const & child, boost::optional<std::string> bound_variable)
//	{
//		return child;
//	}
//
//	template <typename ParentParentContext, typename ParentChildContext, typename ChildContext>
//	auto merge_contexts(stacked_context<ParentParentContext, ParentChildContext> const & parent, ChildContext const & child, boost::optional<std::string> bound_variable)
//	{
//		return make_stacked_context(parent.parent, child, bound_variable);
//	}
//
//}}}}


#endif
