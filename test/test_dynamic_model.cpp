#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <map>
#include <unordered_map>
#include <boost/test/unit_test.hpp>
#include <boost/cppte/frontend/stache_model.hpp>
#include <boost/cppte/frontend/stache_dynamic_model_printer.hpp>
#include <boost/cppte/simple_parser.hpp>

#include <fstream>

using namespace boost::cppte::front_end;

namespace
{

    typedef std::map<std::string, std::string> map_of_strings;
    typedef std::unordered_map<std::string, std::string> umap_of_strings;

    struct user {
        std::string name;
        std::string location;
        std::vector<map_of_strings> favorites;
        std::vector<umap_of_strings> unloved;
    };
    typedef std::map<std::string, user> map_of_users;

    template <typename model_type>
    std::string print(const ast::stache_root& ast, const model_type& model)
    {
        std::ostringstream out;
        boost::cppte::front_end::ast::print(out, ast, model);
        return out.str();
    }

    ast::stache_root parse(const std::string& text)
    {
        ast::stache_root ast;
        if( !boost::simple_parse_template(text, ast) )
        {
            throw std::runtime_error("Parse failed");
        }
        return ast;
    }

}

namespace boost { namespace cppte { namespace front_end { namespace ast
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
    std::string get_variable_value(const std::pair<const std::string, user> &model,
                                   const std::string &key)
    {
        if (key == "NAME") {
            return model.second.name;

        } else if (key == "LOCATION") {
            return model.second.location;
        }
        return "undefined:" + key;
    }

    template <template <typename> class model_printer_type>
    struct call_me_back_please<model_printer_type, std::pair<const std::string, user>>
    {
        void operator()(const std::pair<const std::string, user> &model,
                        const std::string &key,
                        std::ostream &out,
                        const section &v) const
        {
            if (key == "FAVORITES") {
                model_printer_type<decltype(model.second.favorites)>()(out, model.second.favorites, v);

            } else if (key == "UNLOVED") {
                model_printer_type<decltype(model.second.unloved)>()(out, model.second.unloved, v);

            } else {
                // TODO(burlog): what to do?
            }
        }
    };

    template <template <typename> class model_printer_type>
    struct call_me_back_please<model_printer_type, map_of_users>
    {
        void operator()(const map_of_users &model,
                        const std::string &key,
                        std::ostream &out,
                        const section &v) const
        {
            if (key == "USER") {
                model_printer_type<map_of_users>()(out, model, v);

            } else {
                // TODO(burlog): what to do?
            }
        }
    };
}}}}


BOOST_AUTO_TEST_CASE(test_simple_dynamic_model_formatting)
{
    map_of_strings model;
    model["NAME"] = "Boosties";
    ast::stache_root ast = parse("Hello, {{NAME}}!");
    std::string result = print(ast, model);
    // FIXME! This should have whitespace, but it looks like the parser is stripping it.
    BOOST_CHECK_EQUAL("Hello,"
            " "
            "Boosties!", result);
}

BOOST_AUTO_TEST_CASE(test_section_printing)
{
    user bob;
    bob.name = "Bob";
    bob.location = "Earth";
    bob.favorites.push_back(map_of_strings());
    bob.favorites.back()["FOOD"] = "Pizza";
    bob.favorites.back()["MUSIC"] = "Classical";
    map_of_users model;
    model["USER"] = bob;
    ast::stache_root ast = parse(
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

