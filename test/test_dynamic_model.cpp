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
#include <boost/cppte/frontend/stache_model.hpp>
#include <boost/cppte/model/dynamic_model_printer.hpp>

#include "test_utils.hpp"

namespace
{

    // just some user types
    typedef std::map<std::string, std::string> map_of_strings;
    typedef std::unordered_map<std::string, std::string> umap_of_strings;
    struct user {
        std::string name;
        std::string location;
        std::vector<map_of_strings> favorites;
        std::vector<umap_of_strings> unloved;
    };
    typedef std::map<std::string, user> map_of_users;

} // namespace

namespace boost { namespace cppte { namespace model
{

    template <>
    std::string get_variable_value(const map_of_strings &model,
                                   const std::string &key)
    {
        auto ivalue = model.find(key);
        if (ivalue != model.end()) return ivalue->second;
        return "undefined:" + key;
    }

    template <>
    std::string get_variable_value(const umap_of_strings &model,
                                   const std::string &key)
    {
        auto ivalue = model.find(key);
        if (ivalue != model.end()) return ivalue->second;
        return "undefined:" + key;
    }

    template <>
    std::string get_variable_value(const user &model,
                                   const std::string &key)
    {
        if (key == "NAME") {
            return model.name;

        } else if (key == "LOCATION") {
            return model.location;
        }
        return "undefined:" + key;
    }

    template <template <typename> class model_printer_type>
    struct pass_section_value_to_callback<
        model_printer_type,
        user
    >
    {
        void operator()(const user &model,
                        const std::string &key,
                        std::ostream &out,
                        const boost::cppte::front_end::ast::section &v) const
        {
            if (key == "FAVORITES") {
                model_printer_type<decltype(model.favorites)>()
                    (out, model.favorites, v);

            } else if (key == "UNLOVED") {
                model_printer_type<decltype(model.unloved)>()
                    (out, model.unloved, v);

            } else {
                // TODO(burlog): what to do?
            }
        }
    };

    template <template <typename> class model_printer_type>
    struct pass_section_value_to_callback<
        model_printer_type,
        map_of_users
    >
    {
        void operator()(const map_of_users &model,
                        const std::string &key,
                        std::ostream &out,
                        const boost::cppte::front_end::ast::section &v) const
        {
            if (key == "USER") {
                model_printer_type<map_of_users>()(out, model, v);

            } else {
                // TODO(burlog): what to do?
            }
        }
    };

}}}

BOOST_AUTO_TEST_CASE(test_simple_value)
{
    namespace bfe = boost::cppte::front_end;
    map_of_strings model;
    model["NAME"] = "Boosties";
    bfe::ast::stache_root ast = parse("Hello, {{NAME}}!");
    std::string result = print(ast, model);
    BOOST_CHECK_EQUAL("Hello,"
            " "
            "Boosties!", result);
}

BOOST_AUTO_TEST_CASE(test_section_printing)
{
    namespace bfe = boost::cppte::front_end;
    user bob;
    bob.name = "Bob";
    bob.location = "Earth";
    bob.favorites.push_back(map_of_strings());
    bob.favorites.back()["FOOD"] = "Pizza";
    bob.favorites.back()["MUSIC"] = "Classical";
    map_of_users model;
    model["USER"] = bob;
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
            "user.favorite.music=Classical\n",
            print(ast, model));
}

