#include "liaw2014/render_ast.hpp"

namespace liaw2014
{
	void RenderAST::set_text(std::string text)
	{
		m_text = std::move(text);
	}
	std::string RenderAST::text()
	{
		return m_text;
	}
}
