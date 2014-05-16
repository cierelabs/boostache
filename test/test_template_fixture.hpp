#pragma once

#include <boost/cppte/frontend/stache_ast.hpp>
#include <boost/cppte/frontend/stache_model.hpp>
#include <boost/cppte/frontend/stache_model_printer.hpp>
#include <boost/cppte/simple_parser.hpp>

#include <map>
#include <string>

using namespace boost::cppte::front_end;

struct TemplateFixture
{
	public:
		TemplateFixture()
		{
		}
		~TemplateFixture()
		{
		}

		std::string generate_template()
		{
			boost::cppte::front_end::ast::stache_root ast;
			if( !boost::simple_parse_template(template_string, ast) )
			{
				throw std::runtime_error("Parse failed");
			}
			std::ostringstream out;
			boost::cppte::front_end::ast::print(out, ast, model);
			result = out.str();
			return result;
		}

		void set_tag_value(const std::string& tag, const std::string& value)
		{
			model[tag] = value;
		}

		typedef std::map<std::string, std::string> object_t;

		void add_section_item(const std::string& section_tag, const object_t& item)
		{
			auto model_item = boost::cppte::front_end::stache_model { };
			for (auto& x : item)
			{
				model_item[x.first] = x.second;
			}
			if (model.find(section_tag) == model.end())
			{
				model[section_tag] = stache_model_vector { };
			}
			boost::get<stache_model_vector>(model[section_tag]).push_back(model_item);
		}

		void register_partial_template(const std::string& name, const std::string& template_string)
		{
		}

		boost::cppte::front_end::stache_model model;

		std::string result;
		std::string template_string;

	private:
		//liaw2014::RenderData ctx;
};

