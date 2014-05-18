/**
 *  \file default_adapater.hpp
 *
 *  Copyright 2014 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BOOSTACHE_MODEL_DEFAULT_ADAPTER_HPP
#define BOOST_BOOSTACHE_MODEL_DEFAULT_ADAPTER_HPP

#include <array>
#include <string>
#include <functional>

#include <boost/variant/apply_visitor.hpp>
#include <boost/boostache/frontend/stache_model.hpp>
#include <boost/boostache/model/dynamic_model_printer.hpp>

namespace boost { namespace boostache { namespace model
{

namespace detail
{

struct section_getter_visitor
{
    typedef void result_type;

    section_getter_visitor(section_range_sink &sink): sink(sink) {}

    template <typename param_type>
    void operator()(const param_type &) const
    {
        sink("<<<variable/function is not section>>>");
    }

    void operator()(const std::vector<frontend::stache_variant> &model) const
    {
        sink(model);
    }

    void operator()(const frontend::stache_model &model) const
    {
        sink(std::array<decltype(&model), 1>{{&model}});
    }

    section_range_sink &sink;
};

struct variable_getter_visitor
{
    typedef void result_type;

    variable_getter_visitor(variable_sink &sink): sink(sink) {}

    template <typename param_type>
    void operator()(const param_type &value) const
    {
        sink(value);
    }

    template <typename return_type>
    void operator()(std::function<return_type()> function) const
    {
        (*this)(function());
    }

    void operator()(const std::vector<frontend::stache_variant> &) const
    {
        sink("<<<vector of entries is not variable>>>");
    }

    void operator()(const frontend::stache_model &) const
    {
        sink("<<<map of entries is not variable>>>");
    }

    variable_sink &sink;
};

} // namespace detail

template <>
void get_variable_value(const frontend::stache_model &model,
                        const std::string &key,
                        variable_sink &sink)
{
    auto ivar = model.find(key);
    if (ivar != model.end()) {
        detail::variable_getter_visitor get_variable(sink);
        boost::apply_visitor(get_variable, ivar->second);
    }
}

// TODO(burlog): if variant change remove this specialization
template <>
void get_variable_value(const frontend::stache_variant &model,
                        const std::string &key,
                        variable_sink &sink)
{
    auto m = boost::get<frontend::stache_model>(&model);
    if (m) get_variable_value(*m, key, sink);
}

template <>
void get_section_value(const frontend::stache_model &model,
                       const std::string &key,
                       section_range_sink &sink)
{
    auto isubmodel = model.find(key);
    if (isubmodel != model.end()) {
        detail::section_getter_visitor get_section(sink);
        boost::apply_visitor(get_section, isubmodel->second);
    }
}

}}}

#endif // BOOST_BOOSTACHE_MODEL_DEFAULT_ADAPTER_HPP

