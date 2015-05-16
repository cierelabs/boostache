/**
 *  \file property_tree_render_extension.hpp
 *
 *  Copyright 2015 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BOOSTACHE_MODEL_EXTENSIONS_PROPERTY_TREE_HPP
#define BOOST_BOOSTACHE_MODEL_EXTENSIONS_PROPERTY_TREE_HPP

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/model/render_traits.hpp>
#include <boost/boostache/model/basic_render_extension.hpp>
#include <boost/property_tree/ptree.hpp>
#include <type_traits>

namespace boost { namespace boostache { namespace extension {

bool test(const property_tree::ptree &context,
          const std::string &tag,
          container_attribute)
{
    std::cout << "TEST: '" << tag << "'" << std::endl;
    auto ientry = context.find(tag);
    if (ientry != context.not_found()) {
        return !ientry->second.empty()
            || test(ientry->second.data(),
                    test_category<decltype(ientry->second.data())>::type{});
    }
    return false;
}

template <typename Stream>
void render(Stream &&stream,
            const property_tree::ptree &context,
            const std::string &tag,
            container_attribute)
{
    std::cout << "RENDER: '" << tag << "'" << std::endl;
    auto ientry = context.find(tag);
    if (ientry != context.not_found()) {
        std::cout << "RENDER_FOUND: '" << tag << "'='"
                  << ientry->second.data() << "'" << std::endl;
        render(std::forward<Stream>(stream), ientry->second.data(), tag);
    }
}

template <typename Stream>
void select_context_dispatch(Stream &stream,
                             const vm::ast::select_context &v,
                             const property_tree::ptree &context,
                             container_attribute)
{
    std::cout << "SELECT: '" << v.tag << "' " << std::endl;
    auto ientry = context.find(v.tag);
    if (ientry != context.not_found())
        generate(stream, v.body, ientry->second);
    else
        generate(stream, v.body, context);
}

template <typename Stream, typename Node>
void foreach(Stream &stream,
             const Node &node,
             const property_tree::ptree &context,
             container_attribute)
{
    if (!context.empty() && context.front().first.empty()) {
        for (const auto &entry: context) {
            std::cout << "FOREACH: [" << std::endl;
            generate(stream, node.value, entry.second);
            std::cout << "FOREACH: ]" << std::endl;
        }
    } else {
        std::cout << "FOREACH: {" << std::endl;
        generate(stream, node.value, context);
        std::cout << "FOREACH: }" << std::endl;
    }
}

}}} // namespace boost::boostache::extension

#endif /* BOOST_BOOSTACHE_MODEL_EXTENSIONS_PROPERTY_TREE_HPP */

