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


namespace boost { namespace boostache { namespace vm { namespace detail
{
	template <typename ParentContext, typename ChildContext>
	struct stacked_context {
		stacked_context(ParentContext const & parent, ChildContext const & child)
			: parent(parent)
			, child(child)
		{}

		ParentContext const & parent;
		ChildContext const & child;
	};


}}}}


#endif
