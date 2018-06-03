/**
 *  \file detail/engine_visitor.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/vm/detail/foreach.hpp>
#include <boost/boostache/vm/detail/select_context.hpp>
#include <boost/boostache/model/select_traits.hpp>


 ////////// some test hackery ////////////////////////

namespace boost {
    namespace boostache {
        namespace extension
        {
            template <typename Stream, typename Object>
            bool render_lit(Stream & stream, Object const & v)
            {
                stream << v;
                return true;
            }

            template <typename T, typename Stack, typename Global>
            std::pair<bool, Stack const*> test_with_stack(T const & context, Stack const* stack, Global const* global, std::string const & tag);

            template< typename Stream, typename T, typename Stack, typename Global>
            bool render_name(Stream & stream, T const & context, Stack const* stack, Global const* global, std::string const & name);
        }
    }
}

/////////////////////////////////////////////////////


namespace boost {
    namespace boostache {
        namespace vm {
            namespace detail
            {





                template <typename Stream, typename Context, typename Stack, typename Global>
                class engine_visitor_base
                {
                public:
                    typedef void result_type;

                    engine_visitor_base(Stream & s, Context const & c, Stack const* stack, Global const* global)
                        : stream(s)
                        , context(c)
                        , stack(stack)
                        , global(global)
                    {}

                    void operator()(ast::undefined) const
                    {}

                    void operator()(ast::nop) const
                    {}

                    void operator()(ast::literal const & lit) const
                    {
                        using boost::boostache::extension::render_lit;
                        render_lit(stream, lit.value);
                    }

                    void operator()(ast::variable const &) const
                    {}

                    void operator()(ast::render const & r) const
                    {
                        using boost::boostache::extension::render_name;
                        render_name(stream, context, stack, global, r.name);
                    }

                    void operator()(ast::for_each const & v) const
                    {
                        using boost::boostache::vm::detail::foreach;

                        foreach(stream, v, context, stack, global);
                    }

                    void operator()(ast::if_then_else const & v) const
                    {
                        using boost::boostache::extension::test_with_stack;
                        auto&& result = test_with_stack(context, stack, global, v.condition_.name);
                        if (!result.second) {
                            // found in current stack
                            if (result.first)
                            {
                                boost::apply_visitor(*this, v.then_);
                            }
                            else
                            {
                                boost::apply_visitor(*this, v.else_);
                            }
                        }
                        else {
                            // continue in the stack where element got found

                            boost::apply_visitor(boostache::detail::make_unwrap_variant_visitor<void>(
                                [this,&result,&v](auto ctx)
                            {
                                //Stack local(ctx, stack, global);
                                engine_visitor_base<Stream, decltype(ctx), Stack, Global> engine(stream, ctx, stack, global);
                                //engine(v);
                                if (result.first)
                                {
                                    engine(v.then_);
                                }
                                else
                                {
                                    engine(v.else_);
                                }
                            }
                                )
                                , result.second->current);

                            //Stack local(*result.second, &stack);
                            //engine_visitor_base engine(stream, context, local);
                            //engine(v);
                        }
                    }

                    void operator()(ast::select_context const & select_ctx) const
                    {
                        select_context_dispatch(stream, select_ctx, context, stack, global, false);
                    }

                    void operator()(ast::node_list const & nodes) const
                    {
                        for (auto const & node : nodes.nodes)
                        {
                            boost::apply_visitor(*this, node);
                        }
                    }

                    void operator()(ast::node const & node) const
                    {
                        boost::apply_visitor(*this, node);
                    }

                private:
                    Stream & stream;
                    Context const & context;
                    Stack const* stack;
                    Global const* global;
                };


                template <typename Stream, typename Template, typename Context, typename Stack, typename Global>
                void generate(Stream & stream
                    , Template const & templ
                    , Context const & ctx
                    , Stack const* stack, Global const* global
                )
                {
                    engine_visitor_base<Stream, Context, Stack, Global> engine(stream, ctx, stack, global);
                    engine(templ);
                }

            }
        }
    }
}

#endif
