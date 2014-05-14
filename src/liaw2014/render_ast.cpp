#include "liaw2014/render_ast.hpp"

namespace liaw2014
{
	void RenderAST::set_template_text(std::string template_text)
	{
		m_template_text = std::move(template_text);
	}
	std::string RenderAST::template_text()
	{
		return m_template_text;
	}
}
