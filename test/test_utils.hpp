/**
 *  \file test_utils.hpp
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_TEST_TEST_UTILS_HPP
#define BOOST_TEST_TEST_UTILS_HPP

#include <boost/cppte/simple_parser.hpp>
#include <boost/cppte/model/dynamic_model_printer.hpp>

template <typename model_type> inline
std::string print(const boost::cppte::front_end::ast::stache_root &ast,
                  const model_type &model)
{
    std::ostringstream out;
    boost::cppte::model::print(out, ast, model);
    return out.str();
}

inline boost::cppte::front_end::ast::stache_root parse(const std::string &text)
{
    boost::cppte::front_end::ast::stache_root ast;
    if( !boost::cppte::simple_parse_template(text, ast) )
    {
        throw std::runtime_error("Parse failed");
    }
    return ast;
}

#endif

