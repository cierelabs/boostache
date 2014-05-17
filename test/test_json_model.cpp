/**
 *  \file test_json_model.cpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Kevin Harris
 *  Copyright 2014 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <boost/cppte/simple_parser.hpp>
#include <boost/cppte/model/dynamic_model_printer.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "test_utils.hpp"

namespace boost { namespace cppte { namespace model
{
    template <>
    std::string get_variable_value(const boost::property_tree::ptree &model,
                                   const std::string &key)
    {
        return model.get(key, "json:undefined:" + key);
    }

    template <template <typename> class model_printer_type>
    struct pass_section_value_to_callback<
        model_printer_type,
        boost::property_tree::ptree
    >
    {
        void operator()(const boost::property_tree::ptree &model,
                        const std::string &key,
                        std::ostream &out,
                        const boost::cppte::front_end::ast::section &v) const
        {
            // in terms of property tree are nodes with empty key array entries
            const boost::property_tree::ptree &submodel = model.get_child(key);
            if (submodel.empty() || !submodel.front().first.empty())
            {
                // wrap single node into array wrapper
                auto fake_array = make_single_obj_as_array(submodel);
                model_printer_type<decltype(fake_array)>()(out, fake_array, v);

            } else {
                model_printer_type<decltype(submodel)>()(out, submodel, v);
            }
        }
    };

}}}

BOOST_AUTO_TEST_CASE(test_json_simple_value)
{
    // prepare model
    namespace bpt = boost::property_tree;
    namespace bfe = boost::cppte::front_end;
    bpt::ptree model;
    model.put("NAME", "Muchomurka");

    // parse template
    bfe::ast::stache_root ast = parse(
            "Amanita in czech is {{NAME}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "Amanita in czech is Muchomurka",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_json_single_obj_as_section)
{
    // prepare model
    namespace bpt = boost::property_tree;
    namespace bfe = boost::cppte::front_end;
    bpt::ptree model;
    model.put("MUSHROOMS.NAME", "Muchomurka Zelena");

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}\n{{/MUSHROOMS}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "Muchomurka Zelena\n",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_json_single_empty_obj_as_section)
{
    // prepare model
    namespace bpt = boost::property_tree;
    namespace bfe = boost::cppte::front_end;
    bpt::ptree model;
    bpt::ptree empty;
    model.add_child("MUSHROOMS", empty);

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}\n{{/MUSHROOMS}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "json:undefined:NAME\n", // TODO(burlog): something better?
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_json_array_as_section)
{
    // prepare model
    namespace bpt = boost::property_tree;
    namespace bfe = boost::cppte::front_end;
    bpt::ptree model;

    bpt::ptree mushrooms;

    bpt::ptree mushroom1;
    mushroom1.put("NAME", "Muchomurka Zelena");
    mushrooms.push_back(std::make_pair("", mushroom1));

    bpt::ptree mushroom2;
    mushroom2.put("NAME", "Hrib Hnedy");
    mushrooms.push_back(std::make_pair("", mushroom2));

    model.add_child("MUSHROOMS", mushrooms);

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}\n{{/MUSHROOMS}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "Muchomurka Zelena\n"
            "Hrib Hnedy\n",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_json_section_printing)
{
    namespace bpt = boost::property_tree;
    namespace bfe = boost::cppte::front_end;
    bpt::ptree model;
    model.put("USER.NAME", "Bob");
    model.put("USER.LOCATION", "Earth");

    bpt::ptree fs;
    bpt::ptree f1;
    f1.put("FOOD", "Pizza");
    f1.put("MUSIC", "Classical");
    fs.push_back(std::make_pair("", f1));

    bpt::ptree f2;
    f2.put("FOOD", "Knedlik");
    f2.put("MUSIC", "Folk");
    fs.push_back(std::make_pair("", f2));

    model.add_child("USER.FAVORITES", fs);

    bfe::ast::stache_root ast = parse(
            "{{#USER}}"
            "user.name={{NAME}}\n"
            "user.location={{LOCATION}}\n"
            "{{#FAVORITES}}"
            "user.favorite.food={{FOOD}}\n"
            "user.favorite.music={{MUSIC}}\n"
            "{{/FAVORITES}}"
            "{{/USER}}");

    // FIXME! Whitespace!
    // Should have a trailing newline on all of these lines.
    BOOST_CHECK_EQUAL(
            "user.name=Bob\n"
            "user.location=Earth" // FIXME: there should be \n !
            "user.favorite.food=Pizza\n"
            "user.favorite.music=Classical\n"
            "user.favorite.food=Knedlik\n"
            "user.favorite.music=Folk\n",
            print(ast, model));
}

