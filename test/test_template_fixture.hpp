#pragma once

#include <boost/cppte/front_end/stache_ast.hpp>

#include <map>
#include <string>

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
			//liaw2014::RenderAST t(template_string);
			//liaw2014::Renderer renderer(t);
			//result = renderer.render(ctx);
			//return result;
			return "";
		}

		void set_tag_value(const std::string& tag, const std::string& value)
		{
			//ctx[tag] = value;
		}

		typedef std::map<std::string, std::string> object_t;

		void add_section_item(const std::string& section_tag, const object_t& item)
		{
		}

		void register_partial_template(const std::string& name, const std::string& template_string)
		{
		}

		std::string result;
		std::string template_string;

	private:
		//liaw2014::RenderData ctx;
};

