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

#ifndef BOOST_BOOSTACHE_MODEL_DYNAMIC_MODEL_PRINTER_HPP
#define BOOST_BOOSTACHE_MODEL_DYNAMIC_MODEL_PRINTER_HPP

#include <array>
#include <typeinfo>
#include <boost/range/empty.hpp>
#include <boost/function.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/boostache/frontend/stache_ast.hpp>

namespace boost { namespace boostache { namespace model
{

namespace detail
{

struct empty_model {};
struct root_parent_printer {};

typedef boost::function<
            void (const frontend::ast::variable &v)
        > parent_lookup_type;

template <typename model_type>
class dynamic_model_printer
{
public:
    typedef void result_type;

    dynamic_model_printer(std::ostream &out,
                          const model_type &model,
                          parent_lookup_type parent_lookup
                          = parent_lookup_type())
        : out(out), model(model), parent_lookup(parent_lookup)
    {}

    void operator()(frontend::ast::comment) const
    {
        // just ignore comments
    }

    void operator()(frontend::ast::partial) const
    {
        // FIXME: implement me please
    }

    void operator()(frontend::ast::undefined) const
    {
        out << "WHOA! we have an undefined" << std::endl;
    }

    void operator()(const frontend::ast::literal_text &v) const
    {
        out << v;
    }

    void operator()(const frontend::ast::variable &v) const;

    void operator()(const frontend::ast::section &v) const;

private:
    std::ostream &out;
    const model_type &model;
    parent_lookup_type parent_lookup;
};

template <typename printer_type>
parent_lookup_type make_parent_lookup(printer_type *printer)
{
    return [printer] (const frontend::ast::variable &v) { (*printer)(v);};
}

} // namespace detail

struct variable_sink: public boost::noncopyable
{
    variable_sink(std::ostream &out, frontend::ast::variable const &v)
        : out(out), v(v), printed(false)
    {}

    template <typename variable_value_type>
    void operator()(const variable_value_type &value)
    {
        printed = true;
        out << value;
    }

    bool isprinted() const { return printed;}

private:
    std::ostream &out;
    const frontend::ast::variable &v;
    bool printed;
};

struct section_range_sink: public boost::noncopyable
{
    section_range_sink(std::ostream &out,
                       const frontend::ast::section &v,
                       detail::parent_lookup_type parent_lookup
                       = detail::parent_lookup_type())
        : out(out), v(v), printed(false), parent_lookup(parent_lookup)
    {}

    template <typename submodel_range_type>
    void operator()(const submodel_range_type &submodels)
    {
        printed = true;
        if (!v.is_inverted)
        {
            for (const auto &submodel: submodels)
            {
                auto submodel_printer = make_printer(submodel);
                for (const auto &node: v.nodes)
                {
                    boost::apply_visitor(submodel_printer, node);
                }
            }

        }
        else if (v.is_inverted && boost::empty(submodels))
        {
            auto submodel_printer = make_printer(detail::empty_model());
            for(const auto &node: v.nodes)
            {
                boost::apply_visitor(submodel_printer, node);
            }
        }
    }

    template <typename submodel_type>
    detail::dynamic_model_printer<submodel_type>
    make_printer(const submodel_type *submodel) const
    {
        return make_printer(*submodel);
    }

    template <typename submodel_type>
    detail::dynamic_model_printer<submodel_type>
    make_printer(const submodel_type &submodel) const
    {
        return detail::dynamic_model_printer<submodel_type>
               (out, submodel, parent_lookup);
    }

    template <typename key_type, typename submodel_type>
    detail::dynamic_model_printer<submodel_type>
    make_printer(const std::pair<const key_type, submodel_type> &pair) const
    {
        return detail::dynamic_model_printer<submodel_type>
               (out, pair.second, parent_lookup);
    }

    bool isprinted() const { return printed;}

private:
    std::ostream &out;
    const frontend::ast::section &v;
    bool printed;
    detail::parent_lookup_type parent_lookup;
};

/** This template function is intended to specialization for user own
 * types and should return variable for given key.
 */
template <typename model_type>
void get_variable_value(const model_type &,
                        const std::string &key,
                        variable_sink &)
{
    throw std::runtime_error("you should write specialization for "
                             "get_variable_value(" + key + ") for type: "
                             + typeid(model_type).name());
}

template <typename model_type>
void get_section_value(const model_type &,
                       const std::string &key,
                       section_range_sink &)
{
    throw std::runtime_error("you should write specialization for "
                             "get_section_value(" + key + ") for type: "
                             + typeid(model_type).name());
}

template <typename model_type>
void detail::dynamic_model_printer<model_type>::operator()
    (const frontend::ast::variable &v) const
{
    variable_sink sink(out, v);
    get_variable_value(model, v.value, sink);
    if (!sink.isprinted())
    {
        // if user don't call sink it means that no variable exist
        if (parent_lookup) parent_lookup(v);
    }
}

template <typename model_type>
void detail::dynamic_model_printer<model_type>::operator()
    (const frontend::ast::section &v) const
{
    section_range_sink sink(out, v, make_parent_lookup(this));
    get_section_value<model_type>(model, v.name, sink);
    if (!sink.isprinted())
    {
        // if user don't call sink it means that no section exist
        section_range_sink sink(out, v, make_parent_lookup(this));
        sink(std::array<detail::empty_model, 0>());
    }
}

template <typename model_type>
void print(std::ostream &out,
           const frontend::ast::stache_root &root,
           const model_type &model)
{
    // TODO(burlog): solve it
    // HACK - make the stache_root into a section
    frontend::ast::section section;
    section.is_inverted = false;
    section.nodes = root;
    section_range_sink sink(out, section);
    sink(std::array<const model_type *, 1>{{&model}});
}

}}}

#endif // BOOST_BOOSTACHE_MODEL_DYNAMIC_MODEL_PRINTER_HPP

