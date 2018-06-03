/**
 *  \file generate.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_GENERATE_HPP
#define BOOST_BOOSTACHE_VM_GENERATE_HPP

#include <boost/boostache/vm/detail/engine_visitor.hpp>

namespace boost {
    namespace boostache {
        namespace vm
        {
            template <typename Stream, typename Template, typename Context, typename Stack, typename Global>
            void generate(Stream & stream
                , Template const & templ
                , Context const & context
                , Stack const* stack, Global const* global
            )
            {
                vm::detail::generate(stream, templ, context, stack, global);
            }
        }
    }
}

#endif
