/**
 *  \file test_json_model.cpp
 *
 *  Copyright 2014 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CPPTE_MODEL_PROPERTY_TREE_ADAPTER_HPP
#define BOOST_CPPTE_MODEL_PROPERTY_TREE_ADAPTER_HPP

#include <array>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/cppte/model/dynamic_model_printer.hpp>

namespace boost { namespace cppte { namespace model
{

template <>
void get_variable_value(const boost::property_tree::ptree &model,
                        const std::string &key,
                        variable_sink &sink)
{
    auto ivar = model.find(key);
    if (ivar != model.not_found()) sink(ivar->second.data());
}

template <>
void get_section_value(const boost::property_tree::ptree &model,
                       const std::string &key,
                       section_range_sink &sink)
{
    auto isubmodel = model.find(key);
    if (isubmodel != model.not_found())
    {
        auto &submodel = isubmodel->second;
        if (!submodel.empty())
        {
            // in terms of property tree world: nodes with empty key are array
            // entries so pass them directly into printer otherwise create
            // temporary array
            if (submodel.front().first.empty()) sink(submodel);
            else sink(std::array<decltype(&submodel), 1>{{&submodel}});
        }
    }
}

}}}

#endif // BOOST_CPPTE_MODEL_PROPERTY_TREE_ADAPTER_HPP

