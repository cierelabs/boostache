#pragma once
#include <string>

namespace liaw2014
{
	class RenderAST
	{
	public:
		RenderAST()
		{
		}

		RenderAST(const std::string& template_text)
			: m_template_text(template_text)
		{
		}

		static RenderAST load_file(const std::string& path)
		{
			return RenderAST();
		}

		void set_template_text(std::string template_text);
		std::string template_text();
	private:
		std::string m_template_text;
	};
}
