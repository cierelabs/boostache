#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <map>
#include <sstream>
#include <unordered_map>
#include <boost/test/unit_test.hpp>
#include <boost/cppte/frontend/stache_model.hpp>
#include <boost/cppte/model/stache_dynamic_model_printer.hpp>
#include <boost/cppte/simple_parser.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::cppte::front_end;

namespace
{
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
    std::string get_variable_value(const std::pair<const std::string,
                                        boost::property_tree::ptree> &model,
                                   const std::string &key)
    {
        return model.second.get(key, "gv_undefined:" + key);
    }

    template <template <typename> class model_printer_type>
    struct call_me_back_please<model_printer_type,
        std::pair<const std::string, boost::property_tree::ptree> >
    {
        void operator()(const std::pair<const std::string,
                                        boost::property_tree::ptree> &model,
                        const std::string &key,
                        std::ostream &out,
                        const section &v) const
        {
            model_printer_type<boost::property_tree::ptree>()
                (out, model.second.get_child(key), v);
        }
    };

    template <template <typename> class model_printer_type>
    struct call_me_back_please<model_printer_type, boost::property_tree::ptree>
    {
        void operator()(const boost::property_tree::ptree &model,
                        const std::string &key,
                        std::ostream &out,
                        const section &v) const
        {
            model_printer_type<boost::property_tree::ptree>()
                (out, model, v);
        }
    };
}}}}

BOOST_AUTO_TEST_CASE(test_json_section_printing)
{
    namespace bpt = boost::property_tree;
    bpt::ptree model;
    model.put("USER.NAME", "Bob");
    model.put("USER.LOCATION", "Earth");

    bpt::ptree fs;
    bpt::ptree f1;
    f1.put("FOOD", "Pizza");
    f1.put("MUSIC", "Classical");
    fs.push_back(std::make_pair("", f1));

    //bpt::ptree f2;
    //f2.put("FOOD", "Knedlik");
    //f2.put("MUSIC", "Folk");
    //fs.push_back(std::make_pair("", f2));

    model.add_child("USER.FAVORITES", fs);

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

