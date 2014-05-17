/**
 *  \file stache_model_printer.hpp
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

#include <type_traits>

#include <boost/variant/apply_visitor.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>

namespace boost { namespace cppte { namespace model
{

/** This template function is intended to specialization for user own
 * types and should return variable for given key.
 */
template <typename model_type>
std::string get_variable_value(const model_type &, const std::string &key)
{
    // TODO(burlog): do something better
    return "undefined:" + key + "|" + __PRETTY_FUNCTION__;
}

/** This template class is intended to specialization for user own types and
 * should call given meta callback with type of submodel.
 */
template <
    template <typename submodel_type> class printer_callback_type,
    typename model_type
>
struct pass_section_value_to_callback
{
    void operator()(const model_type &,
                    const std::string &key,
                    std::ostream &,
                    const front_end::ast::section &)
    {
        // TODO(burlog): do something better
        throw std::runtime_error("undefined:" + key + "|" + __PRETTY_FUNCTION__);
    }
};

template <typename model_type>
struct single_obj_as_array {
    single_obj_as_array(const model_type &model)
        : model(model)
    {}
    typedef model_type value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef const value_type *const_iterator;
    const_iterator begin() const { return &model;}
    const_iterator end() const { return &model + 1;}
    const model_type &model;
};

template <typename model_type>
single_obj_as_array<model_type> make_single_obj_as_array(const model_type &model)
{
    return single_obj_as_array<model_type>(model);
}

namespace detail
{

template <typename model_type>
class dynamic_model_printer
{
public:
    typedef void result_type;

    dynamic_model_printer(std::ostream& out, const model_type &model)
        : out(out)
        , model(model)
    {}

    template <typename submodel_range_type>
    class section_callback
    {
    public:
        void operator()(std::ostream &out,
                        const submodel_range_type &submodels,
                        front_end::ast::section const &v) const
        {
            for (const auto &submodel: submodels)
            {
                auto submodel_printer = make_printer(out, submodel);
                for( const auto& node : v.nodes)
                {
                    boost::apply_visitor(submodel_printer, node);
                }
            }
        }

        template <typename submodel_type>
        dynamic_model_printer<submodel_type>
        make_printer(std::ostream &out, const submodel_type &submodel) const
        {
            return dynamic_model_printer<submodel_type>(out, submodel);
        }

        template <typename key_type, typename submodel_type>
        dynamic_model_printer<submodel_type>
        make_printer(std::ostream &out,
                     const std::pair<const key_type, submodel_type> &pair) const
        {
            return dynamic_model_printer<submodel_type>(out, pair.second);
        }
    };

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

    void operator()(front_end::ast::variable const &v) const
    {
        // TODO(burlog): optional
        out << get_variable_value(model, v.value);
    }

    void operator()(front_end::ast::section const &v) const
    {
        // TODO(burlog): optional
        pass_section_value_to_callback<section_callback, model_type>()
            (model, v.name, out, v);
    }

private:
    std::ostream &out;
    const model_type &model;
};

} // detail

template <typename model_type>
void print(std::ostream &out,
           const front_end::ast::stache_root &root,
           const model_type &model)
{
    detail::dynamic_model_printer<model_type> printer(out, model);
    for(const auto &node: root)
    {
        boost::apply_visitor(printer, node);
    }
}

}}}

#endif

