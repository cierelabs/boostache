/**
 *  \file generate_extension.hpp
 *
 *  Copyright 2014 Michael Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_GENERATE_EXTENSION_HPP
#define BOOST_BOOSTACHE_VM_GENERATE_EXTENSION_HPP

#include <boost/boostache/vm/generate.hpp>

namespace boost { namespace boostache { namespace extension
{

template <typename Stream, typename Context, typename = void>
struct generate_dispatcher {
    void operator()(Stream &stream,
                    const vm::ast::node &templ,
                    const Context &context) const
    {
        vm::generate(stream, templ, context);
    }
};

}}} // namespace boost::boostache::extension

#endif /* BOOST_BOOSTACHE_VM_GENERATE_EXTENSION_HPP */

