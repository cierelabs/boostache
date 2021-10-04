/**
 *  \file detail/select_context.hpp
 *
 *  Copyright 2017 Tobias Loew : die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_MULTI_CONTEXT_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_MULTI_CONTEXT_HPP

#include <string>
#include <boost/optional.hpp>
#include <boost/boostache/vm/engine_ast.hpp>


namespace boost { namespace boostache { namespace vm { namespace detail
{
	template <typename Context, typename Stream>
	struct multi_context {
		multi_context(Context const & context, std::function<void (Stream &, ast::select_context const &)> const & fun)
			: context(context)
			, fun(fun)
		{}

		Context const & context;
		std::function<void (Stream &, ast::select_context const &)> const & fun;
	};


	template <typename Context, typename Stream>
	auto make_multi_context(Context const & context, std::function<void (Stream &, ast::select_context const &)> const & fun)
	{
		return multi_context<Context, Stream>{context, fun};
	}


}}}}


#endif
