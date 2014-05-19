/**
 *  \file test_dynamic_model.cpp
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

#include <map>
#include <unordered_map>

#include <boost/test/unit_test.hpp>
#include <boost/boostache/model/stache_model.hpp>
#include <boost/boostache/model/dynamic_model_printer.hpp>

#include "test_utils.hpp"

namespace
{

// just some user types
typedef std::map<std::string, std::string> map_of_strings;
typedef std::unordered_map<std::string, std::string> umap_of_strings;
struct user {
    std::string name;
    std::string location;
    int age;
    std::vector<map_of_strings> favorites;
    std::vector<umap_of_strings> unloved;
};
typedef std::map<std::string, user> map_of_users;

} // namespace

namespace boost { namespace boostache { namespace model
{

template <>
void get_variable_value(const map_of_strings &model,
                        const std::string &key,
                        variable_sink &sink)
{
    auto ivalue = model.find(key);
    if (ivalue != model.end()) sink(ivalue->second);
}

template <>
void get_variable_value(const umap_of_strings &model,
                        const std::string &key,
                        variable_sink &sink)
{
    auto ivalue = model.find(key);
    if (ivalue != model.end()) sink(ivalue->second);
}

template <>
void get_variable_value(const user &model,
                        const std::string &key,
                        variable_sink &sink)
{
    if (key == "NAME") sink(model.name);
    else if (key == "LOCATION") sink(model.location);
    else if (key == "AGE") sink(model.age);
}

template <>
void get_section_value(const user &model,
                       const std::string &key,
                       section_range_sink &sink)
{
    if (key == "FAVORITES") sink(model.favorites);
    else if (key == "UNLOVED") sink(model.unloved);
}

template <>
void get_section_value(const map_of_users &model,
                       const std::string &key,
                       section_range_sink &sink)
{
    if (key == "USER") sink(model);
}

}}}

BOOST_AUTO_TEST_CASE(test_simple_value)
{
    namespace bfe = boost::boostache::frontend;
    map_of_strings model;
    model["NAME"] = "Boosties";
    bfe::stache::ast::root ast = parse("Hello, {{NAME}}!");
    std::string result = print(ast, model);
    BOOST_CHECK_EQUAL("Hello,"
            " "
            "Boosties!", result);
}

BOOST_AUTO_TEST_CASE(test_section_printing)
{
    namespace bfe = boost::boostache::frontend;
    user bob;
    bob.name = "Bob";
    bob.location = "Earth";
    bob.age = 33;
    bob.favorites.push_back(map_of_strings());
    bob.favorites.back()["FOOD"] = "Pizza";
    bob.favorites.back()["MUSIC"] = "Classical";
    map_of_users model;
    model["USER"] = bob;
    bfe::stache::ast::root ast = parse(
            "{{#USER}}"
            "user.name={{NAME}}\n"
            "user.location={{LOCATION}}\n"
            "user.age={{AGE}}"
            "{{#FAVORITES}}"
            "user.favorite.food={{FOOD}}\n"
            "user.favorite.music={{MUSIC}}\n"
            "{{/FAVORITES}}"
            "{{/USER}}");

    BOOST_CHECK_EQUAL(
            "user.name=Bob\n"
            "user.location=Earth\n"
            "user.age=33" // FIXME: there should be \n !
            "user.favorite.food=Pizza\n"
            "user.favorite.music=Classical\n",
            print(ast, model));
}

