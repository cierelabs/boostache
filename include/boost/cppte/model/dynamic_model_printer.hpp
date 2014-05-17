/**
 *  \file dynamic_model_printer.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Kevin Harris
 *  Copyright 2014 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CPPTE_MODEL_DYNAMIC_MODEL_PRINTER_HPP
#define BOOST_CPPTE_MODEL_DYNAMIC_MODEL_PRINTER_HPP

#include <boost/range/empty.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>

namespace boost { namespace cppte { namespace model
{

namespace detail
{

struct empty_model {};
empty_model *begin(std::pair<empty_model *, empty_model *> p) { return p.first;}
empty_model *end(std::pair<empty_model *, empty_model *> p) { return p.second;}

} // namespace detail

template <typename model_type>
class dynamic_model_printer
{
public:
    typedef void result_type;

    dynamic_model_printer(std::ostream& out, const model_type &model)
        : out(out), model(model)
    {}

    void operator()(front_end::ast::comment) const
    {
        // just ignore comments
    }

    void operator()(front_end::ast::partial) const
    {
        // FIXME: implement me please
    }

    void operator()(front_end::ast::undefined) const
    {
        out << "WHOA! we have an undefined" << std::endl;
    }

    void operator()(front_end::ast::literal_text const &v) const
    {
        out << v;
    }

    void operator()(front_end::ast::variable const &v) const;

    void operator()(front_end::ast::section const &v) const;

private:
    std::ostream &out;
    const model_type &model;
};

template <typename parent_model_type>
struct section_range_sink: public boost::noncopyable {
    section_range_sink(std::ostream &out, front_end::ast::section const &v)
        : out(out), v(v), printed(false)
    {}

    template <typename submodel_range_type>
    void operator()(const submodel_range_type &submodels)
    {
        printed = true;
        if (!v.is_inverted) {
            for (const auto &submodel: submodels)
            {
                auto submodel_printer = make_printer(submodel);
                for( const auto& node : v.nodes)
                {
                    boost::apply_visitor(submodel_printer, node);
                }
            }

        }
        else if (v.is_inverted && boost::empty(submodels))
        {
            auto submodel_printer = make_printer(detail::empty_model());
            for( const auto& node : v.nodes)
            {
                boost::apply_visitor(submodel_printer, node);
            }
        }
    }

    template <typename submodel_type>
    dynamic_model_printer<submodel_type>
    make_printer(const submodel_type &submodel) const
    {
        return dynamic_model_printer<submodel_type>(out, submodel);
    }

    template <typename key_type, typename submodel_type>
    dynamic_model_printer<submodel_type>
    make_printer(const std::pair<const key_type, submodel_type> &pair) const
    {
        return dynamic_model_printer<submodel_type>(out, pair.second);
    }

    bool isprinted() const { return printed;}

private:
    std::ostream &out;
    const front_end::ast::section &v;
    bool printed;
};

/** This template function is intended to specialization for user own
 * types and should return variable for given key.
 */
template <typename model_type>
std::string get_variable_value(const model_type &, const std::string &key)
{
    // TODO(burlog): do something better
    return "undefined:" + key + "|" + __PRETTY_FUNCTION__;
}

template <typename model_type>
void get_section_value(const model_type &,
                       const std::string &key,
                       section_range_sink<model_type> &)
{
    // TODO(burlog): do something better
    throw std::runtime_error("undefined:" + key + "|" + __PRETTY_FUNCTION__);
}

template <typename model_type>
void dynamic_model_printer<model_type>::operator()
    (front_end::ast::variable const &v) const
{
    // TODO(burlog): What is better? If variable does not exist then rended
    // TODO(burlog): something like "undefined" or leave it up to the user
    // TODO(burlog): to let it do what he wants? (empty string, exp, ...)
    out << get_variable_value(model, v.value);
}

template <typename model_type>
void dynamic_model_printer<model_type>::operator()
    (front_end::ast::section const &v) const
{
    section_range_sink<model_type> sink(out, v);
    get_section_value(model, v.name, sink);
    if (!sink.isprinted())
    {
        // if user don't call sink it means that no section exist
        section_range_sink<model_type> sink(out, v);
        sink(std::pair<detail::empty_model *,
                       detail::empty_model *>(nullptr, nullptr));
    }
}

template <typename model_type>
void print(std::ostream &out,
           const front_end::ast::stache_root &root,
           const model_type &model)
{
    dynamic_model_printer<model_type> printer(out, model);
    for (const auto &node: root)
    {
        boost::apply_visitor(printer, node);
    }
}

}}}

#endif

