/**
 *  \file test_default_model.cpp
 *
 *  Copyright 2014 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <boost/boostache/simple_parser.hpp>
#include <boost/boostache/model/dynamic_model_printer.hpp>
#include <boost/boostache/model/default_adapter.hpp>

#include "test_utils.hpp"

BOOST_AUTO_TEST_CASE(test_default_simple_value)
{
    // prepare model
    namespace bfe = boost::boostache::frontend;
    bfe::stache_model model;
    model["NAME"] = std::string("Muchomurka");

    // parse template
    bfe::ast::stache_root ast = parse(
            "Amanita in czech is {{NAME}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "Amanita in czech is Muchomurka",
            print(ast, model));
}

// TODO(burlog): if we add int into variant...
//BOOST_AUTO_TEST_CASE(test_simple_int_value)
//{
//    // prepare model
//    namespace bpt = boost::property_tree;
//    namespace bfe = boost::boostache::frontend;
//    bpt::ptree model;
//    model.put("NUMBER", 3);
//
//    // parse template
//    bfe::ast::stache_root ast = parse(
//            "{{NUMBER}} is three");
//
//    // render and check
//    BOOST_CHECK_EQUAL(
//            "3 is three",
//            print(ast, model));
//}

BOOST_AUTO_TEST_CASE(test_parent_variable)
{
    // prepare model
    namespace bfe = boost::boostache::frontend;
    bfe::stache_model model;
    model["EXCLAMATION"] = std::string("!");

    bfe::stache_model mushroom;
    mushroom["NAME"] = std::string("Muchomurka Zelena");

    model["MUSHROOMS"] = mushroom;

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}{{EXCLAMATION}}\n{{/MUSHROOMS}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "Muchomurka Zelena!\n",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_single_obj_as_section)
{
    // prepare model
    namespace bfe = boost::boostache::frontend;
    bfe::stache_model model;

    bfe::stache_model mushroom;
    mushroom["NAME"] = std::string("Muchomurka Zelena");

    model["MUSHROOMS"] = mushroom;

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}\n{{/MUSHROOMS}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "Muchomurka Zelena\n",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_array_as_section)
{
    // prepare model
    namespace bfe = boost::boostache::frontend;
    bfe::stache_model model;

    bfe::stache_model_vector mushrooms;

    bfe::stache_model mushroom1;
    mushroom1["NAME"] = std::string("Muchomurka Zelena");
    mushrooms.push_back(mushroom1);

    bfe::stache_model mushroom2;
    mushroom2["NAME"] = std::string("Hrib Hnedy");
    mushrooms.push_back(mushroom2);

    model["MUSHROOMS"] = mushrooms;

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}\n{{/MUSHROOMS}}");

    // render and check
    BOOST_CHECK_EQUAL(
            "Muchomurka Zelena\n"
            "Hrib Hnedy\n",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_single_obj_as_inv_section)
{
    // prepare model
    namespace bfe = boost::boostache::frontend;
    bfe::stache_model model;

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}\n{{/MUSHROOMS}}"
            "{{^MUSHROOMS}}No mushrooms\n{{/MUSHROOMS}}"
            );

    // render and check
    BOOST_CHECK_EQUAL(
            "No mushrooms\n",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_single_obj_as_inv_empty_section)
{
    // prepare model
    namespace bfe = boost::boostache::frontend;
    bfe::stache_model model;
    bfe::stache_model_vector mushrooms;
    model["MUSHROOMS"] = mushrooms;

    // parse template
    bfe::ast::stache_root ast = parse(
            "{{#MUSHROOMS}}{{NAME}}\n{{/MUSHROOMS}}"
            "{{^MUSHROOMS}}No mushrooms\n{{/MUSHROOMS}}"
            );

    // render and check
    BOOST_CHECK_EQUAL(
            "No mushrooms\n",
            print(ast, model));
}

BOOST_AUTO_TEST_CASE(test_section_printing)
{
    namespace bfe = boost::boostache::frontend;
    bfe::stache_model model;

    bfe::stache_model user;
    user["NAME"] = std::string("Bob");
    user["LOCATION"] = std::string("Earth");

    bfe::stache_model_vector favorites;

    bfe::stache_model favorite1;
    favorite1["FOOD"] = std::string("Pizza");
    favorite1["MUSIC"] = std::string("Classical");
    favorites.push_back(favorite1);

    bfe::stache_model favorite2;
    favorite2["FOOD"] = std::string("Knedlik");
    favorite2["MUSIC"] = std::string("Folk");
    favorites.push_back(favorite2);

    user["FAVORITES"] = favorites;
    model["USER"] = user;

    bfe::ast::stache_root ast = parse(
            "{{#USER}}"
            "user.name={{NAME}}\n"
            "user.location={{LOCATION}}\n"
            "{{#FAVORITES}}"
            "user.favorite.food={{FOOD}}\n"
            "user.favorite.music={{MUSIC}}\n"
            "{{/FAVORITES}}"
            "{{/USER}}");

    BOOST_CHECK_EQUAL(
            "user.name=Bob\n"
            "user.location=Earth\n"
            "user.favorite.food=Pizza\n"
            "user.favorite.music=Classical\n"
            "user.favorite.food=Knedlik\n"
            "user.favorite.music=Folk\n",
            print(ast, model));
}

